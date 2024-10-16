#include "JpegImage.h"
#include <stdexcept>

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
}

void JpegImage::Render(HDC hdc, int x, int y) const {
    if (m_pBitmap) {
        Gdiplus::Graphics graphics(hdc);
        graphics.DrawImage(m_pBitmap, x, y, m_width, m_height);
    }
}

void JpegImage::InitializeGdiPlus() {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

void JpegImage::ShutdownGdiPlus() {
    Gdiplus::GdiplusShutdown(m_gdiplusToken);
}