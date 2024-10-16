#pragma once

#include "Image.h"

class AppManager
{
public:
    static AppManager& GetInstance();

    Image* GetImage() { return m_image.get(); }
    bool HasImageLoaded() const { return m_imageLoaded; }
    bool HasDarkTheme() const { return m_darkTheme; }
    std::string GetUserInput() const { return m_userInput; }

    void SetUserInput(std::string input) { m_userInput = input; }
    void SetImageLoaded(bool value) { m_imageLoaded = value; }
    void SetDarkTheme(bool value) { m_darkTheme = value; }

    void LoadImage(const std::string& filename);

private:
    static std::unique_ptr<AppManager> m_instance;

    std::unique_ptr<Image> m_image;
    bool m_imageLoaded;
    bool m_darkTheme;
    std::string m_userInput;

    AppManager();
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};