#pragma once

#include "Common.h"

typedef struct WindowConfig {
	HDC	hdc; //���沼
	HDC	mdc; //����沼
	HDC	bufdc; //��ɫ�ͳ������ػ沼
	HWND	hwnd; //����
	DWORD	tPre, tNow; //��Ϸ����ʱ��
	DWORD	interval; //��Ϸѭ��ʱ����
	HBITMAP fullmap; //����λͼ������mdc
	RECT rect; //ʵ�ʴ����С����������
	int w; //������
	int h; //����߶�

	WindowConfig();
	WindowConfig(int _w, int _h, int _interval);
	void InitializeDC(); //��ʼ��DC
	void MergeDC(); //�ϲ�DC
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
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE); //��������ߴ磬����������Ӱ��
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