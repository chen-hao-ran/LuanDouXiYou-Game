#include <windows.h>
#include <stdio.h>
#include <string.h>

//全局变量声明
//HBITMAP role, bg;
HBITMAP roleleft[15], roleright[15], scene;
HDC		hdc, mdc, bufdc;
HWND	hWnd;
DWORD	tPre, tNow;
int num;
int x, y;
bool jump, reach_up;
int jumpnum;
bool moveright, moveleft;
bool is_attack;
bool directleft, directright;

//全局函数声明
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				MyPaint(HDC hdc);
void MyTransparentBlt(HDC hdcDest,      // 目标DC
	int nXOriginDest,   // 目标X偏移
	int nYOriginDest,   // 目标Y偏移
	int nWidthDest,     // 目标宽度
	int nHeightDest,    // 目标高度
	HDC hdcSrc,         // 源DC
	int nXOriginSrc,    // 源X起点
	int nYOriginSrc,    // 源Y起点
	int nWidthSrc,      // 源宽度
	int nHeightSrc,     // 源高度
	UINT crTransparent  // 透明色,COLORREF类型
);
void PrintBitmap(HDC hdc, int x, int y, const char* bmpFilePath);

//****WinMain函数，程序入口点函数***********************
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
			tNow = GetTickCount();
			if (tNow - tPre >= 40) {
				MyPaint(hdc);
			}

		}
	}
	return msg.wParam;
}

//****设计一个窗口类****
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

//****初始化函数****
// 加载位图并设定各种初始值
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hWnd = CreateWindow("canvas", "绘图窗口", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd, 10, 10, 1320, 640, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	////////////////////////////////////////
	HBITMAP fullmap;
	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);

	//建立空的位图并置入mdc中
	fullmap = CreateCompatibleBitmap(hdc, 1320, 640);
	SelectObject(mdc, fullmap);

	//设定人物贴图初始位置和移动方向
	x = 300;
	y = 250;
	//dir = 0;
	num = 1;
	directleft = true;

	//载入各连续移动位图及背景图
//	girl[0] = (HBITMAP)LoadImage(NULL,"girl0.bmp",IMAGE_BITMAP,440,148,LR_LOADFROMFILE);
//	girl[1] = (HBITMAP)LoadImage(NULL,"girl1.bmp",IMAGE_BITMAP,424,154,LR_LOADFROMFILE);
//	girl[2] = (HBITMAP)LoadImage(NULL,"girl2.bmp",IMAGE_BITMAP,480,148,LR_LOADFROMFILE);
//	girl[3] = (HBITMAP)LoadImage(NULL,"girl3.bmp",IMAGE_BITMAP,480,148,LR_LOADFROMFILE);

	//role = (HBITMAP)LoadImage(NULL, "boy.bmp", IMAGE_BITMAP, 189, 251, LR_LOADFROMFILE);
	//bg = (HBITMAP)LoadImage(NULL, "Scene.bmp", IMAGE_BITMAP, 1120, 525, LR_LOADFROMFILE);
	for (int i = 1; i <= 10; i++) {
		char str[5];
		sprintf_s(str, "%d", i);
		char file_path1[50], file_path2[50]; 
		if (i < 10) {
			strcpy_s(file_path1, "res/attack000");
		}
		else {
			strcpy_s(file_path1, "res/attack00");
		}

		strcat_s(file_path1, str); 
		strcat_s(file_path1, ".bmp");
		strcat_s(file_path1, ""); 

		if (i < 10) {
			strcpy_s(file_path2, "res/attackright000");
		}
		else {
			strcpy_s(file_path2, "res/attackright00");
		}

		strcat_s(file_path2, str);
		strcat_s(file_path2, ".bmp");
		strcat_s(file_path2, "");

		
		roleleft[i] = (HBITMAP)LoadImage(NULL, file_path1, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
		roleright[i] = (HBITMAP)LoadImage(NULL, file_path2, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
	}
	scene = (HBITMAP)LoadImage(NULL, "res/map1.bmp", IMAGE_BITMAP, 1320, 640, LR_LOADFROMFILE);
	MyPaint(hdc);

	return TRUE;
}

//****自定义绘图函数****
// 人物贴图坐标修正及窗口贴图
void MyPaint(HDC hdc)
{
	//先在mdc中贴上背景图
	SelectObject(bufdc, scene);
	BitBlt(mdc, 0, 0, 1320, 640, bufdc, 0, 0, SRCCOPY);

	//按照目前的移动方向取出对应人物的连续走动图，并确定截取人物图的宽度与高度
	if (directleft) {
		SelectObject(bufdc, roleleft[num]);
	}
	if (directright) {
		SelectObject(bufdc, roleright[num]);
	}


	//if (mousemove > 0) {
	//	x += stepx;
	//	y += stepy;
	//	mousemove--;
	//	if (mousemove == 0) {
	//		x = targetx;
	//		y = targety;
	//	}
	//}

	//按照目前的X，Y的值在mdc上进行透明贴图，然后显示在窗口画面上
//	BitBlt(mdc,x,y,w,h,bufdc,num*w,dir*h,SRCAND);
//	BitBlt(mdc,x,y,w,h,bufdc,num*w,dir*h,SRCPAINT);
	MyTransparentBlt(mdc, x, y, 150, 150, bufdc, 0, 0, 150, 150, RGB(0, 0, 0));

	BitBlt(hdc, 0, 0, 1320, 640, mdc, 0, 0, SRCCOPY);

	if (jumpnum < 5 && jump && !reach_up) {
		y -= 10;
		jumpnum++;
	} else if (jump){
		reach_up = true;
		y += 10;
		jumpnum--;
	}
	if (jumpnum == 0) {
		jump = false;
		reach_up = false;
	}

	if (moveleft) {
		x -= 10;
	}
	if (moveright) {
		x += 10;
	}
	tPre = GetTickCount();         //记录此次绘图时间
	if (is_attack) {
		num++;
	}	//静止也在动
	if (num == 11) {
		num = 1;
		is_attack = false;
	}
}

//****消息处理函数***********************************
// 1.按下【Esc】键结束程序
// 2.按下方向键重设贴图坐标
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		//targetx = LOWORD(lParam);
		//targety = HIWORD(lParam);
		////			stepx = (int)((targetx - x) / 30.0);
		////			stepy = (int)((targety - y) / 30.0);
		//stepx = (targetx - x) / 20.0;
		//stepy = (targety - y) / 20.0;
		//mousemove = 20;

		//if (abs(stepx) >= abs(stepy)) {
		//	if (stepx < 0) dir = 1;
		//	else if (stepx > 0) dir = 2;
		//}
		//else {
		//	if (stepy < 0) dir = 3;
		//	else if (stepy > 0) dir = 0;
		//}
		break;

	case WM_KEYDOWN:	     //按下键盘消息
		//mousemove = 0;
		////判断按键的虚拟键码
		//switch (wParam)
		//{
		//case VK_ESCAPE:           //按下【Esc】键
		//	PostQuitMessage(0);  //结束程序
		//	break;
		//case VK_UP:				  //按下【↑】键
		//	//先按照目前的移动方向来进行贴图坐标修正，并加入人物往上移动的量（每次按下一次按键移动10个单位），来决定人物贴图坐标的X与Y值，接着判断坐标是否超出窗口区域，若有则再次修正
		//	switch (dir)
		//	{
		//	case 3:	//之前向上
		//		y -= 10;
		//		break;
		//	case 0: //之前向下
		//		x -= 1;
		//		y -= 8;
		//		break;
		//	case 1:	//之前向左
		//		x += 2;
		//		y -= 10;
		//		break;
		//	case 2: //之前向右
		//		x += 2;
		//		y -= 10;
		//		break;
		//	}
		//	if (y < 0) //到顶了
		//		y = 0;
		//	dir = 3;  //改方向
		//	break;
		//case VK_DOWN:			  //按下【↓】键
		//	switch (dir)
		//	{
		//	case 3:
		//		x += 1;
		//		y += 8;
		//		break;
		//	case 0:
		//		y += 10;
		//		break;
		//	case 1:
		//		x += 3;
		//		y += 6;
		//		break;
		//	case 2:
		//		x += 3;
		//		y += 6;
		//		break;
		//	}

		//	if (y > 525 - h - 30)
		//		y = 525 - h - 30;
		//	dir = 0;
		//	break;
		//case VK_LEFT:			  //按下【←】键
		//	switch (dir)
		//	{
		//	case 3:
		//		x -= 12;
		//		break;
		//	case 0:
		//		x -= 13;
		//		y += 4;
		//		break;
		//	case 1:
		//		x -= 10;
		//		break;
		//	case 2:
		//		x -= 10;
		//		break;
		//	}
		//	if (x < 0)
		//		x = 0;
		//	dir = 1;
		//	break;
		//case VK_RIGHT:			   //按下【→】键
		//	switch (dir)
		//	{
		//	case 0:
		//		x += 8;
		//		break;
		//	case 1:
		//		x += 7;
		//		y += 4;
		//		break;
		//	case 2:
		//		x += 10;
		//		break;
		//	case 3:
		//		x += 10;
		//		break;
		//	}
		//	if (x > 1120 - w - 15)
		//		x = 1120 - w - 15;
		//	dir = 2;
		//	break;
		//}
		//break;
	case WM_CHAR:
		if (GetAsyncKeyState('a') || GetAsyncKeyState('A')) {
			
			moveleft = true;
			directleft = true;
			directright = false;
		}
		if (GetAsyncKeyState('d') || GetAsyncKeyState('D')) {
			moveright = true;
			directleft = false;
			directright = true;
		}
		if (GetAsyncKeyState('k') || GetAsyncKeyState('K')) {
			if (!jump) {
				y -= 10;
				jumpnum = 1;
			}
			jump = true;
		}
		if (GetAsyncKeyState('j') || GetAsyncKeyState('J')) {
			is_attack = true;
			num = 1;
		}
		break;
	case WM_KEYUP:
		switch (wParam) {
			case 'A':
				moveleft = false;
				break;
			case 'D':
				moveright = false;
				break;
			break;
		}
	break;
	case WM_DESTROY:			    	//窗口结束消息
		int i;

		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteObject(roleleft);
		DeleteObject(roleright);
		DeleteObject(scene);
		ReleaseDC(hWnd, hdc);

		PostQuitMessage(0);
		break;
	default:							//其他消息
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MyTransparentBlt(HDC hdcDest,      // 目标DC
	int nXOriginDest,   // 目标X偏移
	int nYOriginDest,   // 目标Y偏移
	int nWidthDest,     // 目标宽度
	int nHeightDest,    // 目标高度
	HDC hdcSrc,         // 源DC
	int nXOriginSrc,    // 源X起点
	int nYOriginSrc,    // 源Y起点
	int nWidthSrc,      // 源宽度
	int nHeightSrc,     // 源高度
	UINT crTransparent  // 透明色,COLORREF类型
)
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// 创建兼容位图
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// 创建单色掩码位图
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

	// 将源DC中的位图拷贝到临时DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,
			hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// 设置透明色
	SetBkColor(hImageDC, crTransparent);

	// 生成透明区域为白色，其它区域为黑色的掩码位图
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// 生成透明区域为黑色，其它区域保持不变的位图
	SetBkColor(hImageDC, RGB(0, 0, 0));
	SetTextColor(hImageDC, RGB(255, 255, 255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// 透明部分保持屏幕不变，其它部分变成黑色
	SetBkColor(hdcDest, RGB(0xff, 0xff, 0xff));
	SetTextColor(hdcDest, RGB(0, 0, 0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// "或"运算,生成最终效果
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

void PrintBitmap(HDC hdc, int x, int y, const char* bmpFilePath) {
	// Load the bitmap from file
	HBITMAP hBitmap = (HBITMAP)LoadImage(
		NULL,
		bmpFilePath,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);

	if (hBitmap) {
		// Create a memory device context compatible with the specified device context
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		// Get the bitmap's dimensions
		BITMAP bitmap;
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		// Copy the bitmap from the memory device context to the specified device context
		BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);

		// Restore the old bitmap and delete the memory device context
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);

		// Delete the loaded bitmap
		DeleteObject(hBitmap);
	}
	else {
		// Handle the error if the bitmap could not be loaded
		MessageBox(NULL, "Failed to load bitmap", "Error", MB_OK | MB_ICONERROR);
	}
}
