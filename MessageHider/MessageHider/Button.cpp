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
    m_hWnd(nullptr),
    m_lsb(std::make_unique<LSB>())
{
    m_pageToDisplay = Page::All;

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
        m_pageToDisplay = Page::Encode;
        break;
    case ButtonType::Help:
        m_name = L"?";
        m_id = (HMENU)8;
        break;
    case ButtonType::Theme:
        m_name = L"T";
        m_id = (HMENU)5;
        break;
    case ButtonType::EncodeAction:
        m_name = L"Hide the message and export image";
        m_id = (HMENU)6;
        m_pageToDisplay = Page::Encode;
        break;
    case ButtonType::DecodeAction:
        m_name = L"Extract the message";
        m_id = (HMENU)7;
        m_pageToDisplay = Page::Decode;
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
        if (manager.GetCurrentPage() == Page::Encode) return;
        manager.SetCurrentPage(Page::Encode);
        manager.HandleNewPage();
        break;
    case ButtonType::DecodePage:
        if (manager.GetCurrentPage() == Page::Decode) return;
        manager.SetCurrentPage(Page::Decode);
        manager.HandleNewPage();
        break;
    case ButtonType::Load: 
        manager.Load();
    break;
    // case ButtonType::Download:
    //     break;
    case ButtonType::Help:
        manager.HelpPopup();
        break;
    case ButtonType::Theme:
        manager.ChangeTheme();
        break;
    case ButtonType::EncodeAction:
    {
        std::string input = manager.GetUserInput();

        if (input.empty() || !manager.HasImageLoaded()) break;

        if (m_lsb->Encode(input)) {
            MessageBoxA(NULL, "Message encoded successfully!.", "Success", MB_OK | MB_ICONINFORMATION);
            if (!manager.HasImageLoaded()) {
                MessageBoxA(NULL, "No image loaded or no message encoded.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            OPENFILENAMEA ofn;
            char szFile[260] = { 0 };
            char currentDir[260] = { 0 };

            GetCurrentDirectoryA(260, currentDir);

            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = m_parent;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "BMP Files\0*.bmp\0All Files\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = currentDir;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            ofn.lpstrDefExt = "bmp";

            if (GetSaveFileNameA(&ofn) == TRUE)
            {
                try {
                    m_lsb->SaveAsBmp(ofn.lpstrFile);
                    MessageBoxA(NULL, "Image saved successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                }
                catch (const std::exception& e) {
                    std::string errorMsg = "Error saving image: ";
                    errorMsg += e.what();
                    MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
                }
            }
        }
        else {
            MessageBoxA(NULL, "Failed to encode the message", "Error", MB_OK | MB_ICONERROR);
        }

        // Clear the input field
        input.clear();
        manager.SetUserInput(input);
    }
    break;
    case ButtonType::DecodeAction:
    {
        if (!manager.HasImageLoaded()) break;
        std::string result = m_lsb->Decode();
        manager.SetDecodedMessage(result);
        InvalidateRect(m_parent, NULL, TRUE);
    }
    break;
    }
}