#pragma once

#include "Steganography.h"

class F5 : public Steganography
{
public:
	bool Encode(std::string message) override;
	std::string Decode() override;
};