#include "Steganography.h"

#include "AppManager.h"
#include "Image.h"

bool Steganography::Encode(std::string message)
{
	GetImageData();

	Reset();
	m_message = message;
	ConvertMessageToBinary();

	if (!HandleErrors()) return false;
	return true;
}

void Steganography::Reset()
{
	m_messageIndex = 0;
	m_message.clear();
	m_binaryMessage.clear();
}

void Steganography::ConvertMessageToBinary()
{
	for (char c : m_message) {
		int ascii = static_cast<int>(c); // conversion en base 10 (ascii)
		std::bitset<BIT_LENGTH> binary(ascii); // conversion en binaire (8bits)
		m_binaryMessage += binary.to_string();
	}

	// Ajout d'un character nul pour le décodage
	std::bitset<BIT_LENGTH> nullChar(0);
	m_binaryMessage += nullChar.to_string();
}

bool Steganography::EncodeIsOver(size_t i)
{
	return i < m_imageBytes.size() && m_messageIndex < m_binaryMessage.size();
}

bool Steganography::HandleErrors()
{
	if (m_message.empty())
	{
		std::cerr << "Error: No message provided." << std::endl;
		return false;
	}
	else if (m_binaryMessage.size() > m_imageBytes.size())
	{
		std::cerr << "Error: Message too long for the image size." << std::endl;
		return false;
	}

	return true;
}

void Steganography::GetImageData()
{
	try {
		Image* image = AppManager::GetInstance().GetImage();
		if (image) {
			m_imageBytes = image->GetPixelData();
		}
		else {
			throw std::runtime_error("Failed to get image data from AppManager");
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error loading image: " << e.what() << std::endl;
		throw;
	}
}