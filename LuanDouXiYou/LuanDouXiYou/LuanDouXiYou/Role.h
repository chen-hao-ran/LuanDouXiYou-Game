#pragma once

#include "Common.h"
#include "WindowConfig.h"

typedef struct Role {
	HBITMAP attack[3]; //攻击状态位图
	HBITMAP move; //行走状态位图
	HBITMAP run; //奔跑状态位图
	HBITMAP injuried; //受伤状态位图
	HBITMAP skill[4]; //技能位图
	int blood; //血量
	int blood_up; //血量上限
	int mana; //魔力值
	int mana_up; //魔力上限
	int empiric; //经验值
	int empiric_up; //升级所需经验值
	int level; //等级
	int x; //位置
	int center_x; //人物中心位置 
	int y; //位置
	int v; //移动速度
	int orientation; //方向
	int state; // 移动状态
	bool is_jump; //是否跳跃
	bool is_reachup; //跳跃是否到顶
	int jump_num; // 跳跃帧数
	bool is_dead; //是否死亡

	int damage; //伤害
	bool is_skill; //是否释放技能
	bool is_attack; //是否普通攻击
	int skill_type; //技能类型

	char attack_path[50]; //攻击位图路径
	char move_path[50]; //行走位图路径
	char run_path[50]; //奔跑位图路径
	char injuried_path[50]; //受伤位图路径
	char skill1_path[50]; //1技能位图路径
	char skill2_path[50]; //2技能位图路径
	char skill3_path[50]; //3技能位图路径
	
	int attack_stage; //普通攻击阶段
	int attack_num; //普通攻击加载状态
	int move_num; //移动加载状态
	int run_num; //奔跑加载状态
	int skill_num; //技能加载状态

	Role();
	Role(
		int _blood_up,
		int _mana_up,
		int _empiric_up,
		int _x,
		int _y,
		int _orientation,
		int _damage,
		const char* _attack_path,
		const char* _move_path,
		const char* _run_path,
		const char* _injuried_path,
		const char* _skill1_path,
		const char* _skill2_path,
		const char* _skill3_path
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

Role::Role() {
	blood = 0;
	blood_up = 0;
	mana = 0;
	mana_up = 0;
	empiric = 0;
	empiric_up = 0;
	level = 0;
	x = 0;
	center_x = x + 40;
	y = 0;
	v = 0;
	orientation = 0;
	state = STOP;
	is_jump = false;
	is_reachup = false;
	jump_num = 0;
	is_dead = false;
	damage = 0;
	is_skill = false;
	is_attack = false;
	skill_type = 0;
	strcpy_s(attack_path, "");
	strcpy_s(move_path, "");
	strcpy_s(run_path, "");
	strcpy_s(injuried_path, "");
	strcpy_s(skill1_path, "");
	strcpy_s(skill2_path, "");
	strcpy_s(skill3_path, "");
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;
	run_num = 0;
	skill_num = 0;
}

Role::Role(

	int _blood_up,
	int _mana_up,
	int _empiric_up,
	int _x,
	int _y,
	int _orientation,
	int _damage,
	const char* _attack_path,
	const char* _move_path,
	const char* _run_path,
	const char* _injuried_path,
	const char* _skill1_path,
	const char* _skill2_path,
	const char* _skill3_path
) {
	blood_up = _blood_up;
	blood = blood_up;
	mana_up = _mana_up;
	mana = mana_up;
	empiric = 0;
	empiric_up = _empiric_up;
	level = 0;
	x = _x;
	center_x = x + 40;
	y = _y;
	v = 0;
	orientation = _orientation;
	state = STOP;
	is_jump = false;
	is_reachup = false;
	jump_num = 0;
	is_dead = false;
	damage = _damage;
	is_skill = false;
	is_attack = false;
	skill_type = 0;

	strcpy_s(attack_path, _attack_path);
	strcpy_s(move_path, _move_path);
	strcpy_s(run_path, _run_path);
	strcpy_s(injuried_path, _injuried_path);
	strcpy_s(skill1_path, _skill1_path);
	strcpy_s(skill2_path, _skill2_path);
	strcpy_s(skill3_path, _skill3_path);
	attack_stage = 0;
	attack_num = 0;
	move_num = 0;
	run_num = 0;
	skill_num = 0;

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
	injuried = (HBITMAP)LoadImage(NULL, injuried_path, IMAGE_BITMAP, 100, 260, LR_LOADFROMFILE);
	skill[0] = (HBITMAP)LoadImage(NULL, skill1_path, IMAGE_BITMAP, 2250, 400, LR_LOADFROMFILE);
	skill[1] = (HBITMAP)LoadImage(NULL, skill2_path, IMAGE_BITMAP, 6000, 800, LR_LOADFROMFILE);
	skill[2] = (HBITMAP)LoadImage(NULL, skill3_path, IMAGE_BITMAP, 2250, 300, LR_LOADFROMFILE);
}

void Role::LoadRole(WindowConfig& wndconf, int state) {
	//跳跃攻击状态
	if (is_jump && state == ATTACK) {
		SelectObject(wndconf.bufdc, attack[2]);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x - 60, y, 150, 130, wndconf.bufdc, attack_num * 150, 0, 150, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 130, 150, 130, RGB(0, 0, 0));
		}
	}
	//攻击状态
	else if (state == ATTACK) {
		SelectObject(wndconf.bufdc, attack[attack_stage]);

		//左右方向判断
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x - 60, y, 150, 130, wndconf.bufdc, attack_num * 150, 0, 150, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 130, 150, 130, RGB(0, 0, 0));
		}
	}
	//技能
	else if (state == SKILL) {
		SelectObject(wndconf.bufdc, skill[skill_type - 1]);
		if (skill_type == 1) {
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x - 35, y - 70, 150, 200, wndconf.bufdc, skill_num * 150, 0, 150, 200, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y - 70, 150, 200, wndconf.bufdc, skill_num * 150, 200, 150, 200, RGB(0, 0, 0));
			}
		}
		if (skill_type == 2) {
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y - 300, 400, 400, wndconf.bufdc, skill_num * 400, 0, 400, 400, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y - 300, 400, 400, wndconf.bufdc, skill_num * 400, 400, 400, 400, RGB(0, 0, 0));
			}
		}
		if (skill_type == 3) {
			if (orientation == LEFT) {
				MyTransparentBlt(wndconf.mdc, x, y + 30, 250, 150, wndconf.bufdc, skill_num * 250, 0, 250, 150, RGB(0, 0, 0));
			}
			else if (orientation == RIGHT) {
				MyTransparentBlt(wndconf.mdc, x, y + 30, 250, 150, wndconf.bufdc, skill_num * 250, 150, 250, 150, RGB(0, 0, 0));
			}
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
	//受伤状态
	else if (state == INJURIED) {
		SelectObject(wndconf.bufdc, injuried);
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, 0, 130, 100, 130, RGB(0, 0, 0));
		}
	} 


}

void Role::Move(int stepx, int move_orientation) {
	if (move_orientation == LEFT) {
		x -= stepx;
	}
	else {
		x += stepx;
	};

	x = min(x, 1100 - 80);
	x = max(x, 0);

	center_x = x + 40;
}

void Role::Jump(int stepy) {
	y += stepy;
}