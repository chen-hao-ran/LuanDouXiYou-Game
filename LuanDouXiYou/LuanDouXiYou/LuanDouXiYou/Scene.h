#pragma once

#include "Common.h"
#include "WindowConfig.h"

typedef struct Scene {
	HBITMAP background; //背景
	HBITMAP guild;
	int x; //加载起始坐标
	int y; //加载起始坐标
	int v; //场景移动速度
	int w; //加载宽度
	int h; //加载高度
	bool lock[3]; //场景是否能够通行
	char background_path[50]; //背景路径
	char guild_path[50]; //图标路径
	RECT click_area;
	int horizon; //地面高度

	Scene();
	Scene(int _w,
		int _h,
		const char* _background_path,
		int _left,
		int _top,
		int _right,
		int _bottom,
		const char* _guild_path,
		int _horizon);
	void LoadScene(WindowConfig& wndconf); //加载场景
	void Change(WindowConfig& wndconf, int step); //场景变化
	void Destroy(); //销毁场景
};

Scene::Scene() {
	x = 0;
	y = 0;
	v = 0;
	w = 0;
	h = 0;
	lock[0] = lock[1] = lock[2] = true;
	click_area = { 0, 0, 0, 0 };
	strcpy_s(background_path, "");
	strcpy_s(guild_path, "");
}

Scene::Scene(int _w,
	int _h,
	const char* _background_path,
	int _left,
	int _top,
	int _right,
	int _bottom,
	const char* _guild_path,
	int _horizon) {
	x = 0;
	y = 0;
	v = 0;
	w = _w;
	h = _h;
	lock[0] = lock[1] = lock[2] = true;
	horizon = _horizon;
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