#pragma once

#include <windows.h>
#include <CommCtrl.h>
#include "resource.h" // Inclure si on utilises des ressources (icônes, etc.)

constexpr auto MAX_LOADSTRING = 100;

class Window 
{
public:
    Window(HINSTANCE hInstance, int nCmdShow);
    ~Window();

    bool Create() const;
    void ShowMessageLoop() const;

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE hInstance;
    HWND hWnd;
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];

    void InitCommonControls();
    ATOM MyRegisterClass();
    BOOL InitInstance(int nCmdShow);
};