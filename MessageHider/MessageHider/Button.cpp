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
    m_hWnd(nullptr)
{
    switch (type)
    {
    case ButtonType::EncodePage:
        m_name = L"ENCODE";
        m_id = (HMENU)1;
        break;
    case ButtonType::DecodePage:
        m_name = L"DECODE";
        m_id = (HMENU)2;
        break;
    case ButtonType::Load:
        m_name = L"Load an image";
        m_id = (HMENU)3;
        break;
    case ButtonType::Download:
        m_name = L"Download the new image";
        m_id = (HMENU)4;
        break;
    case ButtonType::Theme:
        m_name = L"T";
        m_id = (HMENU)5;
        break;
    case ButtonType::EncodeAction:
        m_name = L"Hide the message";
        m_id = (HMENU)6;
        break;
    case ButtonType::DecodeAction:
        m_name = L"Extract the message";
        m_id = (HMENU)7;
        break;
    }
}

Button::~Button() { }

void Button::Create()
{
	m_hWnd = CreateWindow(
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
}

void Button::OnClick()
{
    AppManager& manager = AppManager::GetInstance();

    switch (m_type)
    {
    case ButtonType::EncodePage:
        break;
    case ButtonType::DecodePage:
        break;
    case ButtonType::Load:
        try {
            manager.LoadImage("eevee.png");
            InvalidateRect(m_parent, NULL, TRUE); 
        }
        catch (const std::exception& e) {
            MessageBoxA(NULL, e.what(), "Error loading image", MB_OK | MB_ICONERROR);
        }
        break;
    case ButtonType::Download:
        break;
    case ButtonType::Theme:
        manager.SetDarkTheme(!manager.HasDarkTheme());
        InvalidateRect(m_parent, NULL, TRUE);  // Force a redraw
        break;
    case ButtonType::EncodeAction:
    {
        std::string input = manager.GetUserInput();
        if (input.empty() || !manager.HasImageLoaded()) break;
        LSB lsb;
        lsb.Encode(input);

        input = input.empty();
        manager.SetUserInput(input);
    }
        break;
    case ButtonType::DecodeAction:
    {
        LSB lsb;
        std::string result = lsb.Decode();
    }
        break;
    }
}