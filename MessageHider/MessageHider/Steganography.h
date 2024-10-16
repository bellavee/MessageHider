#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <fstream>

#define BIT_LENGTH 8
#define NULL_BYTE '\0'

// ----- Explications -----
// Quelle que soit la méthode utilisée, il faut :
// - coder chaque caractère du message secret en base 10 (ascii)
// - puis convertir le message en binaire
// Il faut terminer le message sur un caractère nul pour l'extraction

class Steganography
{
public:
	virtual bool Encode(std::string message);
	virtual std::string Decode() = 0;

protected:
	void Reset();
	void ConvertMessageToBinary();
	bool EncodeIsOver(size_t i);
	bool HandleErrors();

	void GetImageData();

	std::string m_message;
	std::string m_binaryMessage;

	std::vector<uint8_t> m_imageBytes;
	int m_messageIndex = 0;
};