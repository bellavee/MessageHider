#include "PngImage.h"

PngImage::PngImage() : m_hBitmap(NULL), m_width(0), m_height(0) {}

PngImage::~PngImage() {
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
    }
}

void PngImage::LoadFromFile(const std::string& filename) {

}

void PngImage::Render(HDC hdc, int x, int y) const {

}

uint32_t PngImage::ReadUint32(const uint8_t* data) {

}

std::vector<uint8_t> PngImage::ParsePng(const std::string& filename, PNG_IHDR& ihdr) {

}

void PngImage::CreateDIBSection(const std::vector<uint8_t>& idat, const PNG_IHDR& ihdr) {

}
