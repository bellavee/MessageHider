#include "Steganography.h"

Steganography::~Steganography()
{
	delete m_image;
}

bool Steganography::Encode(const WCHAR* image, std::string message)
{
	for (m_pixelY = 0; m_pixelY < m_height; m_pixelY++)
	{
		for (m_pixelX = 0; m_pixelX < m_width; m_pixelX++)
		{
			if (!ChangePixelColor()) return true;
		}
	}

	return true;
}

std::string Steganography::Decode()
{
	m_message.clear();

	int width = m_image->GetWidth();
	int height = m_image->GetHeight();

	for (m_pixelY = 0; m_pixelY < height; m_pixelY++)
	{
		for (m_pixelX = 0; m_pixelX < width; m_pixelX++)
		{
			if (!GetPixelBinary()) return m_message;
		}
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

void Steganography::SetDatas(const WCHAR* image, std::string message)
{
	m_image = new Bitmap(image);

	m_index = 0;
	m_message = message;
	m_message += END_CHAR;
	ConvertMessageToBinary();

	m_width = m_image->GetWidth();
	m_height = m_image->GetHeight();
	m_size = m_width * m_height;
}