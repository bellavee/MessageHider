#include "F5.h"

bool F5::Encode(std::string message)
{
    if (!Steganography::Encode(message)) return false;

    //

    Reset();
    return true;
}

std::string F5::Decode()
{
	return std::string();
}