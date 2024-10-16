//#include "ImageFilter.h"
//
//void BrightnessFilter::Apply(Image& image) const {
//    std::vector<uint8_t>& pixelData = const_cast<std::vector<uint8_t>&>(image.GetPixelData());
//
//    for (size_t i = 0; i < pixelData.size(); i += 4) {
//        for (int j = 0; j < 3; ++j) {  // Adjust R, G, and B channels
//            int newValue = pixelData[i + j] + m_value;
//            pixelData[i + j] = static_cast<uint8_t>(std::clamp(newValue, 0, 255));
//        }
//        // Skip alpha channel (i + 3)
//    }
//
//    // Notify the image that its data has changed
//    image.UpdateBitmap();
//}