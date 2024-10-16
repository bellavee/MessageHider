#pragma once

#include <memory>
#include <string>

#include "PngImage.h"

class AppManager
{
public:
    static AppManager& GetInstance();

    PngImage* GetPngImage() { return m_pngImage.get(); }
    bool HasImageLoaded() const { return m_imageLoaded; }
    std::string GetUserInput() const { return m_userInput; }

    void SetUserInput(std::string input) { m_userInput = input; }
    void SetImageLoaded(bool value) { m_imageLoaded = value; }

private:
    static std::unique_ptr<AppManager> m_instance;

    std::unique_ptr<PngImage> m_pngImage;
    bool m_imageLoaded;
    std::string m_userInput;

    AppManager();
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};