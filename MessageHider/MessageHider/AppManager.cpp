#include "AppManager.h"

#include "BmpImage.h"
#include "PngImage.h"
#include "JpegImage.h"
#include "Button.h"
#include "Resource.h"

std::unique_ptr<AppManager> AppManager::m_instance = nullptr;

AppManager& AppManager::GetInstance()
{
    if (m_instance == nullptr) m_instance = std::unique_ptr<AppManager>(new AppManager());
    return *m_instance;
}

std::string AppManager::GetUserInput()
{
    m_userInput.clear();
    int length = GetWindowTextLength(m_inputField);

    if (length > 0) {
        // Buffer pour stocker le texte
        wchar_t* buffer = new wchar_t[length + 1];
        GetWindowText(m_inputField, buffer, length + 1);

        std::wstring wstr(buffer);
        m_userInput.assign(wstr.begin(), wstr.end());

        delete[] buffer;
    }

    return m_userInput;
}

void AppManager::UpdateElement() const
{
    SendMessage(m_inputField, EM_SETLIMITTEXT, m_imageLoaded ? GetImage()->GetPixelData().size() : 100, 0);
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
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN | WS_TABSTOP,
        (((WINDOW_WIDTH - 480) / 2) - ANCHOR_SPACING),
        540,
        480,
        150,
        m_wHWND,
        NULL,
        m_wInstance,
        NULL
    );

    //SendMessage(m_inputField, EM_SETLIMITTEXT, m_imageLoaded ? GetImage()->GetPixelData().size() : 100, 0);

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
        (HMENU)IDC_FILTER_DROPDOWN,                     // Identifiant de la combo box
        m_wInstance,                                    // Instance de l'application
        NULL                                            // Paramètre additionnel
    );

    // Ajout des options de filtre dans la combo box
    SendMessage(m_dropdown, CB_ADDSTRING, 0, (LPARAM)L"No Filter");
    SendMessage(m_dropdown, CB_ADDSTRING, 0, (LPARAM)L"Brightness");
    SendMessage(m_dropdown, CB_ADDSTRING, 0, (LPARAM)L"Contrast");
    SendMessage(m_dropdown, CB_ADDSTRING, 0, (LPARAM)L"Saturation");

    // Sélectionner la première option par défaut
    SendMessage(m_dropdown, CB_SETCURSEL, 0, 0);

    // Slider
    m_slider = CreateWindowEx
    (
        0,
        TRACKBAR_CLASS,
        L"",
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
        (((WINDOW_WIDTH - 450) / 2) - ANCHOR_SPACING),
        500,
        450,
        30,
        m_wHWND,
        (HMENU)IDC_SLIDER,
        m_wInstance,
        nullptr
    );

    // Définir la plage du slider
    SendMessage(m_slider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));     // Plage de 0 à 100
    SendMessage(m_slider, TBM_SETPOS, TRUE, 50);    
}

void AppManager::CreateDecodeElements()
{
    // Input field
    m_readOnlyInputField = CreateWindowEx
    (
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L" ",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_DISABLED,
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

void AppManager::DrawEncodeElements() const
{
    SelectObject(m_wHDC, m_normalFont);
    SetBkMode(m_wHDC, TRANSPARENT);
    SetTextColor(m_wHDC, WHITE);

    // Slider text
    const WCHAR* sliderText = L"Filter intensity";
    TextOut(m_wHDC, ((WINDOW_WIDTH / 2 - 48)), ((WINDOW_HEIGHT / 2) + 30), sliderText, wcslen(sliderText));

    // Capcity label
    const WCHAR* capacityLabel = L"Message max size : ";
    TextOut(m_wHDC, (WINDOW_WIDTH / 2), ((WINDOW_HEIGHT / 2) + 3), capacityLabel, wcslen(capacityLabel));

    SetTextColor(m_wHDC, RED);

    // Capacity text
    std::wstring capacityText = L"---";

    if (m_imageLoaded)
    {
        size_t size = (GetImage()->GetPixelData().size() / 8) - 1;
        capacityText = (std::wstringstream() << size).str() + L" words";
    }

    TextOut(m_wHDC, ((WINDOW_WIDTH / 2) + 112), ((WINDOW_HEIGHT / 2) + 3), capacityText.c_str(), capacityText.length());
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
        DestroyWindow(m_readOnlyInputField);
        InvalidateRect(m_wHWND, NULL, TRUE);
        break;
    case Page::Decode:
        CreateDecodeElements();
        DestroyWindow(m_inputField);
        DestroyWindow(m_dropdown);
        DestroyWindow(m_slider);
        InvalidateRect(m_wHWND, NULL, TRUE);
        break;
    }

    m_imageLoaded = false;
}

void AppManager::ShowErrorPopup(const WCHAR* error)
{
    MessageBox(
        NULL,
        error,                  // Le message d'erreur � afficher
        L"ERROR",              // Le titre de la bo�te de dialogue
        MB_ICONERROR | MB_OK    // Ic�ne d'erreur et bouton "OK"
    );
}

void AppManager::HelpPopup()
{
    const WCHAR* helpMessage =
        L"===========================\n"
        L"      Keyboard Shortcuts      \n"
        L"===========================\n"
        L"CTRL + E  -> Encode tab\n"
        L"CTRL + D  -> Decode tab\n"
        L"CTRL + L  -> Load image\n"
        L"CTRL + T  -> Change theme\n"
        L"===========================\n"
        L"If you encounter errors,\n"
        L"check the displayed error messages.\n"
        L"===========================\n";

    MessageBox(
        NULL,
        helpMessage,
        L"Help",
        MB_ICONINFORMATION | MB_OK
    );
}

void AppManager::Load()
{
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_wHWND;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Images\0*.png;*.jpg;*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        // Convert wchar_t[] (file path) to std::string after file selection
        std::wstring wstr(szFile);
        std::string file(wstr.begin(), wstr.end());

        try {
            LoadImage(file);                    // Load the selected image
            InvalidateRect(m_wHWND, NULL, TRUE);       // Force window refresh
        }
        catch (const std::exception& e) {
            MessageBoxA(NULL, e.what(), "Error loading image", MB_OK | MB_ICONERROR);
        }
    }

}

void AppManager::ChangeTheme()
{
    SetDarkTheme(!HasDarkTheme());
    InvalidateRect(m_wHWND, NULL, TRUE);  // Forcer le rafra�chissement de la fen�tre
void AppManager::Loading(bool loading)
{
    HCURSOR hCursor = LoadCursor(NULL, loading ? IDC_WAIT : IDC_ARROW);
    SetCursor(hCursor);
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
    else if (extension == ".bmp") {
        m_image = std::make_unique<BmpImage>();
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

void AppManager::HandleDropdownChange()
{
    m_selectedFilterIndex = SendMessage(m_dropdown, CB_GETCURSEL, 0, 0);
	if (m_image && m_imageLoaded)
        m_image->ResetToOriginal();
    InvalidateRect(m_wHWND, NULL, TRUE);
}

void AppManager::HandleSliderChange(int value)
{
    m_currentFilterValue = value;
    ApplyCurrentFilter();
}

void AppManager::ApplyCurrentFilter()
{
    if (m_image && m_imageLoaded)
    {
        // Reset to original image before applying new filter
        m_image->ResetToOriginal();

        switch (m_selectedFilterIndex)
        {
        case 0: // No filter
            m_currentFilter = nullptr;
            break;
        case 1: // Brightness filter
            m_currentFilter = std::make_unique<BrightnessFilter>(m_currentFilterValue - 50);
            break;
        case 2: // Contrast filter
            m_currentFilter = std::make_unique<ContrastFilter>((m_currentFilterValue / 50.0f) + 0.5f);
            break;
        case 3: // Saturation filter
            m_currentFilter = std::make_unique<SaturationFilter>((m_currentFilterValue / 50.0f) + 0.5f);
            break;
        }

        if (m_currentFilter)
        {
            m_currentFilter->Apply(*m_image);
        }

        // Trigger a repaint of the window
        InvalidateRect(m_wHWND, NULL, TRUE);
    }
}