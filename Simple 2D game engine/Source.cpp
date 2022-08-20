#pragma once
#include "Source.h"

int WINAPI WinMain(HINSTANCE _hInstance,
    HINSTANCE _hPrevInstance,
    LPSTR _lpCmdLine,
    int _nCmdShow)
{
    hInstance = _hInstance;
    hPrevInstance = _hPrevInstance;
    lpCmdLine = _lpCmdLine;
    nCmdShow = _nCmdShow;
    InitWindow();

    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            MainLoop();
            
        }
    }
    DisableOpenGL(hwnd, hDC, hRC);
    DestroyWindow(hwnd);

    return msg.wParam;
}
void InitWindow() {
    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return;

    /* create main window */
    hwnd = CreateWindowEx(0,
        L"GLSample",
        L"OpenGL Sample",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        512,
        512,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    Init();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
    {
        cout << "wParam = " << wParam << endl;
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_UP:
            DrawList.push_back(new Object(5, 5, 4, 4));
            break;
        }
        break;
    }
    case WM_KEYUP:
    {
        cout << "wParam2 = " << wParam << endl;
        switch (wParam)
        {
        }
        break;
    }
    case WM_MOUSEMOVE:
        if (Moving) Active->pos = GetMousePos(lParam) - MouseRelativeCoordinates;
        break;
    case WM_LBUTTONDOWN:
    {
        Object* object = nullptr;
        if (FindObject(GetMousePos(lParam), &object)) {
            if (object->color[0] != 0) {
                if (Active != nullptr) Active->color[0] = 1;
                object->color[0] = 0;
                Active = object;
            }
            else
            {
                Moving = true;
                MouseRelativeCoordinates = GetMousePos(lParam) - Active->pos;
            }

        }
        else
        {
            Moving = false;
        }
        break;
    }
    case WM_LBUTTONUP:
        if (Moving) {
            Active->pos = GetMousePos(lParam) - MouseRelativeCoordinates;
            Moving = false;
        }
        break;
    case WM_SIZE:
        WndResize(lParam);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}


void Init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glOrtho(0, WndWight / Size, 0, WndHeight / Size, 0, 1);

    obj.pos = { 4,8 };
    obj.pos2 = { 4,4 };

    obj2.pos = { 12,8 };
    obj2.pos2 = { 4,0.2 };

    //obj3.pos = { 20,15 };
    //obj3.pos2 = { 4,4 };
    obj3.pos = { 40,20 };
    obj3.pos2 = { 5,5 };

    player.pos = { 30,15 };
    player.pos2 = { 3.99,7.99 };
    player.color[1] = 0;


    DrawList.push_back(&player);
    DrawList.push_back(&obj);
    DrawList.push_back(&obj2);
    DrawList.push_back(&obj3);
    //DrawList.push_back(new Object(40,20,5,5));


    RedirectIOToConsole();
    HANDLE hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FixedUpdate, 0, 0, NULL);
}
void MainLoop() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < DrawList.size(); i++)
    {
        Draw(*DrawList[i]);
    }

    SwapBuffers(hDC);
    Sleep(1);
}
void FixedUpdate() {
    Now = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    int error = 0;
    while (true)
    {
        if (duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - Now.count() >= 10000) {
            Now = duration_cast<microseconds>(system_clock::now().time_since_epoch());


            player.Move();
            bool UpOrDown = false;
            player.OnGround = false;
            for (int i = 1; i < DrawList.size(); i++)
            {
                if (CheckColisions(player, *DrawList.at(i), player.MoveVec)) {
                    cout << "BAN\n";
                    UpOrDown = ResolveColisions(player, *DrawList.at(i), player.MoveVec);
                }
                if (CheckColisions(player, *DrawList.at(i), player.MoveVec, -n)) {
                    cout << "Staing\n";
                    player.OnGround = true;
                }
            }
            
            //cout << "BAN\n";
            CalculatePhisic(player);
            if (UpOrDown) {
                player.MoveVec.y = 0;
            }

            //player.Move();

        }
    }
}
bool FindObject(Pos pos, Object** findedObject) {
    for (auto i : DrawList) {
        if (CheckColisions(pos, *i)) {
            *findedObject = i;
            return true;
        }
    }
    return false;
}