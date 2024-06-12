#pragma once

#include "Common.h"

typedef struct WindowConfig {
	HDC	hdc; //主绘布
	HDC	mdc; //缓冲绘布
	HDC	bufdc; //角色和场景加载绘布
	HWND	hwnd; //窗体
	DWORD	tPre, tNow; //游戏加载时间
	DWORD	interval; //游戏循环时间间隔
	HBITMAP fullmap; //虚拟位图，用于mdc
	RECT rect; //实际窗体大小，包括顶栏
	int w; //窗体宽度
	int h; //窗体高度

	WindowConfig();
	WindowConfig(int _w, int _h, int _interval);
	void InitializeDC(); //初始化DC
	void MergeDC(); //合并DC
};

WindowConfig::WindowConfig() {
	w = 0;
	h = 0;
	interval = 0;
	rect = { 0, 0, w, h };
}

WindowConfig::WindowConfig(int _w, int _h, int _interval) {
	w = _w;
	h = _h;
	interval = _interval;
	rect = { 0, 0, w, h };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE); //调整窗体尺寸，消除顶栏的影响
}

void WindowConfig::InitializeDC() {
	hdc = GetDC(hwnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	fullmap = CreateCompatibleBitmap(hdc, w, h);
	//char str[100];
	//sprintf_s(str, "w:%d, h:%d", w, h);
	//MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
	SelectObject(mdc, fullmap);
}
void WindowConfig::MergeDC() {
	BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
}