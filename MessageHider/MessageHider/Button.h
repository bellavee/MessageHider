#pragma once

#include <Windows.h>
#include "AppManager.h"
#include "LSB.h"
#include <commdlg.h>  // Pour GetOpenFileName

enum class ButtonType
{
    EncodePage,
    DecodePage,
    Load,
    Download,
    Theme,
    EncodeAction,
    DecodeAction
};

class Button
{
public:
    Button(
        ButtonType type,
        COLORREF color,          // la couleur du bouton (définie par un code COLORREF)
        int x, int y,            // la position du coin supérieur gauche du bouton dans la fenêtre parent
        int width, int height,   // les dimensions du bouton
        HINSTANCE hInstance,     // handle de l'instance de l'application
        HWND parent              // handle de la fenêtre parente qui contiendra le bouton
    );

    virtual ~Button();

    void Create();
    void Destroy();
    virtual void OnClick();

    HMENU GetId() const { return m_id; }
    Page GetPage() const { return m_pageToDisplay; }

private:

    HWND m_hWnd;

    const wchar_t* m_name;
    COLORREF m_color;
    int m_x, m_y, m_width, m_height;
    HMENU m_id;
    HINSTANCE m_hInstance;
    HWND m_parent; // parent
    ButtonType m_type;
    Page m_pageToDisplay;

    std::unique_ptr<LSB> m_lsb;
};
