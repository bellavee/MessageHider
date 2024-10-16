#include "LSB.h"

bool LSB::Encode(std::string message)
{
	if (!Steganography::Encode(message)) return false;

	for (size_t i = 0; EncodeIsOver(i); i++)
	{
		uint8_t byte = m_imageBytes[i];
		bool bit = m_binaryMessage[m_messageIndex] == '1';

		// Masquage (AND) puis insertion du bit (OR)
		m_imageBytes[i] = (byte & 0xFE) | bit;
		m_messageIndex++;
	}

	Reset();
	return true;
}

std::string LSB::Decode()
{
	m_message.clear();
	uint8_t character = 0;

	for (size_t i = 0; i < m_imageBytes.size(); i++)
	{
		// On récupère le bit
		uint8_t byte = m_imageBytes[i];

		character <<= 1;
		character |= (byte & 0x01);

		// Tous les 8 bits récupérés, on reconstitue le character
		// Si on trouve le character nul, on s'arrête
		if ((i + 1) % 8 == 0)
		{
			if (character == 0) break;
			m_message += static_cast<char>(character);
			character = 0;
		}
	}

	return m_message;
}