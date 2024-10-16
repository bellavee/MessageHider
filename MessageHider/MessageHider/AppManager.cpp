#include "AppManager.h"

std::unique_ptr<AppManager> AppManager::m_instance = nullptr;

AppManager& AppManager::GetInstance()
{
    if (m_instance == nullptr) {
        m_instance = std::unique_ptr<AppManager>(new AppManager());
    }
    return *m_instance;
}

AppManager::AppManager()
{
    m_imageLoaded = false;
    m_darkTheme = true;
    m_pngImage = std::make_unique<PngImage>();
}