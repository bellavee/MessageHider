#pragma once

#include <windows.h>
#include <CommCtrl.h>
#include "Image.h"
#include "ImageFilter.h"

class Button;

constexpr int WINDOW_WIDTH = 540;
constexpr int WINDOW_HEIGHT = 900;
constexpr int ANCHOR_SPACING = 8;
constexpr COLORREF WHITE = RGB(255, 255, 255);
constexpr COLORREF RED = RGB(237, 54, 91);

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

    void UpdateElement() const;

    void CreateElements(HWND hwnd, HINSTANCE instance);
    void CreateEncodeElements();
    void CreateDecodeElements();
    void DrawEncodeElements() const;
    void HandleNewPage();
    void ShowErrorPopup(const WCHAR* error);
    void Loading(bool loading);

    // Getters
    Image* GetImage() const { return m_image.get(); }
    bool HasImageLoaded() const { return m_imageLoaded; }
    bool HasDarkTheme() const { return m_darkTheme; }
    std::string GetDecodedMessage() const { return m_decodedMessage; }
    Page GetCurrentPage() const { return m_currentPage; }
    std::vector<Button*> GetButtons() { return m_buttons; }
    HWND GetDropdown() const { return m_dropdown; }
    HWND GetSlider() const { return m_slider; }

    // Setters
    void SetUserInput(std::string input) { m_userInput = input; }
    void SetImageLoaded(bool value) { m_imageLoaded = value; }
    void SetDarkTheme(bool value) { m_darkTheme = value; }
    void SetDecodedMessage(std::string message) { m_decodedMessage = message; }
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
    std::string m_decodedMessage;
    Page m_currentPage;

    HWND m_wHWND;
    HINSTANCE m_wInstance;
    HDC m_wHDC;

    // Elements
    HFONT m_normalFont;
    std::vector<Button*> m_buttons;
    HWND m_inputField;
    HWND m_dropdown;
    HWND m_slider;
    HWND m_readOnlyInputField;

    std::unique_ptr<ImageFilter> m_currentFilter;
    int m_currentFilterValue;
    int m_selectedFilterIndex;

    AppManager();
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};