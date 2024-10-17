#pragma once

#include "framework.h"

class Image {
public:
    virtual ~Image() = default;

    virtual void LoadFromFile(const std::string& filename) = 0;
    virtual void Render(HDC hdc, int x, int y, int desiredWidth, int desiredHeight) const = 0;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    const std::vector<uint8_t>& GetPixelData() const { return m_pixelData; }

    virtual void UpdateBitmap() = 0;
    virtual void ResetToOriginal();

protected:
    HBITMAP m_hBitmap = NULL;
    int m_width = 0;
    int m_height = 0;
    std::vector<uint8_t> m_pixelData;
    std::vector<uint8_t> m_originalPixelData;
};