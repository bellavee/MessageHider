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
    m_wHDC = GetDC(hwnd);

    if (!m_normalFont)
    {
        m_normalFont = CreateFont
        (
            15,                         // Hauteur de la police
            0,                          // Largeur de la police
            0,                          // Angle de l'orientation de la police
            0,                          // Angle d'orientation du texte
            FALSE,                      // Gras
            FALSE,                      // Italique
            FALSE,                      // Souligné
            FALSE,                      // Barré
            DEFAULT_CHARSET,            // Jeu de caractères par défaut
            OUT_DEFAULT_PRECIS,         // Précision de sortie par défaut
            CLIP_DEFAULT_PRECIS,        // Précision de découpe par défaut
            DEFAULT_QUALITY,            // Qualité de rendu par défaut
            0,                          // Méthode d'orientation (0 pour utiliser la méthode par défaut)
            L"Arial"                    // Nom de la police
        );
    }

    CreateEncodeElements();
}

void AppManager::CreateEncodeElements()
{
    // Input field
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

    // Dropdown
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

    // Slider
    m_slider = CreateWindowEx
    (
        0,
        TRACKBAR_CLASS,                                                 // Classe
        L"",                                                            // Texte de la fenêtre
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,     // Style de la fenêtre
        (((WINDOW_WIDTH - 450) / 2) - ANCHOR_SPACING),                  // Position X
        500,                                                            // Position Y 
        450,                                                            // Largeur
        30,                                                             // Hauteur
        m_wHWND,                                                         // Handle de la fenêtre parent
        nullptr,                                                        // Identifiant du slider 
        nullptr,                                                        // Instance de l'application
        nullptr                                                         // Paramètre additionnel
    );

    // Définir la plage du slider
    SendMessage(m_slider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));     // Plage de 0 à 100
    SendMessage(m_slider, TBM_SETPOS, TRUE, 50);
}

void AppManager::DrawEncodeElements()
{
    SelectObject(m_wHDC, m_normalFont);
    SetBkMode(m_wHDC, TRANSPARENT);
    SetTextColor(m_wHDC, WHITE);

    // Slider text
    const WCHAR* sliderText = L"Filter intensity";
    TextOut(m_wHDC, ((WINDOW_WIDTH / 2 - 48)), ((WINDOW_HEIGHT / 2) + 30), sliderText, wcslen(sliderText));

    // Capcity label
    const WCHAR* capacityLabel = L"Maximum message capacity : ";
    TextOut(m_wHDC, (WINDOW_WIDTH / 2), ((WINDOW_HEIGHT / 2) + 3), capacityLabel, wcslen(capacityLabel));

    SetTextColor(m_wHDC, RED);

    // Capacity text
    const WCHAR* capacityText = m_imageLoaded ? L"(size)" : L"---";
    TextOut(m_wHDC, ((WINDOW_WIDTH / 2) + 165), ((WINDOW_HEIGHT / 2) + 3), capacityText, wcslen(capacityText));
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
        CreateEncodeElements();
        break;
    case Page::Decode:
        DestroyWindow(m_inputField);
        DestroyWindow(m_dropdown);
        DestroyWindow(m_slider);
        InvalidateRect(m_wHWND, NULL, TRUE);
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