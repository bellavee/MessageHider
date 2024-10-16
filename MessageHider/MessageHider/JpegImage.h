#pragma once

#include "Image.h"

class JpegImage : public Image {
public:
    JpegImage();
    ~JpegImage();

    void LoadFromFile(const std::string& filename) override;
    void Render(HDC hdc, int x, int y, int desiredWidth, int desiredHeight) const override;

private:
    Gdiplus::Bitmap* m_pBitmap;
    static ULONG_PTR m_gdiplusToken;
    static int m_instanceCount;

    static void InitializeGdiPlus();
    static void ShutdownGdiPlus();
};