#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <conio.h>

using namespace std;

PAINTSTRUCT paintStruct = { NULL };

int wmain(int argc, wchar_t* argv[])
{
	wstring fileName = L"Mask.bmp"; //only .bmp
	if (argc > 1)
		fileName = argv[1];
	HDC hdc = BeginPaint(GetConsoleWindow(), &paintStruct);
	HANDLE hBitmap = LoadImage(NULL, fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hBitmap) { wcout << "Can't load " << fileName << "\n"; return -1; }
	HDC hdcMem = CreateCompatibleDC(hdc); // создаем совместимый контекст
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hBitmap); // устанавливам там свой bitmap
	bool pixelsColorMap[32][32] = { };
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 32; ++x)
		{
			COLORREF colorRef = GetPixel(hdcMem, x, y);
			int R = 0, G = 0, B = 0;
			R = GetRValue(colorRef);
			G = GetGValue(colorRef);
			B = GetBValue(colorRef);
			if (R < 127 && G < 127 && B < 127)
				pixelsColorMap[y][x] = true;
		}
	}
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 32; ++x)
		{
			cout << pixelsColorMap[y][x] << " ";
		}
		cout << endl;
	}
	cout << "\n\n";
	int counter = 0;
	int FOURResults[4] = { };
	vector<int> finalMaskVector = { };
	for (int y = 31; y >= 0; --y)
	{
		for (int clearIndex = 0; clearIndex < 4; ++clearIndex) { FOURResults[clearIndex] = 0; }
		int xOffset = 0, endXCycle = 0, lastByteResult = 0;
		for (int j = 1; j <= 4; ++j)
		{
			lastByteResult = 0;
			for (int x = 7 * j + xOffset; x >= endXCycle; --x)
			{
				if (pixelsColorMap[y][x])
					lastByteResult += (int)pow(2, abs((x % 8) - 7));

			}
			++xOffset;
			endXCycle += 8;
			counter++;
			FOURResults[j - 1] = lastByteResult;
			finalMaskVector.push_back(lastByteResult);
		}
	}
	cout << "--------------------\n\n\n";
	for (size_t i = 1; i <= 128; ++i)
	{
		if (i % 4)
			cout << finalMaskVector[i - 1] << ", ";
		else if (i != 128)
			cout << finalMaskVector[i - 1] << "," << endl;
		else
			cout << finalMaskVector[i - 1];
	}
	SelectObject(hdcMem, hbmOld); // возвращаем назад битмап контекста
	DeleteDC(hdcMem); // удаляем контекст, он уже не нужен
	if (GetLastError()) { cout << "GetLastError return: " << GetLastError() << endl; return -1; }
	_getch();
	return 0;
}
