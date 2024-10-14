#include "F5.h"

bool F5::Encode(const WCHAR* image, std::string message)
{
    SetDatas(image, message);

    // check message length, return error if empty or too long
    if (message.empty() || m_binaryMessage.size() > (m_size * 2)) return false;

    return Steganography::Encode(image, message);
}

bool F5::ChangePixelColor()
{
    // Return false when all characters are hidden

    int length = m_binaryMessage.size();

    // Get pixel
    Color pixelColor;
    m_image->GetPixel(m_pixelX, m_pixelY, &pixelColor);

    for (size_t i = 0; i < 2; i++)
    {
        if (m_index >= length) return false;

        int bit = m_binaryMessage[m_index];

        // Set pixel
        BYTE blue = pixelColor.GetB();
        blue = (blue & 0xFC) | (bit & 0x03);
        pixelColor = Color(pixelColor.GetA(), pixelColor.GetR(), pixelColor.GetG(), blue);

        m_index++;
    }

    m_image->SetPixel(m_pixelX, m_pixelY, pixelColor);
    return true;
}

bool F5::GetPixelBinary()
{
    // Return false when all characters are extracted

    // Get pixel
    Color pixelColor;
    m_image->GetPixel(m_pixelX, m_pixelY, &pixelColor);
    BYTE blue = pixelColor.GetB();

    int bit = blue & 0x01;

    m_character |= (bit << m_bits);
    m_bits++;

    bit = (blue >> 1) & 0x01;

    m_character |= (bit << m_bits);
    m_bits++;

    if (m_bits == BIT_LENGTH)
    {
        if (m_character == END_CHAR) return false;

        m_message.push_back(m_character);
        m_character = 0;
        m_bits = 0;
    }

    return true;
}