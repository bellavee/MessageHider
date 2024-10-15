#pragma once

#include <windows.h>
#include <CommCtrl.h>
#include "resource.h" // Inclure si on utilises des ressources (icônes, etc.)

constexpr auto MAX_LOADSTRING = 100;
constexpr int WINDOW_WIDTH = 540;
constexpr int WINDOW_HEIGHT = 900;
constexpr COLORREF BACKGROUND_COLOR = RGB(30, 30, 30);
constexpr COLORREF DARK_RECT_COLOR = RGB(44, 44, 44);  
constexpr COLORREF LIGHT_RECT_COLOR = RGB(84, 84, 84);
constexpr COLORREF RED_BUTTON_COLOR = RGB(237, 54, 91);
constexpr COLORREF GREEN_BUTTON_COLOR = RGB(76, 175, 80);
constexpr COLORREF BLUE_BUTTON_COLOR = RGB(0, 140, 186);

class Window 
{
public:
    Window(HINSTANCE hInstance, int nCmdShow);
    ~Window();

    bool Create() const;
    void ShowMessageLoop() const;

private:

    HINSTANCE hInstance;

    HWND m_hWnd;
    HWND m_hTab;
    HWND m_hPageEncode;
    HWND m_hPageDecode;

    WCHAR m_szTitle[MAX_LOADSTRING];
    WCHAR m_szWindowClass[MAX_LOADSTRING];

    void InitCommonControls();
    ATOM MyRegisterClass();
    RECT GetCenteredWindow() const;
    BOOL InitInstance(int nCmdShow);
    static void BackgroundColor(HDC hdc, PAINTSTRUCT ps);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

};