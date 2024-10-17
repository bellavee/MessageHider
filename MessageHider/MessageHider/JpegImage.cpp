#include "JpegImage.h"

ULONG_PTR JpegImage::m_gdiplusToken = 0;
int JpegImage::m_instanceCount = 0;

JpegImage::JpegImage() : m_pBitmap(nullptr) {
    if (m_instanceCount == 0) {
        InitializeGdiPlus();
    }
    m_instanceCount++;
}

JpegImage::~JpegImage() {
    if (m_pBitmap) {
        delete m_pBitmap;
    }

    m_instanceCount--;
    if (m_instanceCount == 0) {
        ShutdownGdiPlus();
    }
}

void JpegImage::LoadFromFile(const std::string& filename) {
    std::wstring wfilename(filename.begin(), filename.end());
    m_pBitmap = Gdiplus::Bitmap::FromFile(wfilename.c_str());

    if (m_pBitmap->GetLastStatus() != Gdiplus::Ok) {
        throw std::runtime_error("Failed to load JPEG image");
    }

    m_width = m_pBitmap->GetWidth();
    m_height = m_pBitmap->GetHeight();

    // Extract pixel data from the bitmap
    Gdiplus::BitmapData bitmapData;
    Gdiplus::Rect rect(0, 0, m_width, m_height);
    m_pBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);

    m_pixelData.resize(m_width * m_height * 4);
    uint8_t* pPixels = static_cast<uint8_t*>(bitmapData.Scan0);
    int stride = bitmapData.Stride;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int sourceIndex = y * stride + x * 4;
            int destIndex = (y * m_width + x) * 4;

            // Convert from ARGB to BGRA
            m_pixelData[destIndex + 0] = pPixels[sourceIndex + 2]; // B
            m_pixelData[destIndex + 1] = pPixels[sourceIndex + 1]; // G
            m_pixelData[destIndex + 2] = pPixels[sourceIndex + 0]; // R
            m_pixelData[destIndex + 3] = pPixels[sourceIndex + 3]; // A
        }
    }

    m_pBitmap->UnlockBits(&bitmapData);

    // Store the original pixel data
    m_originalPixelData = m_pixelData;
}

void JpegImage::Render(HDC hdc, int x, int y, int desiredWidth, int desiredHeight) const {
    if (m_pBitmap) {
        Gdiplus::Graphics graphics(hdc);
        graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
        graphics.DrawImage(m_pBitmap, x, y, desiredWidth, desiredHeight);
    }
}

void JpegImage::InitializeGdiPlus() {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

void JpegImage::ShutdownGdiPlus() {
    Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

void JpegImage::UpdateBitmap() {
    if (m_pBitmap && !m_pixelData.empty()) {
        Gdiplus::BitmapData bitmapData;
        Gdiplus::Rect rect(0, 0, m_width, m_height);

        m_pBitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

        uint8_t* pPixels = static_cast<uint8_t*>(bitmapData.Scan0);
        int stride = bitmapData.Stride;

        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                int sourceIndex = (y * m_width + x) * 4;
                int destIndex = y * stride + x * 4;

                // Convert from BGRA to ARGB
                pPixels[destIndex + 0] = m_pixelData[sourceIndex + 2]; // R
                pPixels[destIndex + 1] = m_pixelData[sourceIndex + 1]; // G
                pPixels[destIndex + 2] = m_pixelData[sourceIndex + 0]; // B
                pPixels[destIndex + 3] = m_pixelData[sourceIndex + 3]; // A
            }
        }

        m_pBitmap->UnlockBits(&bitmapData);
    }
}