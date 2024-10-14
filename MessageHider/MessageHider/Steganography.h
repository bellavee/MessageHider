#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <bitset>

#define BIT_LENGTH 8
#define END_CHAR '\0'

class Steganography
{
public:
	bool HideMessage(std::string message); // bitmap en argument
	std::string ExtractMessage();

	//void GetImage() { return m_bitmap; }

private:
	void ConvertMessageToBinary();
	bool ChangePixelColor();
	bool GetPixelBinary();

	std::string m_message;
	// Image m_bitmap

	// Hide message
	std::vector<int> m_binaryMessage;
	int m_index = 0;
	int m_pixelX = 0;
	int m_pixelY = 0;

	// Extract message
	char m_character = 0;
	int m_bits = 0;
};