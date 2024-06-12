
/*
	项目逻辑处理文件
*/

//#include "LuanDouXiyou.h"
#include "WindowConfig.h"
#include "MainScene.h"
#include "Scene.h"
#include "Role.h"
#include "Enemy.h"
#include "Boss.h"
#include "Replay.h"
#include "Tools.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>

//全局函数声明
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//变量初始化
//窗体
WindowConfig wndconf(1100, 640, 40);
//场景
Scene scenes[3] = {
	{3633, 640, "res/scene/scene0001.bmp", 200, 200, 200 + 272, 200 + 118, "res/scene/guild0001.bmp", 435},
	{3960, 640, "res/scene/scene0002.bmp", 400, 300, 400 + 159, 300 + 183, "res/scene/guild0002.bmp", 450},
	{3762, 640, "res/scene/scene0003.bmp", 600, 200, 600 + 148, 200 + 146, "res/scene/guild0003.bmp", 510}
};
//角色
Role role(
		500, 
		500, 
		500, 
		20, 
		500, 
		RIGHT, 
		10, //伤害
		"res/Sun/attack/", 
		"res/Sun/move/move.bmp", 
		"res/Sun/run/run.bmp", 
		"res/Sun/injuried/injuried.bmp",
		"res/Sun/1skill/1skill.bmp",
		"res/Sun/2skill/2skill.bmp",
		"res/Sun/3skill/3skill.bmp"
);
//小怪
Enemy enemies[LEVEL_MAXNUM][ENEMIES_MAXNUM];
//Boss
Boss boss[LEVEL_MAXNUM] = {
	{   
		500, //blood
		500, //x
		375,  //y
		RIGHT, // orientation
		50,  //攻击范围
		5, //伤害值 
		"res/Ju/attack/attack.bmp",
		"res/Ju/move/move.bmp",
		"res/Ju/injuried/injuried.bmp"
	},
	{
		500, //blood
		1000, //x
		385,  //y
		RIGHT, // orientation
		50,  //攻击范围
		5, //伤害值 
		"res/Ying/attack/attack.bmp",
		"res/Ying/move/move.bmp",
		"res/Ying/injuried/injuried.bmp"
	},
	{		
		500, //blood
		1000, //x
		430,  //y
		RIGHT, // orientation
		50,  //攻击范围
		5, //伤害值 
		"res/Ying/attack/attack.bmp",
		"res/Ying/move/move.bmp",
		"res/Ying/injuried/injuried.bmp"}
};
//UI
UI ui = { 
	"res/UI/state.bmp", //状态栏位图路径
	"res/UI/boss_state.bmp", //boss血条路径
	"res/UI/skill_state.bmp", //技能栏路径
	"res/UI/win.bmp", //通关传送门位图路径
	"res/UI/replay.bmp" //回放按钮位图路径
};
//主场景
MainScene mscene = {
					wndconf,
					scenes,
					role,
					enemies,
					boss,
					ui, 
					0, 
					0, 
					1100, 
					640, 
					"res/mainscene/main_scene.bmp" ,
					"res/mainscene/failed.bmp"
};

//回放
Replay replay[HANDLE_MAXNUM];

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
		if (mscene.is_replay) {
			if (GetTickCount() - mscene.replay_start_time >= replay[mscene.now_handle].time - mscene.level_start_time) {
				SendMessage(
					wndconf.hwnd,
					(replay[mscene.now_handle].type == 1) ? WM_KEYDOWN : WM_KEYUP,
					replay[mscene.now_handle].key,
					0
				);
				mscene.now_handle++;
				Sleep(1);

			}
		}
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

	//初始化变量
	for (int i = 0; i < ENEMIES_MAXNUM; i++) {
		enemies[0][i] = { 
			500, //blood
			500 + i * 100, //x
			500,  //y
			RIGHT, // orientation
			50,  //攻击范围
			2, //伤害值 
			1, 
			"res/Shi/attack/attack.bmp", 
			"res/Shi/move/move.bmp", 
			"res/Shi/injuried/injuried.bmp"
		};
		enemies[1][i] = { 
			500, //blood
			500 + i * 100, //x
			500,  //y
			RIGHT, // orientation
			50,  //攻击范围
			2, //伤害值 
			2, 
			"res/Mo/attack/attack.bmp", 
			"res/Mo/move/move.bmp", 
			"res/Mo/injuried/injuried.bmp"
		};
		enemies[2][i] = {
			500, //blood
			500 + i * 100, //x
			500,  //y
			RIGHT, // orientation
			50,  //攻击范围
			10, //伤害值 
			2, 
			"res/Mo/attack/attack.bmp",
			"res/Mo/move/move.bmp",
			"res/Mo/injuried/injuried.bmp"
		};
	}

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
	DWORD current_time = GetTickCount();
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (mscene.scene_id == MAINSCENE) {
			if (x >= mscene.scenes[0].click_area.left 
				&& x <= mscene.scenes[0].click_area.right 
				&& y >= mscene.scenes[0].click_area.top 
				&& y <= mscene.scenes[0].click_area.bottom) {
				mscene.Choose(0);
				//char str[100];
				//sprintf_s(str, "%d", mscene.scene_id);
				//MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
			if (x >= mscene.scenes[1].click_area.left
				&& x <= mscene.scenes[1].click_area.right
				&& y >= mscene.scenes[1].click_area.top
				&& y <= mscene.scenes[1].click_area.bottom) {
				mscene.Choose(1);
				//char str[100];
				//sprintf_s(str, "%d", mscene.scene_id);
				//MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
			if (x >= mscene.scenes[2].click_area.left
				&& x <= mscene.scenes[2].click_area.right
				&& y >= mscene.scenes[2].click_area.top
				&& y <= mscene.scenes[2].click_area.bottom) {
				mscene.Choose(2);
				//char str[100];
				//sprintf_s(str, "%d", mscene.scene_id);
				//MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
			//if (x >= 800
			//	&& x <= 1000
			//	&& y >= 200
			//	&& y <= 400) {
			//	char str[50];
			//	sprintf_s(str, "start, %d", mscene.now_handle);
			//	MessageBox(wndconf.hwnd, str, "", 1);
			//	mscene.is_replay = true;
			//	mscene.now_handle = 0;
			//	mscene.Choose(mscene.backup_scene_id);
			//	mscene.replay_start_time = current_time;
			//	//char str[50];
			//	//sprintf_s(str, "%d, %d, %d, %d", GetTickCount(), mscene.replay_start_time, replay[mscene.now_handle].time, mscene.level_start_time);
			//	//MessageBox(wndconf.hwnd, str, "", 1);
			//}
		}
		else if (mscene.scene_id == FAILED) {
			if (x >= 380
				&& x <= 540
				&& y >= 440
				&& y <= 480) {
				mscene.Choose(mscene.backup_scene_id);
				//char str[100];
				//sprintf_s(str, "%d", mscene.backup_scene_id);
				//MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
			if (x >= 560
				&& x <= 740
				&& y >= 440
				&& y <= 480) {
				mscene.scene_id = MAINSCENE;
				mscene.LoadMainScene();
				//char str[100];
				//sprintf_s(str, "主界面");
				//MessageBox(hwnd, str, "Message Box", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
			case 'A':
				if (current_time - keyA_last <= 100) {
					keyA_double = true;
					keyA = false;
				}
				else {
					keyA = true;
				}
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'A';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 1;
				break;
			case 'D':
				if (current_time - keyD_last <= 100) {
					keyD_double = true;
					keyD = false;
				}
				else {
					keyD = true;
				}
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'D';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 1;
				break;
			case 'K':
				keyK = true;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'K';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 1;
				break;
			case 'J':
				keyJ = true;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'J';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 1;
				break;
			case 'Y':
				keyY = true;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'Y';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 1;
				break;
			case 'L':
				keyL = true;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'L';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 1;
				break;
			case 'U':
				keyU = true;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'U';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 1;
				break;
			case 'W':
				if (mscene.win && role.x >= 700 && role.x <= 900) {
					if (mscene.is_replay) break;
					replay[mscene.now_handle].key = 'W';
					replay[mscene.now_handle].time = current_time;
					replay[mscene.now_handle++].type = 1;

					mscene.backup_scene_id = mscene.scene_id;
					mscene.scene_id = MAINSCENE;
					mscene.LoadMainScene();
					role.x = 0;
					role.center_x = role.x + 40;
				}
				break;
			case 'P':
				char str[50];
				sprintf_s(str, "start, %d", mscene.now_handle);
				MessageBox(wndconf.hwnd, str, "", 1);
				mscene.is_replay = true;
				mscene.now_handle = 0;
				mscene.Choose(mscene.scene_id);
				mscene.replay_start_time = current_time;

				break;
			case VK_ESCAPE: //按esc退出
				PostQuitMessage(0);
				break;
		}
		break;
	case WM_KEYUP:
		switch (wParam) {
			case 'A':
				keyA = false;
				keyA_double = false;
				keyA_last = current_time;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'A';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 2;
				break;
			case 'D':
				keyD = false;
				keyD_double = false;
				keyD_last = current_time;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'D';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 2;
				break;
			case 'K':
				keyK = false;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'K';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 2;
				break;
			case 'J':
				keyJ = false;
				keyJ_last = current_time;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'J';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 2;
				break;
			case 'Y':
				keyY = false;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'Y';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 2;
				break;
			case 'L':
				keyL = false;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'L';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 2;
				break;
			case 'U':
				keyU = false;
				if (mscene.is_replay) break;
				replay[mscene.now_handle].key = 'U';
				replay[mscene.now_handle].time = current_time;
				replay[mscene.now_handle++].type = 2;
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

