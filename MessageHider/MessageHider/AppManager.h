#pragma once

#include "Image.h"
#include "ImageFilter.h"

class Button;

constexpr int WINDOW_WIDTH = 540;
constexpr int WINDOW_HEIGHT = 900;
constexpr int ANCHOR_SPACING = 8;

enum class Page
{
    All,
    Encode,
    Decode
};

class AppManager
{
public:
    static AppManager& GetInstance();

    std::string GetUserInput();

    void CreateElements(HWND hwnd, HINSTANCE instance);
    void CreateInputField();
    void CreateDropdown();
    void HandleNewPage();

    // Getters
    Image* GetImage() { return m_image.get(); }
    bool HasImageLoaded() const { return m_imageLoaded; }
    bool HasDarkTheme() const { return m_darkTheme; }
    Page GetCurrentPage() const { return m_currentPage; }
    std::vector<Button*> GetButtons() { return m_buttons; }
    HWND GetDropdown() const { return m_dropdown; }

    // Setters
    void SetUserInput(std::string input) { m_userInput = input; }
    void SetImageLoaded(bool value) { m_imageLoaded = value; }
    void SetDarkTheme(bool value) { m_darkTheme = value; }
    void SetCurrentPage(Page newPage) { m_currentPage = newPage; }
    void SetButtons(std::vector<Button*> buttons) { m_buttons = buttons; }

    void HandleDropdownChange();
    void HandleSliderChange(int value);
    void ApplyCurrentFilter();

    void LoadImage(const std::string& filename);

private:
    static std::unique_ptr<AppManager> m_instance;

    std::unique_ptr<Image> m_image;
    bool m_imageLoaded;
    bool m_darkTheme;
    std::string m_userInput;
    Page m_currentPage;

    HWND m_wHWND;
    HINSTANCE m_wInstance;

    // Elements
    std::vector<Button*> m_buttons;
    HWND m_inputField;
    HWND m_dropdown;

    std::unique_ptr<ImageFilter> m_currentFilter;
    int m_currentFilterValue;
    int m_selectedFilterIndex;

    AppManager();
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};