#include "framework.h"
#include "Graph Viewer.h"
#include <iostream>
#include <string>

#define MAX_LOADSTRING 100


class IFunction
{
public:
    virtual double getY(double x) = 0;
};

class WeirdFunc:IFunction {
private:
    double k;
    int b;
public:
    double getY(double x);
    WeirdFunc();
    WeirdFunc(double k, int b);
    ~WeirdFunc() {};
};

WeirdFunc::WeirdFunc(double k, int b) {
    this->k = k;
    this->b = b;
}

WeirdFunc::WeirdFunc() {
    this->k = 1;
    this->b = 0;
}

double WeirdFunc::getY(double x) {
    return - std::sin(x) - cos(x*10);
}

void DrawNums(HDC hdc, HWND hWnd) {
    RECT windowRect;
    GetClientRect(hWnd, &windowRect);


    LPCWSTR message = L"y";
    RECT rect = { 30, 10, 0, 0 };
    SetTextColor(hdc, 0x00000000);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
    message = L"x";
    rect = { windowRect.right - 30, windowRect.bottom/2 - 40, 0, 0 };
    SetTextColor(hdc, 0x00000000);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);

   
}

void DrawFuncs(HDC hdc, HWND hWnd) {
    RECT windowRect;
    HPEN pen_ = CreatePen(PS_SOLID, 2, RGB(255, 50, 50));
    HBRUSH brush = CreateSolidBrush(RGB(255, 200, 200));
    WeirdFunc lf;
    int y, yl;
    int percision = 0;

    GetClientRect(hWnd, &windowRect);
    SelectObject(hdc, pen_);
    SelectObject(hdc, brush);
    double scaleY = 1.38;


    MoveToEx(hdc, 0, windowRect.bottom / 2, nullptr);
    int incr = min(windowRect.right / 20, windowRect.bottom / 10);
   

    for (int x = 0; x < windowRect.right; x+=1+percision) {
        y = lf.getY((double)x / (double)incr) * (double)incr + windowRect.bottom / 2;
        if (y < -10000 || y > 10000) { break; }
        LineTo(hdc, x, y);
        //SetPixel(hdc, x, y, RGB(255, 0, 0));

        //Ellipse(hdc, x-10, y-10, x+10, y+10);
    }
  
}

void DrawGrid(HDC hdc, HWND hWnd) {
    RECT windowRect;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
    HPEN pen_ = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    int incrX;  

    SelectObject(hdc, pen);
    GetClientRect(hWnd, &windowRect);
    incrX = min(windowRect.right / 20, windowRect.bottom/10);

    MoveToEx(hdc, 1, 0, nullptr);
    LineTo(hdc, windowRect.left+1, windowRect.bottom);
    MoveToEx(hdc, windowRect.left, windowRect.bottom/2, nullptr);
    LineTo(hdc, windowRect.right, windowRect.bottom/2);

    
    // vertical
    for (int i = incrX; i < windowRect.right; i += incrX) {
        //lines
        SelectObject(hdc, pen_);
        MoveToEx(hdc, i, 0, nullptr);
        LineTo(hdc, i, windowRect.bottom);
        SelectObject(hdc, pen);
        MoveToEx(hdc, i, windowRect.bottom/2-5, nullptr);
        LineTo(hdc, i, windowRect.bottom / 2+5);
        //text
        std::wstring iws = std::to_wstring(i/incrX);
        LPCWSTR message = iws.c_str();
        RECT rect = { i-4, windowRect.bottom/2+15, 0, 0 };
        SetTextColor(hdc, 0x00000000);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
    }
    // horizontal
    SelectObject(hdc, pen_);
    for (int i = windowRect.bottom/2, j = windowRect.bottom / 2;
        i > 0 && j < windowRect.bottom; i -= incrX, j += incrX) {
        //up from center
        //lines
        SelectObject(hdc, pen_);
        MoveToEx(hdc, 0, i, nullptr);
        LineTo(hdc, windowRect.right, i);
        SelectObject(hdc, pen);
        MoveToEx(hdc, 0, i, nullptr);
        LineTo(hdc, 5, i);
        //text
        std::wstring iws = std::to_wstring(windowRect.bottom/incrX/2-(i / incrX ));
        LPCWSTR message = iws.c_str();
        RECT rect = { 15, i-8, 0, 0 };
        SetTextColor(hdc, 0x00000000);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
        //down from center
        //lines
        SelectObject(hdc, pen_);
        MoveToEx(hdc, 0, j, nullptr);
        LineTo(hdc, windowRect.right, j);
        SelectObject(hdc, pen);
        MoveToEx(hdc, 0, j, nullptr);
        LineTo(hdc, 5, j);
        //text
        if (j != 0) {
            std::wstring iws_ = std::to_wstring(j / incrX - windowRect.bottom / incrX / 2);
            LPCWSTR message_ = iws_.c_str();
            RECT rect_ = { 15, j-8, 0, 0 };
            SetTextColor(hdc, 0x00000000);
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, message_, -1, &rect_, DT_SINGLELINE | DT_NOCLIP);
        }
    }

}

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHVIEWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHVIEWER));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHVIEWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GRAPHVIEWER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawGrid(hdc, hWnd);
            DrawFuncs(hdc, hWnd);
            DrawNums(hdc, hWnd);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
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
