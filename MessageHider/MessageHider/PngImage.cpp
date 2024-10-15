#include "PngImage.h"

#include <sstream>

PngImage::PngImage() : m_hBitmap(NULL), m_width(0), m_height(0) {}

PngImage::~PngImage() {
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
    }
}

void PngImage::LoadFromFile(const std::string& filename) {
    PNG_IHDR ihdr;
    std::vector<uint8_t> idat = ParsePng(filename, ihdr);
    CreateBitmapFromPngData(idat, ihdr);
}

void PngImage::Render(HDC hdc, int x, int y) const {
    if (m_hBitmap) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        if (hdcMem == NULL) {
            DWORD error = GetLastError();
            std::stringstream ss;
            ss << "CreateCompatibleDC failed. Error code: " << error;
            MessageBoxA(NULL, ss.str().c_str(), "Error", MB_ICONERROR | MB_OK);
            return;
        }

        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, m_hBitmap);
        if (hOldBitmap == NULL) {
            DWORD error = GetLastError();
            std::stringstream ss;
            ss << "SelectObject failed. Error code: " << error;
            MessageBoxA(NULL, ss.str().c_str(), "Error", MB_ICONERROR | MB_OK);
            DeleteDC(hdcMem);
            return;
        }

        BOOL bltResult = BitBlt(hdc, x, y, m_width, m_height, hdcMem, 0, 0, SRCCOPY);
        if (!bltResult) {
            DWORD error = GetLastError();
            std::stringstream ss;
            ss << "BitBlt failed. Error code: " << error;
            MessageBoxA(NULL, ss.str().c_str(), "Error", MB_ICONERROR | MB_OK);
        }

        SelectObject(hdcMem, hOldBitmap);
        DeleteDC(hdcMem);
    }
    else {
        MessageBoxA(NULL, "m_hBitmap is NULL in Render function", "Error", MB_ICONERROR | MB_OK);
    }
}

uint32_t PngImage::ReadUint32(const uint8_t* data) {
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

std::vector<uint8_t> PngImage::ParsePng(const std::string& filename, PNG_IHDR& ihdr) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    // Check PNG signature
    char signature[8];
    file.read(signature, 8);
    if (memcmp(signature, "\x89PNG\r\n\x1a\n", 8) != 0) {
        throw std::runtime_error("Invalid PNG signature");
    }

    std::vector<uint8_t> idat_data;
    bool ihdr_found = false;

    while (file) {
        uint8_t length_bytes[4];
        file.read(reinterpret_cast<char*>(length_bytes), 4);
        uint32_t chunk_length = ReadUint32(length_bytes);

        char chunk_type[5] = { 0 };
        file.read(chunk_type, 4);

        if (strcmp(chunk_type, "IHDR") == 0) {
            if (chunk_length != 13) {
                throw std::runtime_error("Invalid IHDR chunk size");
            }
            uint8_t ihdr_data[13];
            file.read(reinterpret_cast<char*>(ihdr_data), 13);

            ihdr.width = ReadUint32(ihdr_data);
            ihdr.height = ReadUint32(ihdr_data + 4);
            ihdr.bit_depth = ihdr_data[8];
            ihdr.color_type = ihdr_data[9];
            ihdr.compression_method = ihdr_data[10];
            ihdr.filter_method = ihdr_data[11];
            ihdr.interlace_method = ihdr_data[12];

            m_width = ihdr.width;
            m_height = ihdr.height;
            ihdr_found = true;
        }
        else if (strcmp(chunk_type, "IDAT") == 0) {
            if (!ihdr_found) {
                throw std::runtime_error("IDAT chunk before IHDR chunk");
            }
            std::vector<uint8_t> chunk_data(chunk_length);
            file.read(reinterpret_cast<char*>(chunk_data.data()), chunk_length);
            idat_data.insert(idat_data.end(), chunk_data.begin(), chunk_data.end());
        }
        else if (strcmp(chunk_type, "IEND") == 0) {
            break;
        }
        else {
            // Skip other chunk types
            file.seekg(chunk_length, std::ios::cur);
        }

        // Skip CRC
        file.seekg(4, std::ios::cur);
    }

    if (!ihdr_found) {
        throw std::runtime_error("IHDR chunk not found");
    }

    if (idat_data.empty()) {
        throw std::runtime_error("No IDAT chunks found");
    }

    return idat_data;
}

void PngImage::CreateBitmapFromPngData(const std::vector<uint8_t>& idat, const PNG_IHDR& ihdr) {
    int bytes_per_pixel = 0;
    switch (ihdr.color_type) {
    case 2: bytes_per_pixel = 3; break; // RGB
    case 6: bytes_per_pixel = 4; break; // RGBA
    default:
        MessageBoxA(NULL, "Unsupported color type", "Error", MB_ICONERROR | MB_OK);
        throw std::runtime_error("Unsupported color type");
    }

    int stride = m_width * bytes_per_pixel + 1; // +1 for filter byte
    std::vector<uint8_t> decompressed_data(stride * m_height);

    z_stream zstream = {};
    if (inflateInit(&zstream) != Z_OK) {
        MessageBoxA(NULL, "Failed to initialize zlib", "Error", MB_ICONERROR | MB_OK);
        throw std::runtime_error("Failed to initialize zlib");
    }

    zstream.next_in = const_cast<Bytef*>(idat.data());
    zstream.avail_in = idat.size();
    zstream.next_out = decompressed_data.data();
    zstream.avail_out = decompressed_data.size();

    int result = inflate(&zstream, Z_FINISH);
    inflateEnd(&zstream);

    if (result != Z_STREAM_END) {
        MessageBoxA(NULL, "Failed to decompress PNG data", "Error", MB_ICONERROR | MB_OK);
        throw std::runtime_error("Failed to decompress PNG data");
    }

    // Unfilter the data and convert to BGRA
    std::vector<uint8_t> bgra_data(m_width * m_height * 4);
    for (int y = 0; y < m_height; ++y) {
        uint8_t filter = decompressed_data[y * stride];
        for (int x = 0; x < m_width; ++x) {
            for (int c = 0; c < bytes_per_pixel; ++c) {
                uint8_t filtered_byte = decompressed_data[y * stride + 1 + x * bytes_per_pixel + c];
                uint8_t unfiltered_byte = filtered_byte;

                switch (filter) {
                case 0: // None
                    break;
                case 1: // Sub
                    if (x > 0) unfiltered_byte += bgra_data[y * m_width * 4 + (x - 1) * 4 + c];
                    break;
                case 2: // Up
                    if (y > 0) unfiltered_byte += bgra_data[(y - 1) * m_width * 4 + x * 4 + c];
                    break;
                case 3: // Average
                {
                    uint16_t a = (x > 0) ? bgra_data[y * m_width * 4 + (x - 1) * 4 + c] : 0;
                    uint16_t b = (y > 0) ? bgra_data[(y - 1) * m_width * 4 + x * 4 + c] : 0;
                    unfiltered_byte += (a + b) / 2;
                }
                break;
                case 4: // Paeth
                {
                    int a = (x > 0) ? bgra_data[y * m_width * 4 + (x - 1) * 4 + c] : 0;
                    int b = (y > 0) ? bgra_data[(y - 1) * m_width * 4 + x * 4 + c] : 0;
                    int c_ = (x > 0 && y > 0) ? bgra_data[(y - 1) * m_width * 4 + (x - 1) * 4 + c] : 0;
                    int p = a + b - c_;
                    int pa = std::abs(p - a);
                    int pb = std::abs(p - b);
                    int pc = std::abs(p - c_);
                    if (pa <= pb && pa <= pc) unfiltered_byte += a;
                    else if (pb <= pc) unfiltered_byte += b;
                    else unfiltered_byte += c_;
                }
                break;
                }

                // Convert RGBA to BGRA
                int bgra_index;
                if (c < 3) {
                    bgra_index = y * m_width * 4 + x * 4 + (2 - c); // Swap R and B
                }
                else {
                    bgra_index = y * m_width * 4 + x * 4 + c; // Alpha stays in the same position
                }
                bgra_data[bgra_index] = unfiltered_byte;
            }
            if (bytes_per_pixel == 3) {
                // If RGB, set alpha to 255
                bgra_data[y * m_width * 4 + x * 4 + 3] = 255;
            }
        }
    }

    // Create bitmap
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = m_width;
    bmi.bmiHeader.biHeight = -m_height;  // Negative for top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // Always use 32 bits for BGRA
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(NULL);
    if (hdc == NULL) {
        DWORD error = GetLastError();
        std::stringstream ss;
        ss << "GetDC failed. Error code: " << error;
        MessageBoxA(NULL, ss.str().c_str(), "Error", MB_ICONERROR | MB_OK);
        throw std::runtime_error("Failed to get device context");
    }

    void* bits;
    m_hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);

    if (!m_hBitmap) {
        DWORD error = GetLastError();
        std::stringstream ss;
        ss << "CreateDIBSection failed. Error code: " << error;
        MessageBoxA(NULL, ss.str().c_str(), "Error", MB_ICONERROR | MB_OK);
        ReleaseDC(NULL, hdc);
        throw std::runtime_error("Failed to create bitmap");
    }

    // Copy pixel data to bitmap
    memcpy(bits, bgra_data.data(), bgra_data.size());

    ReleaseDC(NULL, hdc);

    std::stringstream ss;
    ss << "Bitmap created successfully. Width: " << m_width << ", Height: " << m_height;
    MessageBoxA(NULL, ss.str().c_str(), "Success", MB_ICONINFORMATION | MB_OK);
}