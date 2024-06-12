#pragma once

//#include "resource.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "Tools.h"

#define MAINSCENE 10
#define LEFT 18
#define RIGHT 19
#define MOVE 20
#define RUN 21
#define ATTACK 22
#define STOP 23

bool keyA;
bool keyD;
bool keyA_double;
bool keyD_double;
DWORD keyA_last;
DWORD keyD_last;
bool keyS;
bool keyW;
bool keyJ;
DWORD keyJ_last;
bool keyK;

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


typedef struct Role{
	HBITMAP attack[3]; //攻击状态位图
	HBITMAP move; //行走状态位图
	HBITMAP run; //奔跑状态位图
	int blood; //血量
	int blood_up; //血量上限
	int mana; //魔力值
	int mana_up; //魔力上限
	int empiric; //经验值
	int empiric_up; //升级所需经验值
	int level; //等级
	int x; //位置
	int y; //位置
	int v; //移动速度
	int orientation; //方向
	int state; // 移动状态
	bool is_jump; //是否跳跃
	bool is_reachup; //跳跃是否到顶
	int jump_num; // 跳跃帧数
	bool is_dead; //是否死亡

	bool is_skill; //是否释放技能
	bool is_attack; //是否普通攻击

	char attack_path[50]; //攻击位图路径
	char move_path[50]; //行走位图路径
	char run_path[50]; //奔跑位图路径
	int attack_stage; //普通攻击阶段
	int attack_num; //普通攻击加载状态
	int move_num; //移动加载状态
	int run_num; //奔跑加载状态

	Role();
	Role(
		int _blood_up,
		int _mana_up,
		int _empiric_up,
		int _x,
		int _y,
		int _orientation,
		const char* _attack_path,
		const char* _move_path,
		const char* _run_path
	);
	void LoadRole(WindowConfig& wndconf, int state); //加载人物
	void Injuried(); //受伤
	void Dead(); //死亡
	void Destroy(); //销毁角色
	void LevelUp(); //升级
	void Move(int stepx, int move_orientation); //移动
	void Run(int stepx, int move_orientation); //奔跑
	void Jump(int stepy); //跳跃
};

typedef struct Enemy {
	HBITMAP attack; //攻击状态位图
	HBITMAP move; //行走状态位图
	HBITMAP run; //奔跑状态位图
	int blood; //血量
	int blood_up; //血量上限
	int x; //位置
	int y; //位置
	int orientation; //方向
	int state; // 移动状态
	bool is_dead; //是否死亡

	bool is_attack; //是否普通攻击
	bool cooling; //处于冷却状态
	DWORD attack_last; //上次攻击时间

	char attack_path[50]; //攻击位图路径
	char move_path[50]; //行走位图路径
	int attack_stage; //普通攻击阶段
	int attack_num; //普通攻击加载状态
	int move_num; //移动加载状态

	Enemy();
	Enemy(
		int _blood_up,
		int _x,
		int _y,
		int _orientation,
		const char* _attack_path,
		const char* _move_path
	);
	void LoadEnemy(WindowConfig& wndconf, int state); //加载小怪
	void Injuried(); //受伤
	void Dead(); //死亡
	void Destroy(); //销毁小怪
	void Move(int stepx, int move_orientation); //移动
};

typedef struct Scene {
	HBITMAP background; //背景
	HBITMAP guild;
	int x; //加载起始坐标
	int y; //加载起始坐标
	int v; //场景移动速度
	int w; //加载宽度
	int h; //加载高度
	bool lock[3]; //场景是否能够同行
	char background_path[50]; //背景路径
	char guild_path[50]; //图标路径
		//BGM
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

typedef struct MainScene {
	WindowConfig& wndconf; //主窗体结构
	Role& role; //角色
	Enemy* enemies; //小怪
	Scene* scenes; //场景
	HBITMAP background; //背景
	int x; //加载起始坐标
	int y; //加载起始坐标
	int w; //加载宽度
	int h; //加载高度
	char file_path[50]; //位图路径
	int scene_id; //当前所在场景id

	MainScene(WindowConfig& _wndconf, Scene* _scenes, Role& _role, Enemy* _enemies, int _x, int _y, int _w, int _h, const char* _file_path);
	void LoadMainScene(); //加载场景
	void Change(); //场景变化
	void Play(); //关卡运行
	void Choose(int id); //切换关卡
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

MainScene::MainScene(WindowConfig& _wndconf, Scene* _scenes, Role& _role, Enemy* _enemies, int _x, int _y, int _w, int _h, const char* _file_path) : wndconf(_wndconf), scenes(_scenes), role(_role), enemies(_enemies){
	//wndconf = _wndconf; 必须在前面初始化，而不是赋值
	//scenes = _scenes; 
	//role = _role;
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
	//当前时间
	DWORD time_now = GetTickCount();

	//当前场景
	Scene& now_scene = scenes[scene_id];

	//保证行走不出循环
	role.move_num %= 7;
	enemies[scene_id].move_num %= 7;

	if (scene_id == MAINSCENE) {
		
	}
	else {
		scenes[scene_id].LoadScene(wndconf);

		if (keyJ) {
			//判断是否进入下一阶段
			if (time_now - keyJ_last <= 1000 && role.is_attack == false && keyJ_last != 0) {
				role.attack_stage = (role.attack_stage + 1) % 3;
			}
			else if (time_now - keyJ_last > 1000){
				role.attack_stage = 0;
			}

			if (!role.is_attack && role.orientation == LEFT) {
				role.x -= 60;
			}
			role.is_attack = true;

			scenes[scene_id].v = 0;
		}
		else if (!role.is_attack) {
			if (keyA || keyA_double || keyD || keyD_double) {
				int scene_stage = now_scene.x / (now_scene.w / 3); //关卡中角色正处于的场景编号
				if (keyA_double) {        //向左奔跑
					//scenes[scene_id].v = 10;
					//if (scenes[scene_id].x >= scenes[scene_id].v) {
					//	scenes[scene_id].Change(wndconf, -scenes[scene_id].v);
					//}
					//else {
					//	scenes[scene_id].v = 0;
					//}
					//role.Move(5, LEFT);
					//role.orientation = LEFT;
					//role.move_num++;

					////修改人物移动状态
					//role.state = RUN;

					now_scene.v = 15;
					role.v = 15;
					role.orientation = LEFT;
					role.state = RUN;
					role.move_num++;

					if (!now_scene.lock[scene_stage] && role.x >= now_scene.w / 3 / 4) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage]) {
						role.v = 0;
					}
					scenes[scene_id].Change(wndconf, -scenes[scene_id].v);
				}
				else if (keyD_double) {    //向右奔跑
					//scenes[scene_id].v = 10;
					//if (scenes[scene_id].x + 1310 <= scenes[scene_id].w) {
					//	scenes[scene_id].Change(wndconf, scenes[scene_id].v);
					//}
					//else {
					//	scenes[scene_id].v = 0;
					//}
					//role.Move(5, RIGHT);
					//role.orientation = RIGHT;
					//role.move_num++;

					////修改人物移动状态
					//role.state = RUN;
					now_scene.v = 15;
					role.v = 15;
					role.orientation = RIGHT;
					role.state = RUN;
					role.move_num++;

					if (!now_scene.lock[scene_stage] && role.x <= now_scene.w / 3 / 4 * 3) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage]) {
						role.v = 0;
					}
					scenes[scene_id].Change(wndconf, scenes[scene_id].v);
				}
				else if (keyA) {       //向左行走
					//scenes[scene_id].v = 5;
					//if (scenes[scene_id].x >= scenes[scene_id].v) {
					//	scenes[scene_id].Change(wndconf, -scenes[scene_id].v);
					//}
					//else {
					//	scenes[scene_id].v = 0;
					//}
					//role.Move(2, LEFT);
					//role.orientation = LEFT;
					//role.move_num++;

					////修改人物移动状态
					//role.state = MOVE;
					now_scene.v = 10;
					role.v = 10;
					role.orientation = LEFT;
					role.state = MOVE;
					role.move_num++;

					if (!now_scene.lock[scene_stage] && role.x >= now_scene.w / 3 / 4) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage]) {
						role.v = 0;
					}
					scenes[scene_id].Change(wndconf, -scenes[scene_id].v);
				}
				else if (keyD) {      //向右行走
					//scenes[scene_id].v = 5;
					//if (scenes[scene_id].x + 1310 <= scenes[scene_id].w) {
					//	scenes[scene_id].Change(wndconf, scenes[scene_id].v);
					//}
					//else {
					//	scenes[scene_id].v = 0;
					//}
					//role.Move(2, RIGHT);
					//role.orientation = RIGHT;
					//role.move_num++;

					////修改人物移动状态
					//role.state = MOVE;
					now_scene.v = 10;
					role.v = 10;
					role.orientation = RIGHT;
					role.state =MOVE;
					role.move_num++;

					if (!now_scene.lock[scene_stage] && role.x <= now_scene.w / 3 / 4 * 3) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage]) {
						role.v = 0;
					}
					scenes[scene_id].Change(wndconf, scenes[scene_id].v);
				}
				role.Move(role.v, role.orientation);
			}
			else if (!role.is_jump && !role.is_attack) {               //停止
				role.move_num = 0;
				role.state = STOP;
				role.v = 0;
				scenes[scene_id].v = 0;
			}
		}
		
		if (keyK) {
			role.is_jump = true;
		}

		// 跳跃判定
		if (!role.is_reachup && role.is_jump) {
			role.jump_num++;
			role.Jump(-5 * (8 - role.jump_num));

			if (role.jump_num == 8) {
				role.is_reachup = true;
			}
		}
		else if (role.is_jump) {
			role.jump_num--;
			role.Jump(5 * (7 - role.jump_num));
			if (role.jump_num == 0) {
				role.is_jump = false;
				role.is_reachup = false;
			}
		}

		if (role.is_attack) {
			role.move_num = 0; //攻击时不能移动
			role.state = ATTACK;

			role.attack_num++; //继续更新攻击动作
			if (role.attack_stage != 2) {
				if (role.attack_num == 8) {
					role.attack_num = 0;
					role.is_attack = false;
					role.state = STOP;
					if (role.orientation == LEFT) {
						role.x += 60;
					}
				}
			}
			else {
				if (role.attack_num == 10) {
					role.attack_num = 0;
					role.is_attack = false;
					role.state = STOP;
					if (role.orientation == LEFT) {
						role.x += 60;
					}
				}
			}

		}

		//将背景的移动速度抵消
		enemies[scene_id].Move((role.orientation == LEFT) ? -scenes[scene_id].v : scenes[scene_id].v, LEFT);
		//小怪移动状态判定
		//攻击冷却重置
		if (time_now - enemies[scene_id].attack_last >= 1000) {
			enemies[scene_id].cooling = false;
		}

		if (role.x + 100 <= enemies[scene_id].x) {
			enemies[scene_id].Move(4, LEFT);
			enemies[scene_id].orientation = LEFT;
			enemies[scene_id].move_num++;
			enemies[scene_id].state = MOVE;
		} 
		else if (role.x > enemies[scene_id].x + 100) {
			enemies[scene_id].Move(4, RIGHT);
			enemies[scene_id].orientation = RIGHT;
			enemies[scene_id].move_num++;
			enemies[scene_id].state = MOVE;
		}
		else if (!enemies[scene_id].cooling){
			enemies[scene_id].state = ATTACK;
			enemies[scene_id].is_attack = true;
			enemies[scene_id].attack_last = time_now;
			enemies[scene_id].move_num = 0; //攻击时不能移动
			enemies[scene_id].attack_num++;  
			enemies[scene_id].x += (enemies[scene_id].orientation == LEFT) ? -4 : 4; 

			//攻击完成后停止,不再循环
			if (enemies[scene_id].attack_num == 12) {
				enemies[scene_id].attack_num = 0;
				enemies[scene_id].is_attack = false;
				enemies[scene_id].cooling = true;
				enemies[scene_id].state = STOP;
				enemies[scene_id].x -= (enemies[scene_id].orientation == LEFT) ? -4 * 12 : 4 * 12;
			}
		}
		else {
			enemies[scene_id].state = STOP;
			enemies[scene_id].move_num = 0;
		}

		//加载人物
		role.LoadRole(wndconf, role.state);

		//加载小怪
		enemies[scene_id].LoadEnemy(wndconf, enemies[scene_id].state);
	}

	wndconf.tPre = GetTickCount();
}

void MainScene::Choose(int id) {
	scene_id = id;
	role.y = scenes[scene_id].horizon;
	enemies[scene_id].y = scenes[scene_id].horizon;
}

//-------------------------------------

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
	lock[0] = lock[1] = lock[2] = false;
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
	v = 0;
	orientation = 0;
	state = STOP;
	is_jump = false;
	is_reachup = false;
	jump_num = 0;
	is_dead = false;
	is_skill = false;
	is_attack = false;
	strcpy_s(attack_path, "");
	strcpy_s(move_path, "");
	strcpy_s(run_path, "");
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;
	run_num = 0;
}

Role::Role(

	int _blood_up,
	int _mana_up,
	int _empiric_up,
	int _x,
	int _y,
	int _orientation,
	const char* _attack_path,
	const char* _move_path,
	const char* _run_path
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
	v = 0;
	orientation = _orientation;
	state = STOP;
	is_jump = false;
	is_reachup = false;
	jump_num = 0;
	is_dead = false;
	is_skill = false;
	is_attack = false;

	strcpy_s(attack_path, _attack_path);
	strcpy_s(move_path, _move_path);
	strcpy_s(run_path, _run_path);
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;
	run_num = 0;

	move = (HBITMAP)LoadImage(NULL, move_path, IMAGE_BITMAP, 800, 260, LR_LOADFROMFILE);
	run = (HBITMAP)LoadImage(NULL, run_path, IMAGE_BITMAP, 800, 260, LR_LOADFROMFILE);
	char str[] = "attack.bmp";
	for (int i = 0; i < 3; i++) {
		char path[50];
		sprintf_s(path, "%s%d%s", attack_path, i + 1, str);
		
		if (i == 2) {
			attack[i] = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 1500, 260, LR_LOADFROMFILE);
		}
		else {
			attack[i] = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 1200, 260, LR_LOADFROMFILE);
		}
	}
}

void Role::LoadRole(WindowConfig& wndconf, int state) {
	//跳跃攻击状态
	if (is_jump && state == ATTACK) {
		SelectObject(wndconf.bufdc, attack[attack_stage]);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 0, 150, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 130, 150, 130, RGB(0, 0, 0));
		}
	}
	//跳跃状态
	else if (is_jump) {
		SelectObject(wndconf.bufdc, run);
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 4 * 100, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 4 * 100, 130, 100, 130, RGB(0, 0, 0));
		}
		return;
	}
	//行走状态
	else if (state == MOVE) {
		SelectObject(wndconf.bufdc, move);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 130, 100, 130, RGB(0, 0, 0));
		}
	}
	//奔跑状态
	else if (state == RUN) {
		SelectObject(wndconf.bufdc, run);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 130, 100, 130, RGB(0, 0, 0));
		}
	}
	//攻击状态
	else if (state == ATTACK) {
		SelectObject(wndconf.bufdc, attack[attack_stage]);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 0, 150, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 130, 150, 130, RGB(0, 0, 0));
		}
	}
	//停止状态
	else if (state == STOP) {
		SelectObject(wndconf.bufdc, move);
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 130, 100, 130, RGB(0, 0, 0));
		}
		return;
	}

}

void Role::Move(int stepx, int move_orientation) {
	if (move_orientation == LEFT) x -= stepx;
	else x += stepx;

	x = min(x, 1220);
	x = max(x, 0);
}

void Role::Jump(int stepy) {
	y += stepy;
}

//----------------------------------------------

Enemy::Enemy() {
	blood = 0;
	blood_up = 0;
	x = 0;
	y = 0;
	orientation = 0;
	state = STOP;
	is_dead = false;
	is_attack = false;
	cooling = false;
	attack_last = 0;
	strcpy_s(attack_path, "");
	strcpy_s(move_path, "");
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;
}

Enemy::Enemy(

	int _blood_up,
	int _x,
	int _y,
	int _orientation,
	const char* _attack_path,
	const char* _move_path
) {
	blood = 0;
	blood_up = _blood_up;
	x = _x;
	y = _y;
	orientation = _orientation;
	state = STOP;
	is_dead = false;
	is_attack = false;
	cooling = false;
	attack_last = 0;

	strcpy_s(attack_path, _attack_path);
	strcpy_s(move_path, _move_path);
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;

	move = (HBITMAP)LoadImage(NULL, move_path, IMAGE_BITMAP, 800, 260, LR_LOADFROMFILE);
	attack = (HBITMAP)LoadImage(NULL, attack_path, IMAGE_BITMAP, 1320, 200, LR_LOADFROMFILE);
}

void Enemy::LoadEnemy(WindowConfig& wndconf, int state) {
	//行走状态
	if (state == MOVE) {
		SelectObject(wndconf.bufdc, move);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 130, 100, 130, RGB(0, 0, 0));
		}
	}
	//奔跑状态
	else if (state == RUN) {
		SelectObject(wndconf.bufdc, run);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 130, 100, 130, RGB(0, 0, 0));
		}
	}
	//攻击状态
	else if (state == ATTACK) {
		SelectObject(wndconf.bufdc, attack);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 110, 100, wndconf.bufdc, attack_num * 110, 0, 110, 100, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 110, 100, wndconf.bufdc, attack_num * 110, 100, 110, 100, RGB(0, 0, 0));
		}
	}
	//停止状态
	else if (state == STOP) {
		SelectObject(wndconf.bufdc, move);
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 130, 100, 130, RGB(0, 0, 0));
		}
		return;
	}

}

void Enemy::Move(int stepx, int move_orientation) {
	if (move_orientation == LEFT) x -= stepx;
	else x += stepx;
}
