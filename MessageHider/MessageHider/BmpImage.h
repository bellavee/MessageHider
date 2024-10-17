#pragma once

#include "Image.h"

class BmpImage : public Image {
public:
    BmpImage();
    ~BmpImage();

    void LoadFromFile(const std::string& filename) override;
    void Render(HDC hdc, int x, int y, int desiredWidth, int desiredHeight) const override;
    void UpdateBitmap() override;

private:
    void ParseBmpHeader(std::ifstream& file);
    void ReadPixelData(std::ifstream& file);
    void CreateBitmapFromPixelData();

    BITMAPFILEHEADER m_fileHeader;
    BITMAPINFOHEADER m_infoHeader;
};