#pragma once

#include "Steganography.h"

class LSB : public Steganography
{
public:
	bool Encode(const WCHAR* image, std::string message) override;
	//std::string Decode() override;

private:
	bool ChangePixelColor() override;
	bool GetPixelBinary() override;
};