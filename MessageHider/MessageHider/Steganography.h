#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <bitset>

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

#define BIT_LENGTH 8
#define END_CHAR '\0'

class Steganography
{
public:
	~Steganography();
	virtual bool Encode(const WCHAR* image, std::string message);
	virtual std::string Decode();

	Bitmap* GetImage() { return m_image; }

protected:
	void ConvertMessageToBinary();
	void SetDatas(const WCHAR* image, std::string message);
	virtual bool ChangePixelColor() = 0;
	virtual bool GetPixelBinary() = 0;

	std::string m_message;
	Bitmap* m_image;

	// Image
	int m_width;
	int m_height;
	int m_size;

	// Hide message
	std::vector<int> m_binaryMessage;
	int m_index = 0;
	int m_pixelX = 0;
	int m_pixelY = 0;

	// Extract message
	char m_character = 0;
	int m_bits = 0;
};