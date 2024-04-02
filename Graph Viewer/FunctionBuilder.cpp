#include "framework.h"
#include "FunctionBuilder.h"
#include <iostream>
#include <string>

#define MAX_LOADSTRING 100


//imagine this is IFunction.h

class IFunction
{
public:
    virtual double getY(double x) = 0;
};

//imagine this is CosFunc.h

class CosFunc:IFunction {
private:
    double a;
    double k;
    double b;
public:
    double getY(double x);
    CosFunc();
    CosFunc(double a, double b, double k);
    ~CosFunc() {  };
};

//imagine this is CosFunc.cpp

CosFunc::CosFunc(double a, double b, double k) {
    this->a = a;
    this->b = b;
    this->k = k;
}

CosFunc::CosFunc() {
    this->a = 1;
    this->b = 1;
    this->k = 0;
}

double CosFunc::getY(double x) {
    return -(k * std::cos(a*x) + b);
}

// main cpp starts here

void DrawNoScaleNums(HDC hdc, HWND hWnd) { // some text is drawn in DrawFuncs's for loops so we can use less for loops here
    RECT windowRect;
    GetClientRect(hWnd, &windowRect);

    LPCWSTR message = L"y";
    RECT rect = { windowRect.right/2+30, 10, 0, 0 };
    SetTextColor(hdc, 0x00000000);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
    message = L"x";
    rect = { windowRect.right - 30, windowRect.bottom/2 - 40, 0, 0 };
    SetTextColor(hdc, 0x00000000);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
    message = L"0";
    rect = { windowRect.right/2 + 10, windowRect.bottom/2 + 10, 0, 0 };
    SetTextColor(hdc, 0x00000000);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);

   
}

void DrawFunction(HDC hdc, HWND hWnd) {
    RECT windowRect;
    HPEN pen_ = CreatePen(PS_SOLID, 2, RGB(255, 50, 50));
    HBRUSH brush = CreateSolidBrush(RGB(255, 200, 200));
    CosFunc lf(1,0,1);
    int y, yl;
    int percision = 0;

    GetClientRect(hWnd, &windowRect);
    SelectObject(hdc, pen_);
    SelectObject(hdc, brush);
    double scaleY = 1.38;


    int incr = min(windowRect.right / 20, windowRect.bottom / 10);
    MoveToEx(hdc, windowRect.right/2, lf.getY((double)(0) / (double)incr) * (double)incr + windowRect.bottom / 2, nullptr);

    // center to right
    for (int x = 0; x < windowRect.right/2; x+=1+percision) {
        y = lf.getY((double)(x) / (double)incr) * (double)incr + windowRect.bottom / 2;
        if (y < -1000000 || y > 1000000) { break; }
        LineTo(hdc, x + windowRect.right / 2, y); // look's better than SetPixel, but tan funcs look goofy
    }
    // center to left
    MoveToEx(hdc, windowRect.right / 2, lf.getY((double)(0) / (double)incr) * (double)incr + windowRect.bottom / 2, nullptr);
    for (int x = 0; x > - windowRect.right/2; x -= 1 - percision) {
        y = lf.getY((double)(x) / (double)incr) * (double)incr + windowRect.bottom / 2;
        if (y < -1000000 || y > 1000000) { break; }
        LineTo(hdc, x + windowRect.right / 2, y);
    }
  
}

void DrawGrid(HDC hdc, HWND hWnd) {
    RECT windowRect;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
    HPEN pen_ = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    std::wstring iws;
    LPCWSTR message;
    RECT rect;
    int incrX;  

    SelectObject(hdc, pen);
    GetClientRect(hWnd, &windowRect);
    incrX = min(windowRect.right / 20, windowRect.bottom/10);

    MoveToEx(hdc, windowRect.right/2, 0, nullptr);
    LineTo(hdc, windowRect.right/2, windowRect.bottom);
    MoveToEx(hdc, windowRect.left, windowRect.bottom/2, nullptr);
    LineTo(hdc, windowRect.right, windowRect.bottom/2);

    
    // vertical
    
    for (int i = windowRect.right / 2, j = windowRect.right / 2;
        i > 0 && j < windowRect.right; i -= incrX, j += incrX) {
        //left from center
        //lines
        SelectObject(hdc, pen_);
        MoveToEx(hdc, i, 0, nullptr);
        LineTo(hdc, i, windowRect.bottom);
        SelectObject(hdc, pen);
        MoveToEx(hdc, i, windowRect.bottom/2 - 5, nullptr);
        LineTo(hdc, i, windowRect.bottom / 2 + 5);
        //text
        if (i != windowRect.right / 2) {
            iws = std::to_wstring(-(windowRect.right / 2 - i) / incrX);
            message = iws.c_str();
            rect = { i - 8,  15 + windowRect.bottom / 2, 0, 0 };
            SetTextColor(hdc, 0x00000000);
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
        }
        //right from center
        //lines
        SelectObject(hdc, pen_);
        MoveToEx(hdc, j, 0, nullptr);
        LineTo(hdc, j, windowRect.bottom);
        SelectObject(hdc, pen);
        MoveToEx(hdc, j, windowRect.bottom / 2 - 5, nullptr);
        LineTo(hdc, j, windowRect.bottom / 2 + 5);
        //text
        if (-(windowRect.right / 2 - j) / incrX != 0) {
            iws = std::to_wstring(-(windowRect.right / 2 - j) / incrX);
            message = iws.c_str();
            rect = { j - 4,  15 + windowRect.bottom / 2, 0, 0 };
            SetTextColor(hdc, 0x00000000);
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
        }
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
        MoveToEx(hdc, windowRect.right / 2 - 5, i, nullptr);
        LineTo(hdc, windowRect.right / 2 + 5, i);
        //text
        if (windowRect.bottom / incrX / 2 - (i / incrX) != 0) {
            std::wstring iws = std::to_wstring(windowRect.bottom / incrX / 2 - (i / incrX));
            LPCWSTR message = iws.c_str();
            RECT rect = { 15 + windowRect.right / 2, i - 8, 0, 0 };
            SetTextColor(hdc, 0x00000000);
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
        }
        //down from center
        //lines
        SelectObject(hdc, pen_);
        MoveToEx(hdc, 0, j, nullptr);
        LineTo(hdc, windowRect.right, j);
        SelectObject(hdc, pen);
        MoveToEx(hdc, windowRect.right / 2 - 5, j, nullptr);
        LineTo(hdc, windowRect.right / 2 + 5, j);
        //text
        if (j / incrX - windowRect.bottom / incrX / 2 != 0) {
            std::wstring iws_ = std::to_wstring(j / incrX - windowRect.bottom / incrX / 2);
            LPCWSTR message_ = iws_.c_str();
            RECT rect_ = { 15 + windowRect.right / 2, j-8, 0, 0 };
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

   HWND hWnd = CreateWindowW(szWindowClass, L"Function Builder v1.2", WS_OVERLAPPEDWINDOW,
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
            DrawFunction(hdc, hWnd);
            DrawNoScaleNums(hdc, hWnd);
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
