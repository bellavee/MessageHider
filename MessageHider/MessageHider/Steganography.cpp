#include "Steganography.h"

bool Steganography::Encode(std::string message)
{
	GetImageDatas(); // temp

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

void Steganography::GetImageDatas()
{
	AppManager& manager = AppManager::GetInstance();
	m_imageBytes = manager.GetImage()->GetPixelData();
}

void Steganography::SaveAsBmp(const std::string& filename) {
    AppManager& manager = AppManager::GetInstance();
    if (m_imageBytes.empty()) {
        throw std::runtime_error("No image data available");
    }
    int width = manager.GetImage()->GetWidth();
    int height = manager.GetImage()->GetHeight();

    // BMP file header
    BITMAPFILEHEADER bfh = { 0 };
    bfh.bfType = 0x4D42; // "BM"
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 4;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // BMP info header
    BITMAPINFOHEADER bih = { 0 };
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = -height; // Negative for top-down DIB
    bih.biPlanes = 1;
    bih.biBitCount = 32; // 32 bits per pixel for BGRA
    bih.biCompression = BI_RGB;
    bih.biSizeImage = width * height * 4;

    // Open file for writing
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create BMP file: " + filename);
    }

    // Write headers
    file.write(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));

    // Write pixel data directly
    file.write(reinterpret_cast<const char*>(m_imageBytes.data()), m_imageBytes.size());

    if (file.fail()) {
        throw std::runtime_error("Failed to write BMP data");
    }

    file.close();
    if (file.fail()) {
        throw std::runtime_error("Failed to close BMP file");
    }
}
