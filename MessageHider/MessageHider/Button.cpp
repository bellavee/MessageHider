#include "Button.h"

Button::Button(
    ButtonType type,
    COLORREF color,
    int x, int y,
    int width, int height,
    HINSTANCE hInstance,
    HWND parent)
    :
    m_type(type),
    m_color(color),
    m_x(x), m_y(y),
    m_width(width), m_height(height),
    m_hInstance(hInstance),
    m_parent(parent),
    hWnd(nullptr)
{
    switch (type)
    {
    case ButtonType::Encode:
        m_name = L"ENCODE";
        m_id = (HMENU)"ENCODE";
        break;
    case ButtonType::Decode:
        m_name = L"DECODE";
        m_id = (HMENU)"DECODE";
        break;
    case ButtonType::Load:
        m_name = L"Load an image";
        m_id = (HMENU)"LOAD";
        break;
    case ButtonType::Download:
        m_name = L"Download the new image";
        m_id = (HMENU)"DOWNLOAD";
        break;
    case ButtonType::Theme:
        m_name = L"T";
        m_id = (HMENU)"THEME";
        break;
    case ButtonType::OK:
        m_name = L"OK";
        m_id = (HMENU)"OK";
        break;
    }
}

Button::~Button() { }

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
    switch (m_type)
    {
    case ButtonType::Encode:
        break;
    case ButtonType::Decode:
        break;
    case ButtonType::Load:
        break;
    case ButtonType::Download:
        break;
    case ButtonType::Theme:
        break;
    case ButtonType::OK:
        break;
    default:
        break;
    }
}

HWND Button::GetHandle() const
{
    return hWnd;
}