#include "framework.h"

#include <windows.h>
#include <cmath>

// Глобальные переменные
HINSTANCE hInst;
LPCTSTR szClassName = L"WinAPICosineGraph"; // Имя класса окна
int screenWidth = 800; // Ширина окна
int screenHeight = 600; // Высота окна
double scale = 40.0; // Масштаб графика
double offsetX = 0.0; // Смещение по горизонтали
double offsetY = 0.0; // Смещение по вертикали

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DrawGraph(HDC hdc);
void DrawAxis(HDC hdc);
void DrawGrid(HDC hdc);
void DrawText(HDC hdc);

// Точка входа в программу
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    hInst = hInstance;
    WNDCLASSEX wcex;

    // Заполнение структуры wcex данными о классе окна
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; // Указатель на оконную процедуру
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szClassName;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Регистрация класса окна
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"Failed to register the window class.", L"Error", MB_ICONERROR);
        return 1;
    }

    // Создание окна
    HWND hWnd = CreateWindow(szClassName, L"Graph of Cosine Function", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        screenWidth, screenHeight, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, L"Failed to create the window.", L"Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawGraph(hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_SIZE:
    {
        // Обработка изменения размеров окна
        screenWidth = LOWORD(lParam);
        screenHeight = HIWORD(lParam);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Функция для рисования графика
void DrawGraph(HDC hdc)
{
    // Создание перьев и шрифта
    HPEN hPenFunction = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // Перо для графика
    HPEN hPenAxis = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Перо для осей
    HPEN hPenGrid = CreatePen(PS_DOT, 1, RGB(0, 0, 0)); // Перо для сетки
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial"); // Шрифт для текста

    // Выбор объектов в контексте устройства
    SelectObject(hdc, hPenFunction);
    MoveToEx(hdc, 0, screenHeight / 2, NULL);

    // Рисование графика функции до точки разрыва
    for (int x = 0; x < screenWidth / 2; x++)
    {
        double xValue = (x - screenWidth / 2.0) / scale;
        double yValue = ((2 * xValue - 1) / (xValue + 2)) * scale + offsetY;
        int screenY = screenHeight / 2 - (int)yValue;

        if (xValue >= -2.0)
        {
            break;
        }

        if (x == 0)
        {
            MoveToEx(hdc, x, screenY, NULL);
        }
        else
        {
            LineTo(hdc, x, screenY);
        }
    }

    // Выбор других перьев и шрифта
    SelectObject(hdc, hPenAxis);
    DrawAxis(hdc);

    SelectObject(hdc, hPenGrid);
    DrawGrid(hdc);

    SelectObject(hdc, hPenAxis);
    SelectObject(hdc, hFont);
    DrawText(hdc);

    // Рисование графика функции после точки разрыва
    SelectObject(hdc, hPenFunction);
    MoveToEx(hdc, 0, screenHeight / 2, NULL);

    for (int x = screenWidth / 2; x < screenWidth; x++)
    {
        double xValue = (x - screenWidth / 2.0) / scale;
        double yValue = ((2 * xValue - 1) / (xValue + 2)) * scale + offsetY;
        int screenY = screenHeight / 2 - (int)yValue;

        if (x == screenWidth / 2)
        {
            MoveToEx(hdc, x, screenY, NULL);
        }
        else
        {
            LineTo(hdc, x, screenY);
        }
    }

    // Освобождение ресурсов
    DeleteObject(hPenFunction);
    DeleteObject(hPenAxis);
    DeleteObject(hPenGrid);
    DeleteObject(hFont);
}

// Рисование осей
void DrawAxis(HDC hdc)
{
    // Горизонтальная ось
    MoveToEx(hdc, 0, screenHeight / 2, NULL);
    LineTo(hdc, screenWidth, screenHeight / 2);

    // Вертикальная ось
    MoveToEx(hdc, screenWidth / 2, 0, NULL);
    LineTo(hdc, screenWidth / 2, screenHeight);

    // Добавляем метки и цифры на осях
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    // Метки и цифры на горизонтальной оси
    for (int x = -10; x <= 10; x++)
    {
        int xPos = screenWidth / 2 + x * 40;
        MoveToEx(hdc, xPos, screenHeight / 2 - 5, NULL); // Рисуем метки над осью
        LineTo(hdc, xPos, screenHeight / 2 + 5);

        WCHAR strX[16];
        wsprintf(strX, L"%d", x);

        RECT textRect;
        textRect.left = xPos - 15;
        textRect.right = xPos + 15;
        textRect.top = screenHeight / 2 + 10;
        textRect.bottom = screenHeight / 2 + 30;

        DrawText(hdc, strX, -1, &textRect, DT_CENTER);
    }

    // Метки и цифры на вертикальной оси
    for (int y = -10; y <= 10; y++)
    {
        int yPos = screenHeight / 2 + y * 40;
        MoveToEx(hdc, screenWidth / 2 - 5, yPos, NULL); // Рисуем метки слева от оси
        LineTo(hdc, screenWidth / 2 + 5, yPos);

        WCHAR strY[16];
        wsprintf(strY, L"%d", y);

        RECT textRect;
        textRect.left = screenWidth / 2 - 30;
        textRect.right = screenWidth / 2 - 10;
        textRect.top = yPos - 10;
        textRect.bottom = yPos + 10;

        DrawText(hdc, strY, -1, &textRect, DT_CENTER);
    }

    // Освобождение ресурсов
    DeleteObject(hFont);
}

// Рисование сетки
void DrawGrid(HDC hdc)
{
    int step = 40; // Шаг сетки

    // Вертикальные линии
    for (int x = screenWidth / 2 + step; x < screenWidth; x += step)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, screenHeight);
        MoveToEx(hdc, screenWidth - x, 0, NULL);
        LineTo(hdc, screenWidth - x, screenHeight);
    }

    // Горизонтальные линии
    for (int y = screenHeight / 2 + step; y < screenHeight; y += step)
    {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, screenWidth, y);
        MoveToEx(hdc, 0, screenHeight - y, NULL);
        LineTo(hdc, screenWidth, screenHeight - y);
    }
}

// Рисование текста (названия над графиком)
void DrawText(HDC hdc)
{
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hFont);
    TextOut(hdc, screenWidth / 2 - 60, 10, L"y = cos(x)", 10); // Вывод текста
    DeleteObject(hFont);
}