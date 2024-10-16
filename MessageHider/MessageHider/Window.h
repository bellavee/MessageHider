#pragma once

#include <windows.h>
#include <CommCtrl.h>
#include <vector>
#include "resource.h"
#include "Button.h"
#include "PngImage.h"

constexpr WCHAR WINDOW_TITLE[] = L"MESSAGE HIDER";
constexpr auto MAX_LOADSTRING = 100;
constexpr COLORREF WHITE = RGB(255, 255, 255);
constexpr COLORREF BLACK = RGB(28, 28, 46);
constexpr COLORREF LIGHT_BLUE = RGB(169, 200, 232);
constexpr COLORREF DARK_GREY = RGB(44, 44, 44);  
constexpr COLORREF LIGHT_GREY = RGB(84, 84, 84);
constexpr COLORREF RED = RGB(237, 54, 91);
constexpr COLORREF GREEN = RGB(76, 175, 80);
constexpr COLORREF BLUE = RGB(0, 140, 186);

class Window
{
public:
    Window(HINSTANCE hInstance, int nCmdShow);
    ~Window();

    bool Display();
    void ShowMessageLoop() const;

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    HFONT m_hTitleFont;
    HFONT m_hNormalFont;

    WCHAR m_szTitle[MAX_LOADSTRING];
    WCHAR m_szWindowClass[MAX_LOADSTRING];

    ATOM MyRegisterClass() const;
    RECT GetCenteredWindow() const;
    BOOL InitInstance(int nCmdShow);

    static void BackgroundColor(HDC hdc, PAINTSTRUCT ps, COLORREF color);
    void DrawTitle(HDC hdc);
    void DrawImage(HDC hdc);
    void DrawMessageCapacityText(HDC hdc);
    void DrawLoadError(HDC hdc);

    void CreateButtons();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    static ULONG_PTR m_gdiplusToken;
    static void InitializeGdiPlus();
    static void ShutdownGdiPlus();
};