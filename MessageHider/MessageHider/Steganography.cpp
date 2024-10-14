#include "Steganography.h"

bool Steganography::HideMessage(std::string message)
{
	m_index = 0;
	m_message = message;
	m_message += END_CHAR;
	ConvertMessageToBinary();

	int width = 0; // get image width
	int height = 0; // get image height
	int imageSize = 0; // get image pixels size

	// check message length, return error if empty or too long
	if (message.empty() || m_message.size() * 8 > imageSize) return false;

	for (m_pixelY = 0; m_pixelY < height; m_pixelY++)
	{
		for (m_pixelX = 0; m_pixelX < width; m_pixelX++)
		{
			if (!ChangePixelColor()) break;
		}

		if (!ChangePixelColor()) break;
	}

	return true;
}

std::string Steganography::ExtractMessage()
{
	m_message.clear();

	int width = 0; // get image width
	int height = 0; // get image height

	for (m_pixelY = 0; m_pixelY < height; m_pixelY++)
	{
		for (m_pixelX = 0; m_pixelX < width; m_pixelX++)
		{
			if (!GetPixelBinary()) return m_message;
		}

		if (!GetPixelBinary()) return m_message;
	}

	return m_message;
}

void Steganography::ConvertMessageToBinary()
{
	m_binaryMessage.clear();

	for (char c : m_message) {
		int ascii = static_cast<int>(c); // convert to base 10 (ascii)
		std::bitset<8> binary(ascii); // convert to binary (8bits)

		for (int i = 7; i >= 0; --i) m_binaryMessage.push_back(binary[i]);
	}
}

bool Steganography::ChangePixelColor()
{
	int length = m_message.size() * 8;

	if (m_index >= length) return false;

	// Get pixel
	//Color pixelColor;
	//m_bitmap.GetPixel(m_x, m_y, &pixelColor);

	int bit = m_binaryMessage[m_index];

	// Set pixel
	//BYTE blue = pixelColor.GetBlue();
	//blue = (blue & 0xFE) | bit;
	//Color newColor(pixelColor.GetRed(), pixelColor.GetGreen(), blue, pixelColor.GetAlpha());
	//m_bitmap.SetPixel(m_x, m_y, newColor);

	m_index++;
	return true;
}

bool Steganography::GetPixelBinary()
{
	// Get pixel
	//Color pixelColor;
	//m_bitmap.GetPixel(m_pixelX, m_pixelY, &pixelColor);
	//BYTE blue = pixelColor.GetBlue();
	//int bit = blue & 1;

	//m_character |= (bit << m_bits);
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
}