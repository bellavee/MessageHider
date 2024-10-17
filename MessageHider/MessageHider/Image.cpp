#include "Image.h"

void Image::ResetToOriginal()
{
    if (!m_originalPixelData.empty())
    {
        m_pixelData = m_originalPixelData;
        UpdateBitmap();
    }
}