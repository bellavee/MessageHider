#include "BmpImage.h"
#include <fstream>
#include <stdexcept>

BmpImage::BmpImage() : Image() {}

BmpImage::~BmpImage() {
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
    }
}

void BmpImage::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    ParseBmpHeader(file);
    ReadPixelData(file);

    m_originalPixelData = m_pixelData;
    CreateBitmapFromPixelData();
}

void BmpImage::ParseBmpHeader(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&m_fileHeader), sizeof(BITMAPFILEHEADER));
    file.read(reinterpret_cast<char*>(&m_infoHeader), sizeof(BITMAPINFOHEADER));

    if (m_fileHeader.bfType != 0x4D42) { // "BM"
        throw std::runtime_error("Not a valid BMP file");
    }

    m_width = m_infoHeader.biWidth;
    m_height = std::abs(m_infoHeader.biHeight); // Height can be negative for top-down BMPs
}

void BmpImage::ReadPixelData(std::ifstream& file) {
    file.seekg(m_fileHeader.bfOffBits, std::ios::beg);

    int bytesPerPixel = m_infoHeader.biBitCount / 8;
    int paddingSize = (4 - (m_width * bytesPerPixel) % 4) % 4;
    bool isTopDown = m_infoHeader.biHeight < 0;

    m_pixelData.resize(m_width * m_height * 4); // Always store as BGRA

    std::vector<uint8_t> rowData(m_width * bytesPerPixel + paddingSize);

    for (int y = 0; y < m_height; ++y) {
        file.read(reinterpret_cast<char*>(rowData.data()), rowData.size());

        int destY = isTopDown ? y : m_height - 1 - y;

        for (int x = 0; x < m_width; ++x) {
            int srcIndex = x * bytesPerPixel;
            int destIndex = (destY * m_width + x) * 4;

            switch (bytesPerPixel) {
            case 3: // 24-bit BMP
                m_pixelData[destIndex] = rowData[srcIndex];     // B
                m_pixelData[destIndex + 1] = rowData[srcIndex + 1]; // G
                m_pixelData[destIndex + 2] = rowData[srcIndex + 2]; // R
                m_pixelData[destIndex + 3] = 255;                   // A (fully opaque)
                break;
            case 4: // 32-bit BMP
                m_pixelData[destIndex] = rowData[srcIndex];     // B
                m_pixelData[destIndex + 1] = rowData[srcIndex + 1]; // G
                m_pixelData[destIndex + 2] = rowData[srcIndex + 2]; // R
                m_pixelData[destIndex + 3] = rowData[srcIndex + 3]; // A
                break;
            }
        }
    }
}

void BmpImage::CreateBitmapFromPixelData() {
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
    }

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = m_width;
    bmi.bmiHeader.biHeight = -m_height;  // Negative for top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(NULL);
    void* pBits;
    m_hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
    if (m_hBitmap) {
        memcpy(pBits, m_pixelData.data(), m_pixelData.size());
    }
    else {
        throw std::runtime_error("Failed to create bitmap");
    }
    ReleaseDC(NULL, hdc);
}

void BmpImage::Render(HDC hdc, int x, int y, int desiredWidth, int desiredHeight) const {
    if (m_hBitmap) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, m_hBitmap);

        SetStretchBltMode(hdc, HALFTONE);
        StretchBlt(hdc, x, y, desiredWidth, desiredHeight, hdcMem, 0, 0, m_width, m_height, SRCCOPY);

        SelectObject(hdcMem, hOldBitmap);
        DeleteDC(hdcMem);
    }
}

void BmpImage::UpdateBitmap() {
    CreateBitmapFromPixelData();
}