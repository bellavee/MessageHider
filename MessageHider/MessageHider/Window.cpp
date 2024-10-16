#include "window.h"

std::vector<Button*> Window::m_buttons;

Window::Window(HINSTANCE hInst, int nCmdShow) : hInstance(hInst)
{
    LoadStringW(hInst, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInst, IDC_MESSAGEHIDER, m_szWindowClass, MAX_LOADSTRING);

    if (!MyRegisterClass() || !InitInstance(nCmdShow)) {
        m_hWnd = nullptr;
        MessageBoxA(NULL, "Failed to create window", "Error", MB_ICONERROR | MB_OK);
    }
}

Window::~Window()
{
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
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MESSAGEHIDER));

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

    m_buttons.push_back(new Button(ButtonType::EncodePage, LIGHT_GREY, 0, btmBtnPosY, btmBtnW, btnHeight, hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::DecodePage, DARK_GREY, btmBtnW, btmBtnPosY, btmBtnW, btnHeight, hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Theme, RED, (465 + anchorSpacing), 25, 35, 35, hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Load, RED, (((WINDOW_WIDTH - 300) / 2) - anchorSpacing), 100, 300, btnHeight, hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::Download, BLUE, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 750, 480, btnHeight, hInstance, m_hWnd));
    m_buttons.push_back(new Button(ButtonType::EncodeAction, GREEN, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 700, 480, btnHeight, hInstance, m_hWnd));
    //m_buttons.push_back(new Button(ButtonType::DecodeAction, GREEN, (((WINDOW_WIDTH - 480) / 2) - anchorSpacing), 700, 480, btnHeight, hInstance, m_hWnd));

    for (Button* button : m_buttons) button->Create();

    /*int themeWidth = 300;
    Button themeButton(L"T", RED, (465 + anchorSpacing), 25, 35, 35, (HMENU)1, hInstance, m_hWnd);
    themeButton.Create();*/

    /*int downloadImageButtonWidth = 300;
    Button downloadImageButton(L"Download an image", RED, (((WINDOW_WIDTH - downloadImageButtonWidth) / 2) - anchorSpacing), 100, downloadImageButtonWidth, 40, (HMENU)2, hInstance, m_hWnd);
    downloadImageButton.Create();*/

    /*int hideMessageButtonWidth = 480;
    Button hideMessageButton(L"Hide the message", GREEN, (((WINDOW_WIDTH - hideMessageButtonWidth) / 2) - anchorSpacing), 700, hideMessageButtonWidth, 40, (HMENU)3, hInstance, m_hWnd);
    hideMessageButton.Create();*/

    /*int downloadNewImageWidth = 480;
    Button downloadNewImageButton(L"Download the new image", BLUE, (((WINDOW_WIDTH - downloadNewImageWidth) / 2) - anchorSpacing), 750, downloadNewImageWidth, 40, (HMENU)4, hInstance, m_hWnd);
    downloadNewImageButton.Create();*/

    /*int bottomButtonWidth = WINDOW_WIDTH / 2;
    int bottomButtonHeight = 40;
    int bottomButtonPosY = WINDOW_HEIGHT - (bottomButtonHeight * 2);
    Button encodeButton(L"ENCODE", LIGHT_GREY, 0, bottomButtonPosY, bottomButtonWidth, bottomButtonHeight, (HMENU)5, hInstance, m_hWnd);
    encodeButton.Create();
    Button decodeButton(L"DECODE", DARK_GREY, bottomButtonWidth, bottomButtonPosY, bottomButtonWidth, bottomButtonHeight, (HMENU)6, hInstance, m_hWnd);
    decodeButton.Create();*/
}

ATOM Window::MyRegisterClass() const
{
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
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
        nullptr, nullptr, hInstance, this);

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
            if (button->GetId() == (HMENU)LOWORD(wParam))
            {
                button->OnClick();
                break;
            }
        }
    }
    case WM_PAINT: 
    {
        AppManager& manager = AppManager::GetInstance();

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (pThis)
        {
            pThis->BackgroundColor(hdc, ps, manager.HasDarkTheme() ? BLACK : WHITE);
            if (manager.HasImageLoaded() && manager.GetPngImage())
            {
                manager.GetPngImage()->Render(hdc, 0, 0);
            }
            /*else if (!manager.HasImageLoaded())
            {
                TextOutA(hdc, 10, 10, "Loading image...", 15);
            }*/
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