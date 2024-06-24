#include <iostream>
#include <Windows.h>
#include <thread>

using namespace std;
bool windowActived = false;
bool foundOutput = false;

void windowActive();
void bunnyHop();

int main()
{
    SetConsoleTitleA("kesh | External Bunny Hop");

    thread windowActiveThread(windowActive);
    thread bunnyHopThread(bunnyHop);

    COORD coord = {};
    coord.X = 200;
    coord.Y = 150;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 13);
    SetConsoleScreenBufferSize(hConsole, coord);


    HWND consoleWindowHandle = GetConsoleWindow();
    RECT r;
    GetWindowRect(consoleWindowHandle, &r);
    // SetWindowPos(consoleWindowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    MoveWindow(consoleWindowHandle, r.left, r.top, 500, 250, TRUE);

    cout << "Finding Game..." << endl;

    while (true)
    {
        HWND hWnd = FindWindowA("TankWindowClass", nullptr);
        if (hWnd)
        {
            if (!foundOutput)
            {
                system("cls");
                cout << "Game Found!" << endl;
                foundOutput = true;
            }
        }
        Sleep(1000);
    }

    cin.get();

    return 0;
}

void windowActive()
{
    while (true)
    {
        if (GetForegroundWindow() == FindWindowA("TankWindowClass", nullptr))
        {
            windowActived = true;
        }
        else
        {
            windowActived = false;
        }
        Sleep(100);
    }
}

void bunnyHop()
{
    while (true)
    {
        if (GetAsyncKeyState(VK_SPACE) && windowActived)
        {
            PostMessage(GetForegroundWindow(), WM_KEYDOWN, 0x20, NULL);
            PostMessage(GetForegroundWindow(), WM_KEYUP, 0x20, NULL);
            Sleep(25);
        }
    }
}
