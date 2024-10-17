#include "AppManager.h"
#include "PngImage.h"
#include "JpegImage.h"
#include "Button.h"

std::unique_ptr<AppManager> AppManager::m_instance = nullptr;

AppManager& AppManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = std::unique_ptr<AppManager>(new AppManager());
    }

    return *m_instance;
}

std::string AppManager::GetUserInput()
{
    m_userInput.clear();
    int length = GetWindowTextLength(m_inputField);

    if (length > 0) {
        // Créez un buffer pour stocker le texte
        wchar_t* buffer = new wchar_t[length + 1]; // +1 pour le caractère null
        GetWindowText(m_inputField, buffer, length + 1);

        std::wstring wstr(buffer);
        m_userInput.assign(wstr.begin(), wstr.end());

        delete[] buffer;
    }

    return m_userInput;
}

void AppManager::CreateElements(HWND hwnd, HINSTANCE instance)
{
    m_wHWND = hwnd;
    m_wInstance = instance;

    CreateInputField();
    CreateDropdown();
}

void AppManager::CreateInputField()
{
    m_inputField = CreateWindowEx
    (
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"Enter your secret message...",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN,
        (((WINDOW_WIDTH - 480) / 2) - ANCHOR_SPACING),
        540,
        480,
        150,
        m_wHWND,
        nullptr,
        m_wInstance,
        nullptr
    );
}

void AppManager::CreateDropdown()
{
    m_dropdown = CreateWindow
    (
        L"COMBOBOX",                                    // Classe de la fenêtre : une liste déroulante
        L"No filter",                                   // Texte affiché par défaut dans la combo box
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,       // Styles : enfant, visible et liste déroulante
        (((WINDOW_WIDTH - 450) / 2) - ANCHOR_SPACING),  // Position X 
        (WINDOW_HEIGHT / 2),                            // Position Y
        225,                                            // Largeur
        100,                                            // Hauteur
        m_wHWND,                                        // Handle de la fenêtre parent
        NULL,                                           // Identifiant de la combo box (NULL pour que le système en attribue un)
        NULL,                                           // Instance de l'application (NULL pour utiliser l'instance par défaut)
        NULL                                            // Paramètre additionnel
    );

    if (m_dropdown == NULL)
    {
        MessageBox(NULL, L"Échec de la création de la combo box!", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    SendMessage(m_dropdown, CB_ADDSTRING, 0, (LPARAM)L"Filtre 1");
    SendMessage(m_dropdown, CB_ADDSTRING, 0, (LPARAM)L"Filtre 2");
    SendMessage(m_dropdown, CB_ADDSTRING, 0, (LPARAM)L"Filtre 3");
}

void AppManager::HandleNewPage()
{
    for (Button* button : m_buttons)
    {
        switch (button->GetPage())
        {
        case Page::Encode:
        case Page::Decode:
            if (button->GetPage() == m_currentPage) button->Create();
            else button->Destroy();
            break;
        }
    }

    switch (m_currentPage)
    {
    case Page::Encode:
        CreateInputField();
        CreateDropdown();
        break;
    case Page::Decode:
        DestroyWindow(m_inputField);
        DestroyWindow(m_dropdown);
        break;
    }

    //m_imageLoaded = false;
    // TODO -> remove loaded image
}

AppManager::AppManager() : m_imageLoaded(false)
{
    m_darkTheme = true;
    m_currentPage = Page::Encode;
}

void AppManager::LoadImage(const std::string& filename)
{
    size_t dotPos = filename.find_last_of('.');
    std::string extension;
    if (dotPos != std::string::npos) {
        extension = filename.substr(dotPos);
    }

    // Convert to lowercase for case-insensitive comparison
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == ".png") {
        m_image = std::make_unique<PngImage>();
    }
    else if (extension == ".jpg" || extension == ".jpeg") {
        m_image = std::make_unique<JpegImage>();
    }
    else {
        throw std::runtime_error("Unsupported image format");
    }

    try {
        m_image->LoadFromFile(filename);
        m_imageLoaded = true;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error loading image: " + std::string(e.what()));
    }
}