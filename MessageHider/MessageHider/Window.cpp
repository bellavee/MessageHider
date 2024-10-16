#include "window.h"

std::vector<Button*> Window::m_buttons;

Window::Window(HINSTANCE hInst, int nCmdShow)
    : m_hInstance(hInst), m_imageLoaded(false)
{

    wcscpy_s(m_szTitle, WINDOW_TITLE); // Utilisez le titre fixe

    //LoadStringW(hInst, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInst, IDC_MESSAGEHIDER, m_szWindowClass, MAX_LOADSTRING);

    if (!MyRegisterClass() || !InitInstance(nCmdShow)) {
        m_hWnd = nullptr;
        MessageBoxA(NULL, "Failed to create window", "Error", MB_ICONERROR | MB_OK);
    }
}

Window::~Window()
{
    //if (m_hTitleFont) DeleteObject(m_hTitleFont); // Lib�rer la police
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

    int btmBtnW = WINDOW_WIDTH / 2;
    int btnHeight = 40;
    int btmBtnPosY = WINDOW_HEIGHT - (btnHeight * 2);

    m_buttons.push_back(new Button(ButtonType::EncodePage, LIGHT_GREY, 0, btmBtnPosY, btmBtnW, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::DecodePage, DARK_GREY, btmBtnW, btmBtnPosY, btmBtnW, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Theme, RED, (465 + anchorSpacing), 25, 35, 35, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Load, RED, (((WINDOW_WIDTH - 300) / 2) - anchorSpacing), 100, 300, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Download, BLUE, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 750, 480, btnHeight, m_hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::EncodeAction, GREEN, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 700, 480, btnHeight, m_hInstance, m_hWnd));
    //m_buttons.push_back(new Button(ButtonType::DecodeAction, GREEN, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 700, 480, btnHeight, hInstance, m_hWnd));

    for (Button* button : m_buttons) button->Create();
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

void Window::BackgroundColor(HDC hdc, PAINTSTRUCT ps)
{
    HBRUSH hBrush = CreateSolidBrush(BACKGROUND_COLOR);
    FillRect(hdc, &ps.rcPaint, hBrush);
    DeleteObject(hBrush);
}

void Window::DrawTitle(HDC hdc)
{

    //if (!m_hTitleFont) // Cr�er la police seulement si elle n'est pas d�j� cr��e
    //{
    //    m_hTitleFont = CreateFont(
    //        50, // Hauteur de la police
    //        0,  // Largeur de la police
    //        0,  // Angle de rotation
    //        0,  // Angle d'orientation
    //        FW_BOLD, // �paisseur de la police
    //        FALSE, // Italique
    //        FALSE, // Soulign�
    //        FALSE, // Barr�
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

    switch (message) 
    {
    case WM_CREATE:
        if (pThis) pThis->LoadPngImage(); 
        break;
    case WM_COMMAND:
    {
        HMENU commandId = reinterpret_cast<HMENU>(LOWORD(wParam));

        for (Button* button : m_buttons)
        {
            if (button->GetId() == (HMENU)LOWORD(wParam)) button->OnClick();
        }
    }
    case WM_PAINT: 
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        //DrawTitle(hdc);
        if (pThis)
        {
            pThis->BackgroundColor(hdc, ps);
            if (pThis->m_imageLoaded && pThis->m_pngImage)
            {
                pThis->m_pngImage->Render(hdc, 0, 0);
            }
            else if (!pThis->m_imageLoaded)
            {
                TextOutA(hdc, 10, 10, "Loading image...", 15);
            }
        }

        EndPaint(hWnd, &ps);
    } 
    break;
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

void Window::LoadPngImage()
{
    m_pngImage = std::make_unique<PngImage>();
    try {
        m_pngImage->LoadFromFile("sample.png");
        m_imageLoaded = true;
        InvalidateRect(m_hWnd, NULL, TRUE);  // Force a redraw
    }
    catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error loading PNG", MB_OK | MB_ICONERROR);
    }
}