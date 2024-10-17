#include "Steganography.h"

bool Steganography::Encode(std::string message)
{
	if (!GetImageDatas()) return false;

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
	AppManager& manager = AppManager::GetInstance();

	if (m_message.empty())
	{
		manager.ShowErrorPopup(L"No message provided.");
		return false;
	}
	else if (m_binaryMessage.size() > m_imageBytes.size())
	{
		manager.ShowErrorPopup(L"Message too long for the image size.");
		return false;
	}

	return true;
}

bool Steganography::GetImageDatas()
{
	AppManager& manager = AppManager::GetInstance();

	if (!manager.HasImageLoaded())
	{
		manager.ShowErrorPopup(L"No image loaded.");
		return false;
	}

	m_imageBytes = manager.GetImage()->GetPixelData();

	/*std::ifstream imageFile("./eevee.png", std::ios::binary);
	if (!imageFile) {
		std::cerr << "Impossible d'ouvrir l'image" << std::endl;
		return;
	}

	m_imageBytes = std::vector<uint8_t>((std::istreambuf_iterator<char>(imageFile)), std::istreambuf_iterator<char>());
	imageFile.close();*/
}