
/*
	项目逻辑处理文件
*/

#include "LuanDouXiyou.h"
#include "Tools.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>

//全局函数声明
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//变量初始化
WindowConfig wndconf(1300, 640, 40);
Scene scenes[3] = {
	{3633, 640, "res/scene/scene0001.bmp", 0, 200, 272, 200 + 118, "res/scene/guild0001.bmp"},
	{3960, 640, "res/scene/scene0002.bmp", 300, 200, 300 + 159, 200 + 183, "res/scene/guild0002.bmp"},
	{3762, 640, "res/scene/scene0003.bmp", 600, 200, 600 + 148, 200 + 146, "res/scene/guild0003.bmp"}

};
MainScene mscene(wndconf, scenes, 0, 0, 1300, 640, "res/mainscene/main_scene.bmp");


//-------------------WinMain函数，程序入口点函数-----------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	//初始化
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	GetMessage(&msg, NULL, NULL, NULL);//初始化msg              
	//游戏循环
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			wndconf.tNow = GetTickCount();
			if (wndconf.tNow - wndconf.tPre >= wndconf.interval) {
				mscene.Play();
				wndconf.MergeDC();
			}

		}
	}
	return msg.wParam;
}

//----------------设计一个窗口类----------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "canvas";
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

//----------------初始化函数---------------------
// 加载位图并设定各种初始值
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	wndconf.hwnd = CreateWindow("canvas", "绘图窗口", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, wndconf.rect.right - wndconf.rect.left, wndconf.rect.bottom - wndconf.rect.top, NULL, NULL, hInstance, NULL);

	if (!wndconf.hwnd)
	{
		return FALSE;
	}

	MoveWindow(wndconf.hwnd, 100, 50, wndconf.rect.right - wndconf.rect.left, wndconf.rect.bottom - wndconf.rect.top, true); //移动窗体至屏幕中央
	ShowWindow(wndconf.hwnd, nCmdShow);
	UpdateWindow(wndconf.hwnd);

	//-------------------加载位图并设定初始值-------------------------
	wndconf.InitializeDC(); //初始化DC
	
	mscene.LoadMainScene(); //加载主场景到mdc

	wndconf.MergeDC(); //将mdc贴到hdc

	//scenes[0].LoadScene(wndconf);
	//wndconf.MergeDC();

	return TRUE;
}

//------------消息处理函数---------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	switch (message)
	{
	case WM_LBUTTONDOWN:

		if (mscene.scene_id == MAINSCENE) {
			if (x >= mscene.scenes[0].click_area.left 
				&& x <= mscene.scenes[0].click_area.right 
				&& y >= mscene.scenes[0].click_area.top 
				&& y <= mscene.scenes[0].click_area.bottom) {
				mscene.Choose(0);
				char str[100];
				sprintf_s(str, "%d", mscene.scene_id);
				MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
			if (x >= mscene.scenes[1].click_area.left
				&& x <= mscene.scenes[1].click_area.right
				&& y >= mscene.scenes[1].click_area.top
				&& y <= mscene.scenes[1].click_area.bottom) {
				mscene.Choose(1);
				char str[100];
				sprintf_s(str, "%d", mscene.scene_id);
				MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
			if (x >= mscene.scenes[2].click_area.left
				&& x <= mscene.scenes[2].click_area.right
				&& y >= mscene.scenes[2].click_area.top
				&& y <= mscene.scenes[2].click_area.bottom) {
				mscene.Choose(2);
				char str[100];
				sprintf_s(str, "%d", mscene.scene_id);
				MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
			case 'A':
				keyA = true;
				//mscene.Play();
				//scenes[0].Change(wndconf, -5);
				//wndconf.MergeDC();
				break;
			case 'D':
				keyD = true;
				//mscene.Play();
				//scenes[0].Change(wndconf, 5);
				//wndconf.MergeDC();
				break;

		}
		break;
	case WM_KEYUP:
		switch (wParam) {
			case 'A':
				keyA = false;
				break;
			case 'D':
				keyD = false;
				break;

			break;
		}
	break;
	case WM_DESTROY:			    	//窗口结束消息
		int i;

		DeleteDC(wndconf.mdc);
		DeleteDC(wndconf.bufdc);
		ReleaseDC(hwnd, wndconf.hdc);

		PostQuitMessage(0);
		break;
	default:							//其他消息
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

