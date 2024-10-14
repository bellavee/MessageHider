#include "LSB.h"

bool LSB::Encode(const WCHAR* image, std::string message)
{
	SetDatas(image, message);

	// check message length, return error if empty or too long
	if (message.empty() || m_binaryMessage.size() > m_size) return false;

	return Steganography::Encode(image, message);
}

bool LSB::ChangePixelColor()
{
	// Return false when all characters are hidden

	int length = m_binaryMessage.size();

	if (m_index >= length) return false;

	// Get pixel
	Color pixelColor;
	m_image->GetPixel(m_pixelX, m_pixelY, &pixelColor);

	int bit = m_binaryMessage[m_index];

	// Set pixel
	BYTE blue = pixelColor.GetB();
	blue = (blue & 0xFE) | bit;
	Color newColor(pixelColor.GetA(), pixelColor.GetR(), pixelColor.GetG(), blue);
	m_image->SetPixel(m_pixelX, m_pixelY, newColor);

	m_index++;
	return true;
}

bool LSB::GetPixelBinary()
{
	// Return false when all characters are extracted

	// Get pixel
	Color pixelColor;
	m_image->GetPixel(m_pixelX, m_pixelY, &pixelColor);
	BYTE blue = pixelColor.GetB();
	int bit = blue & 1;

	m_character |= (bit << m_bits);
	m_bits++;

	// Check if character is finished
	if (m_bits == BIT_LENGTH)
	{
		// Check if message is finished
		if (m_character == END_CHAR) return false;

		m_message.push_back(m_character);
		m_character = 0;
		m_bits = 0;
	}

	return true;
}