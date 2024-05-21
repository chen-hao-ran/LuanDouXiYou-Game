#pragma once

//#include "resource.h"
#include <windows.h>
#include <stdio.h>
#include "Tools.h"

#define MAINSCENE 10

bool keyA;
bool keyD;
bool keyS;
bool keyW;
bool keyJ;
bool keyK;

typedef struct WindowConfig {
	HDC	hdc; //主绘布
	HDC	mdc; //缓冲绘布
	HDC	bufdc; //角色和场景加载绘布
	HWND	hwnd; //窗体
	DWORD	tPre, tNow; //游戏加载时间
	DWORD	interval;
	HBITMAP fullmap; //虚拟位图，用于mdc
	RECT rect; //实际窗体大小，包括顶栏
	int w; //窗体宽度
	int h; //窗体高度
	
	WindowConfig();
	WindowConfig(int _w, int _h, int _interval);
	void InitializeDC(); //初始化DC
	void MergeDC(); //合并DC
};


typedef struct Role{
	int blood; //血量
	int blood_up; //血量上限
	int mana; //魔力值
	int mana_up; //魔力上限
	int empiric; //经验值
	int empiric_up; //升级所需经验值
	int level; //等级
	int x; //位置
	int y; //位置
	int oriention; //方向
	bool is_jump; //是否跳跃
	bool is_reachup; //跳跃是否到顶
	bool is_dead; //是否死亡

	bool is_skill; //是否释放技能
	bool is_attack; //是否普通攻击

	//背包

	//穿戴武器

	Role();
	Role(
		int _blood_up,
		int _mana_up,
		int _empiric_up,
		int _x,
		int _y,
		int _oriention
	);
	void Injuried(); //受伤
	void Dead(); //死亡
	void Destroy(); //销毁角色
	void LevelUp(); //升级
	void ChangeWeapon(int id); //更换武器
	void Move(int x, int y); //移动

};

typedef struct Scene {
	HBITMAP background; //背景
	HBITMAP guild;
	int x; //加载起始坐标
	int y; //加载起始坐标
	int w; //加载宽度
	int h; //加载高度
	char background_path[50]; //背景路径
	char guild_path[50];
		//BGM
	RECT click_area;

	Scene();
	Scene(int _w, int _h, const char* _background_path, int _left, int _top, int _right, int _bottom, const char* _guild_path);
	void LoadScene(WindowConfig& wndconf); //加载场景
	void Change(WindowConfig& wndconf, int step); //场景变化
	void Destroy(); //销毁场景
};

typedef struct MainScene {
	WindowConfig& wndconf; //主窗体结构
	Role role; //角色
	Scene* scenes; //场景
	HBITMAP background; //背景
	int x; //加载起始坐标
	int y; //加载起始坐标
	int w; //加载宽度
	int h; //加载高度
	char file_path[50]; //位图路径
	int scene_id; //当前所在场景id

	//BGM

	MainScene(WindowConfig& _wndconf, Scene* _scenes, int _x, int _y, int _w, int _h, const char* _file_path);
	void LoadMainScene(); //加载场景
	void Change(); //场景变化
	void Play(); //关卡运行
	void Choose(int id);
	void Destroy(); //销毁场景
};

//------------------------------------------

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

//-------------------------------------------

MainScene::MainScene(WindowConfig& _wndconf, Scene* _scenes, int _x, int _y, int _w, int _h, const char* _file_path) : wndconf(_wndconf), scenes(_scenes) {
	//wndconf = _wndconf; 必须在前面初始化，而不是赋值
	//scenes = _scenes; 
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	scene_id = MAINSCENE;
	strcpy_s(file_path, _file_path);
	background = (HBITMAP)LoadImage(NULL, file_path, IMAGE_BITMAP, w, h, LR_LOADFROMFILE);
}

void MainScene::LoadMainScene() {
	//绘制背景
	SelectObject(wndconf.bufdc, background);
	BitBlt(wndconf.mdc, 0, 0, w, h, wndconf.bufdc, x, y, SRCCOPY); 

	//绘制关卡按钮
	for (int i = 0; i < 3; i++) {
		SelectObject(wndconf.bufdc, scenes[i].guild);	
		MyTransparentBlt(wndconf.mdc, scenes[i].click_area.left, scenes[i].click_area.top, scenes[i].click_area.right - scenes[i].click_area.left, scenes[i].click_area.bottom - scenes[i].click_area.top,
					     wndconf.bufdc, 0, 0, scenes[i].click_area.right - scenes[i].click_area.left, scenes[i].click_area.bottom - scenes[i].click_area.top,
						 RGB(0, 0, 0));
	}
}

void MainScene::Play() {
	if (scene_id == MAINSCENE) {
		
	}
	else {
		if (keyA) {
			scenes[scene_id].Change(wndconf, -10);
		}
		if (keyD) {
			scenes[scene_id].Change(wndconf, 10);
		}
		scenes[scene_id].LoadScene(wndconf);
	}

	wndconf.tPre = GetTickCount();
}

void MainScene::Choose(int id) {
	scene_id = id;
}

//-------------------------------------

Scene::Scene() {
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	click_area = { 0, 0, 0, 0 };
	strcpy_s(background_path, "");
	strcpy_s(guild_path, "");
}

Scene::Scene(int _w, int _h, const char* _background_path, int _left, int _top, int _right, int _bottom, const char* _guild_path) {
	x = 0;
	y = 0;
	w = _w;
	h = _h;
	click_area = { _left, _top, _right, _bottom };
	strcpy_s(background_path, _background_path);
	strcpy_s(guild_path, _guild_path);
	background = (HBITMAP)LoadImage(NULL, background_path, IMAGE_BITMAP, w, h, LR_LOADFROMFILE);
	guild = (HBITMAP)LoadImage(NULL, guild_path, IMAGE_BITMAP, click_area.right - click_area.left, click_area.bottom - click_area.top, LR_LOADFROMFILE);
}

void Scene::LoadScene(WindowConfig& wndconf) {
	SelectObject(wndconf.bufdc, background);
	BitBlt(wndconf.mdc, 0, 0, wndconf.w, wndconf.h, wndconf.bufdc, x, y, SRCCOPY);
}

void Scene::Change(WindowConfig& wndconf, int step) {
	x += step;
	SelectObject(wndconf.bufdc, background);
	BitBlt(wndconf.mdc, 0, 0, wndconf.w, wndconf.h, wndconf.bufdc, x, y, SRCCOPY);
}

//-------------------------------------

Role::Role(){
	blood = 0;
	blood_up = 0;
	mana = 0;
	mana_up = 0;
	empiric = 0;
	empiric_up = 0;
	level = 0;
	x = 0;
	y = 0;
	oriention = 0;
	is_jump = false;
	is_reachup = false;
	is_dead = false;
	is_skill = false;
	is_attack = false;
}

Role::Role(
	int _blood_up,
	int _mana_up,
	int _empiric_up,
	int _x,
	int _y,
	int _oriention
) {
	blood = 0;
	blood_up = _blood_up;
	mana = 0;
	mana_up = _mana_up;
	empiric = 0;
	empiric_up = _empiric_up;
	level = 0;
	x = _x;
	y = _y;
	oriention = _oriention;
	is_jump = false;
	is_reachup = false;
	is_dead = false;
	is_skill = false;
	is_attack = false;
}