#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <fstream>

#include "AppManager.h"

#define BIT_LENGTH 8
#define NULL_BYTE '\0'

// ----- Explications -----
// Quelle que soit la m�thode utilis�e, il faut :
// - coder chaque caract�re du message secret en base 10 (ascii)
// - puis convertir le message en binaire
// Il faut terminer le message sur un caract�re nul pour l'extraction

class Steganography
{
public:
	virtual bool Encode(std::string message);
	virtual std::string Decode() = 0;
	std::vector<uint8_t> GetImageBytes() const { return m_imageBytes; }
	void SaveAsBmp(const std::string& filename);

protected:
	void Reset();
	void ConvertMessageToBinary();
	bool EncodeIsOver(size_t i);
	bool HandleErrors();

	void GetImageDatas();

	std::string m_message;
	std::string m_binaryMessage;

	std::vector<uint8_t> m_imageBytes;
	int m_messageIndex = 0;
};