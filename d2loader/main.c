#include <Windows.h>
#include "functions/sub_404b60.h"
#include "tests.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
#if ENABLE_TESTS
    Tests();
    return 0;
#else
    return sub_404b60_WinMain(
        hInstance,
        hPrevInstance,
        lpCmdLine,
        nShowCmd
    );
#endif
}
