#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <bitset>

class Steganography
{
public:
	bool HideMessage(std::string message); // bitmap en argument
	void GetImage(); // retourner la classe Image
	std::string ExtractMessage();

private:
	void ConvertMessageToBinary();
	bool ChangePixel();

	std::string m_message;
	std::vector<int> m_binaryMessage;
	int m_index = 0;
	int m_pixelX = 0;
	int m_pixelY = 0;
	// Image m_bitmap
};