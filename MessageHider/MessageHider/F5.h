#pragma once

#include "Steganography.h"

class F5 : public Steganography
{
public:
	bool Encode(const WCHAR* image, std::string message) override;
	//std::string Decode() override;

protected:
	bool ChangePixelColor() override;
	bool GetPixelBinary() override;
};