#include "PngImage.h"

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
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, m_hBitmap);

        BitBlt(hdc, x, y, m_width, m_height, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hOldBitmap);
        DeleteDC(hdcMem);
    }
}

uint32_t PngImage::ReadUint32(const uint8_t* data) {
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

std::vector<uint8_t> PngImage::ParsePng(const std::string& filename, PNG_IHDR& ihdr) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    // Read PNG signature
    uint8_t signature[8];
    file.read(reinterpret_cast<char*>(signature), 8);
    if (memcmp(signature, "\x89PNG\r\n\x1a\n", 8) != 0) {
        throw std::runtime_error("Invalid PNG signature");
    }

    std::vector<uint8_t> idatData;

    while (!file.eof()) {
        uint32_t chunkLength;
        file.read(reinterpret_cast<char*>(&chunkLength), 4);
        chunkLength = _byteswap_ulong(chunkLength);

        char chunkType[5] = { 0 };
        file.read(chunkType, 4);

        if (strcmp(chunkType, "IHDR") == 0) {
            file.read(reinterpret_cast<char*>(&ihdr), sizeof(PNG_IHDR));
            ihdr.width = _byteswap_ulong(ihdr.width);
            ihdr.height = _byteswap_ulong(ihdr.height);
        }
        else if (strcmp(chunkType, "IDAT") == 0) {
            std::vector<uint8_t> chunkData(chunkLength);
            file.read(reinterpret_cast<char*>(chunkData.data()), chunkLength);
            idatData.insert(idatData.end(), chunkData.begin(), chunkData.end());
        }
        else if (strcmp(chunkType, "IEND") == 0) {
            break;
        }
        else {
            file.seekg(chunkLength, std::ios::cur);
        }

        // Skip CRC
        file.seekg(4, std::ios::cur);
    }

    // Decompress IDAT data
    std::vector<uint8_t> decompressedData;
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (inflateInit(&zs) != Z_OK) {
        throw std::runtime_error("Failed to initialize zlib");
    }

    zs.next_in = idatData.data();
    zs.avail_in = idatData.size();

    while (true) {
        std::vector<uint8_t> buffer(16384);
        zs.next_out = buffer.data();
        zs.avail_out = buffer.size();

        int ret = inflate(&zs, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            decompressedData.insert(decompressedData.end(), buffer.begin(), buffer.begin() + (buffer.size() - zs.avail_out));
            break;
        }
        else if (ret != Z_OK) {
            inflateEnd(&zs);
            throw std::runtime_error("Failed to decompress data");
        }

        decompressedData.insert(decompressedData.end(), buffer.begin(), buffer.end());
    }

    inflateEnd(&zs);

    return decompressedData;
}

void PngImage::CreateBitmapFromPngData(const std::vector<uint8_t>& idat, const PNG_IHDR& ihdr) {
    // Convert image data to BGRA format
    std::vector<uint8_t> bgraData(ihdr.width * ihdr.height * 4);
    for (size_t i = 0; i < ihdr.height; ++i) {
        for (size_t j = 0; j < ihdr.width; ++j) {
            size_t srcIdx = (i * ihdr.width + j) * 3;
            size_t dstIdx = (i * ihdr.width + j) * 4;
            bgraData[dstIdx + 0] = idat[srcIdx + 2]; // B
            bgraData[dstIdx + 1] = idat[srcIdx + 1]; // G
            bgraData[dstIdx + 2] = idat[srcIdx + 0]; // R
            bgraData[dstIdx + 3] = 255; // A (full opacity)
        }
    }

    // Create a DIB section
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ihdr.width;
    bmi.bmiHeader.biHeight = -(LONG)ihdr.height; // Top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pBits;
    HDC hdc = GetDC(NULL);
    m_hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
    ReleaseDC(NULL, hdc);

    if (m_hBitmap) {
        memcpy(pBits, bgraData.data(), bgraData.size());
        m_width = ihdr.width;
        m_height = ihdr.height;
    }
    else {
        throw std::runtime_error("Failed to create DIB section");
    }
}
