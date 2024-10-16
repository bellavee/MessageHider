#include <algorithm>

#include "window.h"
#include "Button.h"
#include "JpegImage.h"
ULONG_PTR Window::m_gdiplusToken = 0;


std::vector<Button*> Window::m_buttons;

Window::Window(HINSTANCE hInst, int nCmdShow) : m_hInstance(hInst)
{
    wcscpy_s(m_szTitle, WINDOW_TITLE);
    LoadStringW(hInst, IDC_MESSAGEHIDER, m_szWindowClass, MAX_LOADSTRING);

    if (!MyRegisterClass() || !InitInstance(nCmdShow)) 
    {
        m_hWnd = nullptr;
        MessageBoxA(nullptr, "Failed to create window", "Error", MB_ICONERROR | MB_OK);
    }
}

Window::~Window()
{
    if (m_hTitleFont) DeleteObject(m_hTitleFont);
}

bool Window::Display()
{
    CreateComboBox();
    CreateSlider();
    CreateInputField();
    CreateButtons();

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return true;
}

void Window::ShowMessageLoop() const
{
    MSG msg;
    HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_MESSAGEHIDER));

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void Window::CreateButtons()
{
    int btmBtnW = WINDOW_WIDTH / 2;
    int btnHeight = 40;
    int btmBtnPosY = WINDOW_HEIGHT - (btnHeight * 2);

    m_buttons.push_back(new Button(ButtonType::EncodePage, LIGHT_GREY, 0, btmBtnPosY, btmBtnW, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::DecodePage, DARK_GREY, btmBtnW, btmBtnPosY, btmBtnW, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Theme, RED, (465 + anchorSpacing), 25, 35, 35, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Load, RED, (((WINDOW_WIDTH - 300) / 2) - anchorSpacing), 100, 300, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Download, BLUE, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 750, 480, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::EncodeAction, GREEN, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 700, 480, btnHeight, m_hInstance, m_hWnd));

    for (Button* button : m_buttons) button->Create();
}

void Window::CreateInputField() const
{
    AppManager& manager = AppManager::GetInstance();

    manager.SetInputField(CreateWindowEx
    (
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"Enter your secret message...",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN,
        (((WINDOW_WIDTH - 480) / 2) - anchorSpacing),
        540,
        480,
        150,
        m_hWnd,
        nullptr,
        m_hInstance,
        nullptr
    ));
}

ATOM Window::MyRegisterClass() const
{
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = m_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    return RegisterClassExW(&wcex);
}

RECT Window::GetCenteredWindow() const
{
    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    int xPos = (rect.right - WINDOW_WIDTH) / 2;
    int yPos = (rect.bottom - WINDOW_HEIGHT) / 2;

    RECT centeredWindow = { xPos, yPos, xPos + WINDOW_WIDTH, yPos + WINDOW_HEIGHT };
    return centeredWindow;
}

BOOL Window::InitInstance(int nCmdShow)
{
    RECT CenteredWindow = GetCenteredWindow();

    m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CenteredWindow.left, CenteredWindow.top, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, m_hInstance, this);

    if (!m_hWnd) return FALSE;

    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);

    return TRUE;
}

void Window::BackgroundColor(HDC hdc, PAINTSTRUCT ps, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    FillRect(hdc, &ps.rcPaint, hBrush);
    DeleteObject(hBrush);
}

void Window::DrawTitle(HDC hdc)
{
    if (!m_hTitleFont)
    {
        m_hTitleFont = CreateFont
        (
            50,                         // Hauteur de la police
            0,                          // Largeur de la police
            0,                          // Angle de l'orientation de la police
            0,                          // Angle d'orientation du texte
            FW_BOLD,                    // Gras
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

    SelectObject(hdc, m_hTitleFont);
    SetTextColor(hdc, WHITE);
    SetBkMode(hdc, TRANSPARENT);

    const WCHAR* title = L"MESSAGE HIDER";
    TextOut(hdc, 20, 20, title, wcslen(title));
}

void Window::DrawLoadError(HDC hdc) const
{
    SelectObject(hdc, m_hNormalFont);
    SetTextColor(hdc, WHITE);
    SetBkMode(hdc, TRANSPARENT);

    const WCHAR* loadErrorMessage = L"No image loaded";
    TextOut(hdc, ((WINDOW_WIDTH / 2 - 60)), (WINDOW_HEIGHT / 3), loadErrorMessage, wcslen(loadErrorMessage));
}

void Window::DrawMessageCapacityText(HDC hdc)
{
    if (!m_hNormalFont)
    {
        m_hNormalFont = CreateFont
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

    SelectObject(hdc, m_hNormalFont);
    SetTextColor(hdc, WHITE);
    SetBkMode(hdc, TRANSPARENT);

    const WCHAR* messageCapacity = L"Maximum message capacity : ";
    TextOut(hdc,(WINDOW_WIDTH /2), 455, messageCapacity, wcslen(messageCapacity));
}

void Window::DrawFilterIntensityText(HDC hdc) const
{
    SelectObject(hdc, m_hNormalFont);
    SetTextColor(hdc, WHITE);
    SetBkMode(hdc, TRANSPARENT);

    const WCHAR* filterIntensityText = L"Filter intensity";
    TextOut(hdc, ((WINDOW_WIDTH / 2 - 48)), ((WINDOW_HEIGHT / 2) + 30), filterIntensityText, wcslen(filterIntensityText));
}

void Window::CreateComboBox() const
{
    HWND hComboBox = CreateWindow
    (
        L"COMBOBOX",                                    // Classe de la fenêtre : une liste déroulante
        L"No filter",                                   // Texte affiché par défaut dans la combo box
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,       // Styles : enfant, visible et liste déroulante
        (((WINDOW_WIDTH - 450) / 2) - anchorSpacing),   // Position X 
        450,                                            // Position Y
        225,                                            // Largeur
        100,                                            // Hauteur
        m_hWnd,                                         // Handle de la fenêtre parent
        nullptr,                                           // Identifiant de la combo box (NULL pour que le système en attribue un)
        nullptr,                                           // Instance de l'application (NULL pour utiliser l'instance par défaut)
        nullptr                                            // Paramètre additionnel
    );

    if (hComboBox == nullptr)
    {
        MessageBox(nullptr, L"Échec de la création de la combo box!", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Filtre 1");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Filtre 2");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Filtre 3");
}

void Window::CreateSlider()
{
    m_hSlider = CreateWindowEx
    (
        0,                                      
        TRACKBAR_CLASS,                                                 // Classe
        L"",                                                            // Texte de la fenêtre
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,     // Style de la fenêtre
        (((WINDOW_WIDTH - 450) / 2) - anchorSpacing),                   // Position X
        500,                                                            // Position Y 
        450,                                                            // Largeur
        20,                                                             // Hauteur
        m_hWnd,                                                         // Handle de la fenêtre parent
        nullptr,                                                        // Identifiant du slider 
        nullptr,                                                        // Instance de l'application
        nullptr                                                         // Paramètre additionnel
    );

    // Définir la plage du slider
    SendMessage(m_hSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));     // Plage de 0 à 100
    SendMessage(m_hSlider, TBM_SETPOS, TRUE, 50);                       // Position initiale à 50
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Window* pThis = nullptr;
    if (message == WM_NCCREATE)
    {
        pThis = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    AppManager& manager = AppManager::GetInstance();

    switch (message) 
    {
    case WM_CREATE:
        break;
    case WM_COMMAND:
    {
        for (Button* button : m_buttons)
        {
            if (button->GetId() == (HMENU)LOWORD(wParam))
            {
                button->OnClick();
                break;
            }
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (pThis)
        {
            pThis->BackgroundColor(hdc, ps, manager.HasDarkTheme() ? BLACK : WHITE);
            pThis->DrawTitle(hdc);
            pThis->DrawMessageCapacityText(hdc);
            pThis->DrawFilterIntensityText(hdc);

            if (manager.HasImageLoaded() && manager.GetImage())
            {
                manager.GetImage()->Render(hdc, 0, 0, WINDOW_WIDTH);
            }
            else
            {
                pThis->DrawLoadError(hdc);
            }
        }
        EndPaint(hWnd, &ps);
    }
    break;
    /*case WM_CTLCOLORBTN:
    {
        HDC hdcButton = (HDC)wParam;
        HWND hButton = (HWND)lParam;

        for (Button* button : m_buttons)
        {
            if (button->GetId() == (HMENU)GetDlgCtrlID(hButton))
            {
                SetBkColor(hdcButton, RED);
                SetTextColor(hdcButton, RGB(255, 255, 255));
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                return (INT_PTR)hBrush;
            }
        }
    }
    break;*/
    case WM_ERASEBKGND:
        return 1;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Window::InitializeGdiPlus()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

void Window::ShutdownGdiPlus()
{
    Gdiplus::GdiplusShutdown(m_gdiplusToken);
    //m_pngImage = std::make_unique<PngImage>();
    //try {
    //    m_pngImage->LoadFromFile("sample.png");
    //    m_imageLoaded = true;
    //    InvalidateRect(m_hWnd, NULL, TRUE);  // Force a redraw
    //}
    //catch (const std::exception& e) {
    //    MessageBoxA(NULL, e.what(), "Error loading PNG", MB_OK | MB_ICONERROR);
    //}
}