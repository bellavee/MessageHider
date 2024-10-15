#include "Button.h"

Button::Button(const wchar_t* name, COLORREF color, int x, int y, int width, int height, HMENU id, HINSTANCE hInstance, HWND parent)
	: m_name(name), m_color(color), m_x(x), m_y(y), m_width(width), m_height(height), m_id(id), m_hInstance(hInstance), m_parent(parent), hWnd(nullptr)
{
}

Button::~Button()
{
}

void Button::Create()
{
	hWnd = CreateWindow(
        L"BUTTON",                                          // nom de la classe de la fenêtre
        m_name,                                             // texte du bouton
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // styles
        m_x, m_y,                                           // position
        m_width, m_height,                                  // dimensions
        m_parent,                                           // fenêtre parente
        m_id,                                               // identifiant du bouton
        m_hInstance,                                        // instance de l'application
        nullptr                                             // paramètre additionnel
	);

    // Color
    HDC hdc = GetDC(hWnd);
    SetBkColor(hdc, m_color);
    ReleaseDC(hWnd, hdc);
}

void Button::OnClick()
{
    //On click...
}

HWND Button::GetHandle() const
{
	return HWND();
}
