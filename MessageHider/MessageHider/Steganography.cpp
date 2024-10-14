#include "Steganography.h"

bool Steganography::HideMessage(std::string message)
{
	m_index = 0;
	m_message = message;
	ConvertMessageToBinary();

	int width = 0; // get image width
	int height = 0; // get image height
	int imageSize = 0; // get image pixels size

	// check message length, return error if too long
	if (message.size() * 8 > imageSize) return false;

	for (m_pixelY = 0; m_pixelY < height; m_pixelY++)
	{
		for (m_pixelX = 0; m_pixelX < width; m_pixelX++)
		{
			if (!ChangePixel()) break;
		}

		if (!ChangePixel()) break;
	}

	return true;
}

void Steganography::GetImage()
{
	//
}

std::string Steganography::ExtractMessage()
{
	return "todo";
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

bool Steganography::ChangePixel()
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