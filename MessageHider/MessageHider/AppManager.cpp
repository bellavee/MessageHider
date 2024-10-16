#include "AppManager.h"
#include "PngImage.h"
#include "JpegImage.h"

std::unique_ptr<AppManager> AppManager::m_instance = nullptr;

AppManager& AppManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = std::unique_ptr<AppManager>(new AppManager());
    }

    return *m_instance;
}

std::string AppManager::GetUserInput()
{
    m_userInput.clear();
    int length = GetWindowTextLength(m_inputField);

    if (length > 0) {
        // Créez un buffer pour stocker le texte
        wchar_t* buffer = new wchar_t[length + 1]; // +1 pour le caractère null
        GetWindowText(m_inputField, buffer, length + 1);

        std::wstring wstr(buffer);
        m_userInput.assign(wstr.begin(), wstr.end());

        delete[] buffer;
    }

    return m_userInput;
}

AppManager::AppManager() : m_imageLoaded(false)
{
    m_darkTheme = true;
    //m_pngImage = std::make_unique<PngImage>();
}

void AppManager::LoadImage(const std::string& filename)
{
    size_t dotPos = filename.find_last_of('.');
    std::string extension;
    if (dotPos != std::string::npos) {
        extension = filename.substr(dotPos);
    }

    // Convert to lowercase for case-insensitive comparison
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == ".png") {
        m_image = std::make_unique<PngImage>();
    }
    else if (extension == ".jpg" || extension == ".jpeg") {
        m_image = std::make_unique<JpegImage>();
    }
    else {
        throw std::runtime_error("Unsupported image format");
    }

    try {
        m_image->LoadFromFile(filename);
        m_imageLoaded = true;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error loading image: " + std::string(e.what()));
    }
}