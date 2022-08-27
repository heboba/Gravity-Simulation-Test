#pragma once
#include "Source.h"
#include "nlohmann/json.hpp"
#include <fstream>

bool CtrlPressed = false;

int WINAPI WinMain(HINSTANCE _hInstance,HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
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
            if (EditMode) DrawList.push_back(new Object(5 - Camera.x, 5 - Camera.y, 4, 4));
            break;
        case 88: // x
            if (EditMode) EditType = ChangeX;
            break;
        case 89: //y
            if (EditMode) EditType = ChangeY;
            break;
        case 67: //c
            SaveGame("Save.json");
            break;
        case 86: //v
            DrawList.clear();
            LoadGame("Save.json");
            break;
        case 70:
            if (EditMode) EditType = ChangeUfps;
            break;
        case 90: //z
            if (EditMode) {
                if (EditType != ScaleMode) {
                    EditType = ScaleMode;
                }
                else
                {
                    EditType = None;
                }
            }
            break;
        case 69: //e
            if (CtrlPressed) {
                if (!EditMode) {
                    EditMode = true;
                }
                else
                {
                    EditMode = false;
                    if (Active != nullptr) Active->color[0] = 1;
                }
            }
            break;
        case 17: // ctrl
            CtrlPressed = true;
            break;
        }
        break;
    }
    case WM_KEYUP:
    {
        cout << "wParam2 = " << wParam << endl;
        switch (wParam)
        {
        case 17:
            CtrlPressed = false;
            break;
        }
        break;
    }
    case WM_MOUSEMOVE:
        if (Moving && EditMode) Active->pos = GetMousePos(lParam) - MouseRelativeCoordinates;
        break;
    case WM_LBUTTONDOWN:
    {
        if (EditMode) {
            Object* object = nullptr;
            if (FindObject(GetMousePos(lParam) - Camera, &object)) {
                if (object->color[1] != 0.5) {
                    if (Active != nullptr) {
                        Active->color[1] = 1;
                        Active->color[2] = 1;
                    }
                    object->color[1] = 0.5;
                    object->color[2] = 0.5;
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
        }
        break;
    }
    case WM_MOUSEWHEEL:
        cout << "WM_MOUSEWHEEL\n";
        if (EditMode) {
            cout << "wParam = " << short(HIWORD(wParam)) << endl;
            if (Active != nullptr) {
                if (EditType == ChangeX) Active->pos2.x += 0.25 * (short(HIWORD(wParam)) / 120);
                if (EditType == ChangeY) Active->pos2.y += 0.25 * (short(HIWORD(wParam)) / 120);
                if (Active->pos2.x <= 0) Active->pos2.x = 0.25;
                if (Active->pos2.y <= 0) Active->pos2.y = 0.25;
            }
            if (EditType == ScaleMode) {
                Size += 0.25 * (short(HIWORD(wParam)) / 120);
                WndResize();
            }
            if (EditType == ChangeUfps) {
                ufps -= 500 * (short(HIWORD(wParam)) / 120);
            }
        }
        break;
    case WM_LBUTTONUP:
        if (Moving && EditMode && Active != nullptr) {
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

bool CheckAllColisions(Object** object) {
    for (int i = 1; i < DrawList.size(); i++) {
        if (CheckColisions(*player, *DrawList.at(i), player->MoveVec, n) || CheckColisions(*player, *DrawList.at(i), { 0,0 }, n)) {
            *object = DrawList.at(i);
            return true;
        }
    }
    return false;
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
void FixedUpdate() {
    Now = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    while (true)
    {
        if (duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - Now.count() >= ufps) {
            Now = duration_cast<microseconds>(system_clock::now().time_since_epoch());
            Update();
        }
    }
}
void SaveGame(std::string name) {
    nlohmann::json j;
    for (int i = 0; i < DrawList.size(); i++)
    {
        j[i]["Color"][0] = DrawList.at(i)->color[0];
        j[i]["Color"][1] = DrawList.at(i)->color[1];
        j[i]["Color"][2] = DrawList.at(i)->color[2];
        j[i]["Type"] = DrawList.at(i)->type;
        j[i]["pos"][0] = DrawList.at(i)->pos.x;
        j[i]["pos"][1] = DrawList.at(i)->pos.y;
        j[i]["pos2"][0] = DrawList.at(i)->pos2.x;
        j[i]["pos2"][1] = DrawList.at(i)->pos2.y;
    }
    std::ofstream o(name);
    o << std::setw(2) << j << std::endl;
}
void LoadGame(std::string name) {
    std::ifstream i(name);
    nlohmann::json j;
    i >> j;


    for (auto i : j)
    {
        ObjectType type = (ObjectType)i["Type"].get<int>();
        switch (type)
        {
        case tPlayer:
        {
            Player* obj = new Player();
            obj->pos.x = i["pos"][0].get<float>();
            obj->pos.y = i["pos"][1].get<float>();
            obj->pos2.x = i["pos2"][0].get<float>();
            obj->pos2.y = i["pos2"][1].get<float>();
            //obj->color[0] = i["Color"][0].get<float>();
            //obj->color[1] = i["Color"][1].get<float>();
            //obj->color[2] = i["Color"][2].get<float>();
            DrawList.push_back(obj);
            player = obj;
            break;
        }
        case tObject:
        {
            Object* obj = new Object();
            obj->pos.x = i["pos"][0].get<float>();
            obj->pos.y = i["pos"][1].get<float>();
            obj->pos2.x = i["pos2"][0].get<float>();
            obj->pos2.y = i["pos2"][1].get<float>();
            DrawList.push_back(obj);
            break;
        }
        default:
            cout << "Error!!!\n";
            Object* obj = new Object();
            break;
        }
    }
}

void LoadTexturesButtons();

void Init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glOrtho(0, WndWight / Size, 0, WndHeight / Size, 0, 1);

    RedirectIOToConsole();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    TexturesInit();

    obj.pos = { 4,8 };
    obj.pos2 = { 4,4 };

    obj2.pos = { 12,8 };
    obj2.pos2 = { 4,3 };

    obj3.pos = { 40,20 };
    obj3.pos2 = { 5,5 };

    player = new Player();

    player->pos = { 30,15 };
    player->pos2 = { 4,8 };
    player->color[1] = 0;


    DrawList.push_back(player);
    DrawList.push_back(&obj);
    DrawList.push_back(&obj2);
    DrawList.push_back(&obj3);

    SaveGame("Autosave.json");
    DrawList.clear();
    LoadGame("Autosave.json");

    LoadTexturesButtons();
    


    //DrawList.push_back(new Object(40,20,5,5));

    HANDLE hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FixedUpdate, 0, 0, NULL);
}

struct Button
{
    Pos pos1;
    Pos pos2;
    string name;
    Button(float x1, float y1, float x2, float y2, string name) {
        this->name = name;
        pos1 = { x1,y1 };
        pos2 = { x2,y2 };
    }
    Button(string name) {
        this->name = name;
    }
};

vector<Button> Buttons;

void LoadTexturesButtons() {
    std::string path = "textures/tex";
    for (const auto& entry : fs::directory_iterator(path)) {
        Buttons.push_back(Button(entry.path().filename().string()));
    }
}

float SCALE = 2;

void DrawButton(Button& button) {
    glPushMatrix();

    glColor3f(1, 1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);

    glTranslatef(button.pos1.x, WndHeight/Size - button.pos1.y, 0);

    float vertex2[] = { 0,0,0,0,-button.pos2.y,0 ,button.pos2.x, -button.pos2.y,0 ,button.pos2.x, 0,0 };
    glVertexPointer(3, GL_FLOAT, 0, vertex2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();

    DrawText(button.name, button.pos1.x + button.pos2.y, button.pos1.y, button.pos2.y, (button.pos2.x - button.pos2.y)/ button.pos2.y);

    glPushMatrix();


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TexturesAtlas[button.name]);

    glColor3f(1, 1, 1);

    glTranslatef(button.pos1.x, WndHeight / Size - button.pos1.y, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    float vertex3[] = { 0,0,0,0,-button.pos2.y,0 ,button.pos2.y, -button.pos2.y,0 ,button.pos2.y, 0,0 };
    //glScalef(1, 1, 1); 
    glVertexPointer(3, GL_FLOAT, 0, vertex3);
    glTexCoordPointer(2, GL_FLOAT, 0, texCord2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void DrawMenu() {
    glPushMatrix();

    Object menu = { 5,5,WndWight / Size - 10,WndHeight / Size - 10 };
    menu.texture = "Menu";
    Pos pos = { 0,0 };

    //Draw(menu, pos);
    DrawText("Menu", 5, 5, SCALE);

    int num = 0;
    for (int i = 0;i<10 && Buttons.size() > i + num;i++) {

        Buttons.at(i + num).pos1 = { 5,5 + SCALE + i * SCALE + i * SCALE/4};
        Buttons.at(i + num).pos2 = { 30,2 };

        DrawButton(Buttons.at(i + num));
   
    }

    glPopMatrix();
}

void MainLoop() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = DrawList.size() - 1; i >= 0; i--)
    {
        if (EditMode) DrawText("Edit mode active!", 0, 0, 2);
        if (EditMode) DrawText("Edit mode active!", 0, 2, 1.5);
        //Drawv2();
        Draw(*DrawList[i], Camera);

        //LoadTexturesButtons();
        DrawMenu();
        //Button btn("yellow_shulker_box.png");
        //btn.pos1 = { 10,10 };
        //btn.pos2 = { 30,2 };

        //DrawButton(btn);

    }
    SwapBuffers(hDC);
    Sleep(1);
}


void Update() {

    player->Move();


    bool UpOrDown = false;
    player->OnGround = false;
    Object* col = nullptr;


    while (CheckAllColisions(&col)) {
        if (!CheckColisions(*player, *col, player->MoveVec, n) && !CheckColisions(*player, *col, { 0,0 }, n)) continue;

        cout << "Resolving..." << endl;

        !UpOrDown ? UpOrDown = ResolveColisions(*player, *col, player->MoveVec) : ResolveColisions(*player, *col, player->MoveVec);
    }


    for (int i = 1; i < DrawList.size(); i++)
    {
        //Check for staing on Object
        if (!CheckColisions(*player, *DrawList.at(i), player->MoveVec, -n) && !CheckColisions(*player, *DrawList.at(i), { 0,0 }, -n)) continue;

        player->OnGround = true;
    }

    //Finaly move player
    CalculatePhisic(*player);

    //BugFix
    if (UpOrDown) {
        player->MoveVec.y = 0;
    }


    //Simple camera
    if (!EditMode) {
        float temp = (player->pos.x + player->pos2.x / 2) - (WndWight / Size) / 2 + Camera.x;
        if (abs(temp) > ((WndWight / Size) / 2) / 2) {
            Camera.x -= temp / 80;
        }

        temp = (player->pos.y + player->pos2.y / 2) - (WndHeight / Size) / 2 + Camera.y;
        if (abs(temp) > ((WndHeight / Size) / 2) / 2) {
            Camera.y -= temp / 50;
        }
    }
}
