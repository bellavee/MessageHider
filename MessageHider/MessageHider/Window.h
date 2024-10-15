#pragma once

#include <windows.h>
#include <CommCtrl.h>
#include <vector>
#include "resource.h" // Inclure si on utilises des ressources (icônes, etc.)
#include "Button.h"

constexpr auto MAX_LOADSTRING = 100;
constexpr int WINDOW_WIDTH = 540;
constexpr int WINDOW_HEIGHT = 900;
constexpr COLORREF TEXT_COLOR = RGB(255, 255, 255);
constexpr COLORREF BACKGROUND_COLOR = RGB(30, 30, 30);
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

    HINSTANCE hInstance;

    HWND m_hWnd;

    WCHAR m_szTitle[MAX_LOADSTRING];
    WCHAR m_szWindowClass[MAX_LOADSTRING];

    //void InitCommonControls();
    void CreateButtons();
    ATOM MyRegisterClass() const;
    RECT GetCenteredWindow() const;
    BOOL InitInstance(int nCmdShow);
    static void BackgroundColor(HDC hdc, PAINTSTRUCT ps);
    void Draw(HDC hdc);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    static std::vector<Button*> m_buttons;
};