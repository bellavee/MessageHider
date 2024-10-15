#include "window.h"
#include "Button.h"

Window::Window(HINSTANCE hInst, int nCmdShow)
    : m_hInstance(hInst), m_hTitleFont(nullptr)
{

    wcscpy_s(m_szTitle, WINDOW_TITLE); // Utilisez le titre fixe

    //LoadStringW(hInst, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInst, IDC_MESSAGEHIDER, m_szWindowClass, MAX_LOADSTRING);

    if (!MyRegisterClass() || !InitInstance(nCmdShow)) m_hWnd = nullptr;
}

Window::~Window()
{
    if (m_hTitleFont) DeleteObject(m_hTitleFont); // Libérer la police
}

bool Window::Display()  
{
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
    int anchorSpacing = 8;

    int themeWidth = 300;
    Button themeButton(L"T", RED, (465 + anchorSpacing), 25, 35, 35, (HMENU)1, m_hInstance, m_hWnd);
    themeButton.Create();

    int downloadImageButtonWidth = 300;
    Button downloadImageButton(L"Download an image", RED, (((WINDOW_WIDTH - downloadImageButtonWidth) / 2) - anchorSpacing), 100, downloadImageButtonWidth, 40, (HMENU)2, m_hInstance, m_hWnd);
    downloadImageButton.Create();

    int hideMessageButtonWidth = 480;
    Button hideMessageButton(L"Hide the message", GREEN, (((WINDOW_WIDTH - hideMessageButtonWidth) / 2) - anchorSpacing), 700, hideMessageButtonWidth, 40, (HMENU)3, m_hInstance, m_hWnd);
    hideMessageButton.Create();

    int downloadNewImageWidth = 480;
    Button downloadNewImageButton(L"Download the new image", BLUE, (((WINDOW_WIDTH - downloadNewImageWidth) / 2) - anchorSpacing), 750, downloadNewImageWidth, 40, (HMENU)4, m_hInstance, m_hWnd);
    downloadNewImageButton.Create();

    int bottomButtonWidth = WINDOW_WIDTH / 2;
    int bottomButtonHeight = 40;
    int bottomButtonPosY = WINDOW_HEIGHT - (bottomButtonHeight * 2);
    Button encodeButton(L"ENCODE", LIGHT_GREY, 0, bottomButtonPosY, bottomButtonWidth, bottomButtonHeight, (HMENU)5, m_hInstance, m_hWnd);
    encodeButton.Create();
    Button decodeButton(L"DECODE", DARK_GREY, bottomButtonWidth, bottomButtonPosY, bottomButtonWidth, bottomButtonHeight, (HMENU)6, m_hInstance, m_hWnd);
    decodeButton.Create();
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

    return RECT();
}

BOOL Window::InitInstance(int nCmdShow) 
{
    RECT CenteredWindow = GetCenteredWindow();

    m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CenteredWindow.left, CenteredWindow.top, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWnd) return FALSE;

    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);

    return TRUE;
}

void Window::BackgroundColor(HDC hdc, PAINTSTRUCT ps)
{
    // Définir la couleur d'arrière-plan
    HBRUSH hBrush = CreateSolidBrush(BACKGROUND_COLOR);     // Remplace par la couleur désirée
    FillRect(hdc, &ps.rcPaint, hBrush);                     // Remplir l'arrière-plan
    DeleteObject(hBrush);                                   // Libérer la brosse
}

void Window::DrawTitle(HDC hdc)
{

    //if (!m_hTitleFont) // Créer la police seulement si elle n'est pas déjà créée
    //{
    //    m_hTitleFont = CreateFont(
    //        50, // Hauteur de la police
    //        0,  // Largeur de la police
    //        0,  // Angle de rotation
    //        0,  // Angle d'orientation
    //        FW_BOLD, // Épaisseur de la police
    //        FALSE, // Italique
    //        FALSE, // Souligné
    //        FALSE, // Barré
    //        DEFAULT_CHARSET,
    //        OUT_DEFAULT_PRECIS,
    //        CLIP_DEFAULT_PRECIS,
    //        DEFAULT_QUALITY,
    //        DEFAULT_FQUALITY,
    //        L"Arial"); // Choisissez votre police ici
    //}

    SetTextColor(hdc, TEXT_COLOR);
    SetBkMode(hdc, TRANSPARENT); // Pour un fond transparent

    //const WCHAR* title = WINDOW_TITLE; // Utilisez votre titre ici
    //TextOutW(hdc, (WINDOW_HEIGHT / 20), (WINDOW_WIDTH / 15), title, wcslen(title));
    TextOut(hdc, (WINDOW_WIDTH / 20), (WINDOW_HEIGHT / 15), m_szTitle, wcslen(m_szTitle));
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message) 
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        default:
            // Gérer action
        case 1: // themeButton
            // Gérer l'action de cilc
            break;
        case 2: // downloadImageButton
            // Gérer l'action de clic 
            break;
        case 3: // hideMessageButton
            // Gérer l'action de clic 
            break;
        case 4: // downloadNewImageButton
            //
            break;
        }
        break;
    case WM_PAINT: 
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        BackgroundColor(hdc, ps);
        //DrawTitle(hdc);

        EndPaint(hWnd, &ps);
    } break;
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

INT_PTR CALLBACK Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
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