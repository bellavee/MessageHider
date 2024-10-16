#pragma once

#include "Image.h"

class PngImage : public Image {
public:
    PngImage();
    ~PngImage();

    void LoadFromFile(const std::string& filename) override;
    void Render(HDC hdc, int x, int y, int desiredWidth) const override;

private:
    struct PNG_IHDR {
        uint32_t width;
        uint32_t height;
        uint8_t bit_depth;
        uint8_t color_type;
        uint8_t compression_method;
        uint8_t filter_method;
        uint8_t interlace_method;
    };

    static uint32_t ReadUint32(const uint8_t* data);
    std::vector<uint8_t> ParsePng(const std::string& filename, PNG_IHDR& ihdr);
    void CreateBitmapFromPngData(const std::vector<uint8_t>& idat, const PNG_IHDR& ihdr);
};