#pragma once

#include "Common.h"
#include "WindowConfig.h"

typedef struct Enemy {
	HBITMAP attack; //攻击状态位图
	HBITMAP move; //行走状态位图
	HBITMAP run; //奔跑状态位图
	HBITMAP injuried; //受伤状态位图
	int blood; //血量
	int blood_up; //血量上限
	int x; //位置
	int center_x; //小怪中心位置
	int y; //位置
	int orientation; //方向
	int state; // 移动状态
	bool is_dead; //是否死亡

	bool is_attack; //是否普通攻击
	bool cooling; //处于冷却状态
	DWORD attack_last; //上次攻击时间
	int attack_scale; //攻击范围
	int damage; //伤害

	char attack_path[50]; //攻击位图路径
	char move_path[50]; //移动位图路径
	char injuried_path[50]; //受伤位图路径
	int attack_stage; //普通攻击阶段
	int attack_num; //普通攻击加载状态
	int move_num; //移动加载状态

	bool is_reachup;  
	bool is_jump;
	int jump_num;

	int type; //小怪种类

	Enemy();
	Enemy(
		int _blood_up,
		int _x,
		int _y,
		int _orientation,
		int _attack_scale,
		int _damage,
		int _type, 
		const char* _attack_path,
		const char* _move_path,
		const char* _injuried_path
	);
	void LoadEnemy(WindowConfig& wndconf, int state); //加载小怪
	void Injuried(); //受伤
	void Dead(); //死亡
	void Destroy(); //销毁小怪
	void Move(int stepx, int move_orientation); //移动
	void Jump(int stepx, int stepy); //跳跃
};

Enemy::Enemy() {
	blood = 0;
	blood_up = 0;
	x = 0;
	center_x = x + 50;
	y = 0;
	orientation = 0;
	state = STOP;
	is_dead = false;
	is_attack = false;
	cooling = true;
	attack_last = 0;
	attack_scale = 0;
	damage = 0;
	strcpy_s(attack_path, "");
	strcpy_s(move_path, "");
	strcpy_s(injuried_path, "");
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;
	is_jump = false;
	is_reachup = false;
	jump_num = 0;
}

Enemy::Enemy(

	int _blood_up,
	int _x,
	int _y,
	int _orientation,
	int _attack_scale,
	int _damage,
	int _type,
	const char* _attack_path,
	const char* _move_path,
	const char* _injuried_path
) {
	blood_up = _blood_up;
	blood = blood_up;
	x = _x;
	center_x = x + 50;
	y = _y;
	orientation = _orientation;
	state = STOP;
	is_dead = false;
	is_attack = false;
	cooling = true;
	attack_last = 0;
	attack_scale = _attack_scale;
	damage = _damage;

	strcpy_s(attack_path, _attack_path);
	strcpy_s(move_path, _move_path);
	strcpy_s(injuried_path, _injuried_path);
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;
	is_jump = false;
	is_reachup = false;
	jump_num = 0;

	type = _type;
	if (type == 1) {
		move = (HBITMAP)LoadImage(NULL, move_path, IMAGE_BITMAP, 1600, 400, LR_LOADFROMFILE);
		attack = (HBITMAP)LoadImage(NULL, attack_path, IMAGE_BITMAP, 2400, 400, LR_LOADFROMFILE);
		injuried = (HBITMAP)LoadImage(NULL, injuried_path, IMAGE_BITMAP, 200, 400, LR_LOADFROMFILE);
	}
	if (type == 2) {
		move = (HBITMAP)LoadImage(NULL, move_path, IMAGE_BITMAP, 800, 260, LR_LOADFROMFILE);
		attack = (HBITMAP)LoadImage(NULL, attack_path, IMAGE_BITMAP, 1320, 200, LR_LOADFROMFILE);
		injuried = (HBITMAP)LoadImage(NULL, injuried_path, IMAGE_BITMAP, 100, 260, LR_LOADFROMFILE);
	}

}

void Enemy::LoadEnemy(WindowConfig& wndconf, int state) {
	//行走状态
	if (state == MOVE) {
		SelectObject(wndconf.bufdc, move);
		if (type == 1) {
			//左右方向判断
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y - 80, 200, 200, wndconf.bufdc, move_num * 200, 0, 200, 200, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y - 80, 200, 200, wndconf.bufdc, move_num * 200, 200, 200, 200, RGB(0, 0, 0));
			}
		}
		if (type == 2) {
			//左右方向判断
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 0, 100, 130, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 130, 100, 130, RGB(0, 0, 0));
			}
		}
	}
	//攻击状态
	else if (state == ATTACK) {
		SelectObject(wndconf.bufdc, attack);
		if (type == 1) {
			//左右方向判断
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y - 80, 200, 200, wndconf.bufdc, attack_num * 200, 0, 200, 200, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y - 80, 200, 200, wndconf.bufdc, attack_num * 200, 200, 200, 200, RGB(0, 0, 0));
			}
		}
		if (type == 2) {
			//左右方向判断
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y, 110, 100, wndconf.bufdc, attack_num * 110, 0, 110, 100, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y, 110, 100, wndconf.bufdc, attack_num * 110, 100, 110, 100, RGB(0, 0, 0));
			}
		}

	}
	//停止状态
	else if (state == STOP) {
		SelectObject(wndconf.bufdc, move);
		if (type == 1) {
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y - 80, 200, 200, wndconf.bufdc, 0, 0, 200, 200, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y - 80, 200, 200, wndconf.bufdc, 0, 200, 200, 200, RGB(0, 0, 0));
			}
		}
		if (type == 2) {
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 0, 100, 130, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 130, 100, 130, RGB(0, 0, 0));
			}
		}

	}
	//受伤状态
	else if (state == INJURIED) {
		SelectObject(wndconf.bufdc, injuried);
		if (type == 1) {
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y - 90, 200, 200, wndconf.bufdc, 0, 0, 200, 200, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y - 90, 200, 200, wndconf.bufdc, 0, 200, 200, 200, RGB(0, 0, 0));
			}
		}
		if (type == 2) {
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y - 10, 100, 130, wndconf.bufdc, 0, 0, 100, 130, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y - 10, 100, 130, wndconf.bufdc, 0, 130, 100, 130, RGB(0, 0, 0));
			}
		}

	}

}

void Enemy::Move(int stepx, int move_orientation) {
	if (move_orientation == LEFT) {
		x -= stepx;
	}
	else {
		x += stepx;
	};

	center_x = x + 50;
}

void Enemy::Jump(int stepx, int stepy) {
	y += stepy;
	x += stepx;
	center_x = x + 50;
}