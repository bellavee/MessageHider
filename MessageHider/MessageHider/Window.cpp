#include "window.h"
#include <CommCtrl.h>


Window::Window(HINSTANCE hInst, int nCmdShow)
    : hInstance(hInst)
{
    LoadStringW(hInst, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInst, IDC_MESSAGEHIDER, m_szWindowClass, MAX_LOADSTRING);

    if (!MyRegisterClass() || !InitInstance(nCmdShow)) m_hWnd = nullptr;
}

Window::~Window()
{
}

bool Window::Create() const 
{
    return m_hWnd != nullptr;
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

void Window::InitCommonControls()
{
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;       // Ajoute les classes de contrôle d'onglets
    InitCommonControlsEx(&icex);        // Initialise les contrôles
}

ATOM Window::MyRegisterClass()
{
    InitCommonControls();

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

    return RECT();
}

BOOL Window::InitInstance(int nCmdShow) 
{
    RECT CenteredWindow = GetCenteredWindow();

    // Créer la fenêtre avec la taille et la position centrées
    m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,  // Styles pour empêcher le redimensionnement
        CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, hInstance, nullptr);

    if (!m_hWnd) return FALSE;

    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);

    return TRUE;
}


LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message) 
    {
    case WM_COMMAND: 
    {
        int wmId = LOWORD(wParam);
        switch (wmId) 
        {
        case IDM_ABOUT:
            DialogBox(reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE)), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    } break;
    case WM_PAINT: 
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    } break;
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