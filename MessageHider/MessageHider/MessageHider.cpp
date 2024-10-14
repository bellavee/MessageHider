#include "MessageHider.h"
#include "Window.h"
#include "LSB.h"
#include "framework.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) 
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Temp
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    //Bitmap* pImage;
    //pImage = new Bitmap(L"eevee.png");
    LSB lsb;
    //if (!lsb.Encode(L"eevee.png", "Test")) return 0;
    //

    Window window(hInstance, nCmdShow);

    if (!window.Create())return 0;      // Erreur lors de la création de la fenêtre

    window.ShowMessageLoop();           // Lance la boucle de messages

    return 0;
}