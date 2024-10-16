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

    AppManager& manager = AppManager::GetInstance();

    // Temp
    LSB lsb;
    if (!lsb.Encode("Long message de test")) return 0;
    std::string result = lsb.Decode();
    //

    Window window(hInstance, nCmdShow);

    if (!window.Display())return 0;      // Erreur lors de la création de la fenêtre

    window.ShowMessageLoop();           // Lance la boucle de messages

    return 0;
}