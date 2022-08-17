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
    case WM_SIZE:
        WndResize(lParam);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
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
void Init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glOrtho(0, 30, 0, 30, 0, 1);

    obj.pos = { 20,0 };
    obj.pos2 = { 4,4 };

    player.pos = { 10,1 };
    player.pos2 = { 4,8 };


    DrawList.push_back(&player);
    DrawList.push_back(&obj);


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

            CalculatePhisic(player);

            Pos oldPos = player.pos;

            player.Move();
            if (CheckColisions(player, obj)) {
                cout << "BAN\n";
                player.pos = oldPos;
            }


            Now = duration_cast<microseconds>(system_clock::now().time_since_epoch());
        };
    }
}
void Draw(Object& object) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);

    glTranslatef(object.pos.x, object.pos.y, 0);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0, 0);
    glVertex2f(0, object.pos2.y);
    glVertex2f(object.pos2.x, object.pos2.y);
    glVertex2f(object.pos2.x, 0);

    glEnd();

    glPopMatrix();
}
void CalculatePhisic(Entity& entity) {
    if (entity.pos.y > 0) {
        entity.MoveVec.y = entity.MoveVec.y - g;
    }
}
bool CheckColisions(Object& object, Object& object2) {
    float xA[2] = { object.pos.x,object.pos.x + object.pos2.x };
    float xB[2] = { object2.pos.x,object2.pos.x + object2.pos2.x };

    float yA[2] = { object.pos.y,object.pos.y + object.pos2.y };
    float yB[2] = { object2.pos.y,object2.pos.y + object2.pos2.y };

    if (xA[1] < xB[0] || yA[1] < yB[0] || yA[0] > yB[1] || xA[0] > xB[1]) return false;
    return true;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}