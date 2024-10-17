#include "ImageFilter.h"

template<typename T>
T clamp(T value, T min, T max) {
    return (value < min) ? min : ((value > max) ? max : value);
}

void BrightnessFilter::Apply(Image& image) const {
    std::vector<uint8_t>& pixelData = const_cast<std::vector<uint8_t>&>(image.GetPixelData());

    for (size_t i = 0; i < pixelData.size(); i += 4) {
        for (int j = 0; j < 3; ++j) {  // Adjust R, G, and B channels
            int newValue = pixelData[i + j] + m_value;
            pixelData[i + j] = static_cast<uint8_t>(clamp(newValue, 0, 255));
        }
    }

    image.UpdateBitmap();
}

void ContrastFilter::Apply(Image& image) const {
    std::vector<uint8_t>& pixelData = const_cast<std::vector<uint8_t>&>(image.GetPixelData());

    for (size_t i = 0; i < pixelData.size(); i += 4) {
        for (int j = 0; j < 3; ++j) {  // Adjust R, G, and B channels
            float pixelValue = pixelData[i + j] / 255.0f;
            pixelValue = (pixelValue - 0.5f) * m_factor + 0.5f;
            int newValue = static_cast<int>(std::round(pixelValue * 255));
            pixelData[i + j] = static_cast<uint8_t>(clamp(newValue, 0, 255));
        }
    }

    image.UpdateBitmap();
}

void SaturationFilter::Apply(Image& image) const {
    std::vector<uint8_t>& pixelData = const_cast<std::vector<uint8_t>&>(image.GetPixelData());

    for (size_t i = 0; i < pixelData.size(); i += 4) {
        float r = pixelData[i] / 255.0f;
        float g = pixelData[i + 1] / 255.0f;
        float b = pixelData[i + 2] / 255.0f;

        // Convert RGB to HSL
        float max = r > g ? (r > b ? r : b) : (g > b ? g : b);
        float min = r < g ? (r < b ? r : b) : (g < b ? g : b);
        float diff = max - min;
        float l = (max + min) / 2.0f;
        float h, s;

        if (max == min) {
            h = s = 0; // achromatic
        }
        else {
            s = l > 0.5f ? diff / (2.0f - max - min) : diff / (max + min);
            if (max == r) {
                h = (g - b) / diff + (g < b ? 6.0f : 0.0f);
            }
            else if (max == g) {
                h = (b - r) / diff + 2.0f;
            }
            else {
                h = (r - g) / diff + 4.0f;
            }
            h /= 6.0f;
        }

        // Adjust saturation
        s = clamp(s * m_factor, 0.0f, 1.0f);

        // Convert back to RGB
        if (s == 0) {
            r = g = b = l; // achromatic
        }
        else {
            auto hue2rgb = [](float p, float q, float t) {
                if (t < 0) t += 1;
                if (t > 1) t -= 1;
                if (t < 1.0f / 6) return p + (q - p) * 6 * t;
                if (t < 1.0f / 2) return q;
                if (t < 2.0f / 3) return p + (q - p) * (2.0f / 3 - t) * 6;
                return p;
                };

            float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
            float p = 2 * l - q;
            r = hue2rgb(p, q, h + 1.0f / 3);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1.0f / 3);
        }

        // Update pixel data
        pixelData[i] = static_cast<uint8_t>(clamp(r * 255.0f, 0.0f, 255.0f));
        pixelData[i + 1] = static_cast<uint8_t>(clamp(g * 255.0f, 0.0f, 255.0f));
        pixelData[i + 2] = static_cast<uint8_t>(clamp(b * 255.0f, 0.0f, 255.0f));
    }

    // Notify the image that its data has changed
    image.UpdateBitmap();
}