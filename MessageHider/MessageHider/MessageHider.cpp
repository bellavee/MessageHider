#include "MessageHider.h"
#include "Window.h"
#include "framework.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) 
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Window window(hInstance, nCmdShow);

    if (!window.Create())return 0;      // Erreur lors de la création de la fenêtre

    window.ShowMessageLoop();           // Lance la boucle de messages

    return 0;
}