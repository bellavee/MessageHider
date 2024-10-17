#include "MessageHider.h"
#include "Window.h"
#include "framework.h"
#include "LSB.h"
#include "AppManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) 
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Window window(hInstance, nCmdShow);

    if (!window.Display()) return 0;

    window.ShowMessageLoop();

    return 0;
}