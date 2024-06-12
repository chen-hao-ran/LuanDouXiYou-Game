#pragma once

#include "Common.h"
#include "WindowConfig.h"
#include "Scene.h"
#include "Role.h"
#include "Enemy.h"
#include "Boss.h"
#include "UI.h"

struct MainScene {
	WindowConfig& wndconf; //主窗体结构
	Role& role; //角色
	Enemy (&enemies)[LEVEL_MAXNUM][ENEMIES_MAXNUM]; //小怪
	Boss* boss; //Boss
	Scene* scenes; //场景
	UI& ui; //ui
	HBITMAP background; //背景
	HBITMAP failed; //失败结算页面
	int x; //加载起始坐标
	int y; //加载起始坐标
	int w; //加载宽度
	int h; //加载高度
	int enemy_num; //小怪数量
	char file_path[50]; //位图路径
	char failed_path[50]; //失败结算页面位图路径
	int scene_id; //当前所在场景id
	int backup_scene_id; //场景id备份
	bool win; //是否通关
	int win_num; //通关传送门加载状态
	bool is_replay;  //是否回放
	int now_handle;   //回放当前所在操作
	DWORD replay_start_time; //回放开始时间
	DWORD level_start_time; //关卡开始时间

	MainScene();
	MainScene(WindowConfig& _wndconf, Scene* _scenes, Role& _role, Enemy (&_enemies)[LEVEL_MAXNUM][ENEMIES_MAXNUM], Boss* _boss, UI& _ui, int _x, int _y, int _w, int _h, const char* _file_path, const char* _failed_path);
	void LoadMainScene(); //加载场景
	void Change(); //场景变化
	void Play(); //关卡运行
	void Choose(int id); //切换关卡
	void Destroy(); //销毁场景
};

MainScene::MainScene(
	WindowConfig& _wndconf, 
	Scene* _scenes, 
	Role& _role, 
	Enemy (&_enemies)[LEVEL_MAXNUM][ENEMIES_MAXNUM], 
	Boss* _boss, 
	UI& _ui, 
	int _x, 
	int _y, 
	int _w, 
	int _h,
	const char* _file_path,
	const char* _failed_path
) : wndconf(_wndconf), scenes(_scenes), role(_role), enemies(_enemies), ui(_ui), boss(_boss) {
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	enemy_num = ENEMIES_MAXNUM;
	scene_id = MAINSCENE;
	backup_scene_id = MAINSCENE;
	strcpy_s(file_path, _file_path);
	strcpy_s(failed_path, _failed_path);
	background = (HBITMAP)LoadImage(NULL, file_path, IMAGE_BITMAP, w, h, LR_LOADFROMFILE);
	failed = (HBITMAP)LoadImage(NULL, failed_path, IMAGE_BITMAP, w, h, LR_LOADFROMFILE);
	win = false;
	win_num = 0;
	is_replay = false;
	now_handle = 0;
	replay_start_time = 0;
	level_start_time = 0;
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

	now_handle = 0;
	is_replay = false;
}

void MainScene::Play() {
	//当前时间
	DWORD time_now = GetTickCount();

	//当前场景
	Scene& now_scene = scenes[scene_id];

	//关卡中角色正处于的场景编号
	int scene_stage; 


	//主界面
	if (scene_id == MAINSCENE) {

	}
	//失败界面
	else if (scene_id == FAILED) {
		SelectObject(wndconf.bufdc, failed);
		BitBlt(wndconf.mdc, 0, 0, w, h, wndconf.bufdc, 0, 0, SRCCOPY);
		//hP = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		//hBr = CreateSolidBrush(RGB(0, 0, 0));
		//SelectObject(wndconf.mdc, hP);
		//SelectObject(wndconf.mdc, hBr);
		//Rectangle(wndconf.mdc, 560, 440, 740, 480);
		is_replay = false;
	}
	//关卡
	else {
		//角色所处场景
		scene_stage = now_scene.x / (now_scene.w / 3);

		//保证行走不走出循环
		role.move_num %= 7;
		for (int i = 0; i < ENEMIES_MAXNUM; i++) {
			enemies[scene_id][i].move_num %= 7;
		}
		boss[scene_id].move_num %= 7;
		win_num = ++win_num % 20;

		//加载场景
		scenes[scene_id].LoadScene(wndconf);

		if (keyJ && role.state != INJURIED && !role.is_skill) {
			//判断是否进入下一阶段
			if (time_now - keyJ_last <= 1000 && role.is_attack == false && keyJ_last != 0) {
				role.attack_stage = (role.attack_stage + 1) % 3;
			}
			else if (time_now - keyJ_last > 1000) {
				role.attack_stage = 0;
			}

			role.is_attack = true;

			scenes[scene_id].v = 0;
		}

		//按键释放技能
		if ((keyY || keyU || keyL) && !role.is_attack && role.state != INJURIED) {
			if (keyY && !role.is_skill) {
				if (role.mana >= 100) {
					role.skill_type = 1;
					role.state = SKILL;
					role.is_skill = true;
					role.mana = max(role.mana - 100, 0);
				}
			}
			else if (keyL && !role.is_skill) {
				if (role.mana >= 200) {
					role.skill_type = 2;
					role.state = SKILL;
					role.is_skill = true;
					role.mana = max(role.mana - 200, 0);
				}
			}
			else if (keyU && !role.is_skill) {
				if (role.mana >= 50) {
					role.skill_type = 3;
					role.state = SKILL;
					role.is_skill = true;
					role.mana = max(role.mana - 50, 0);
				}
			}

		}

		//角色在非攻击和非受伤的状态才能移动
		else if (!role.is_skill && !role.is_attack && role.state != INJURIED) {
			if (keyA || keyA_double || keyD || keyD_double) {
				if (keyA_double) {        //向左奔跑
					now_scene.v = 15;
					role.v = 15;
					role.orientation = LEFT;
					role.state = RUN;
					role.move_num++;

					if (role.x >= w / 4) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage]) {
						if (!(scene_stage == 0 && now_scene.x <= now_scene.v))
							role.v = 0;
					}
					else {
						now_scene.v = 0;
					}

					//如果碰到边界，速度为0
					if (now_scene.x - now_scene.v <= 0) {
						now_scene.v = 0;
					}
					if (role.x - role.v <= 0) {
						role.v = 0;
					}

					scenes[scene_id].Change(wndconf, -scenes[scene_id].v);
				}
				else if (keyD_double) {    //向右奔跑

					now_scene.v = 15;
					role.v = 15;
					role.orientation = RIGHT;
					role.state = RUN;
					role.move_num++;

					if (role.x <= w / 4 * 3) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage]) {
						if (!(scene_stage == 2 && now_scene.x + w + now_scene.v >= now_scene.w)) {
							role.v = 0;
						}
					}
					else {
						now_scene.v = 0;
					}

					//如果碰到边界，速度为0
					if (now_scene.x + w + now_scene.v >= now_scene.w) {
						now_scene.v = 0;
					}
					if (x + 100 + role.v >= w) {
						role.v = 0;
					}

					scenes[scene_id].Change(wndconf, scenes[scene_id].v);
				}
				else if (keyA) {       //向左行走

					now_scene.v = 10;
					role.v = 10;
					role.orientation = LEFT;
					role.state = MOVE;
					role.move_num++;

					if (role.x >= w / 4) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage] && scene_stage != 0) {
						if (!(scene_stage == 0 && now_scene.x <= now_scene.v))
							role.v = 0;
					}
					else {
						now_scene.v = 0;
					}

					//如果碰到边界，速度为0
					if (now_scene.x - now_scene.v <= 0) {
						now_scene.v = 0;
					}
					if (role.x - role.v <= 0) {
						role.v = 0;
					}

					scenes[scene_id].Change(wndconf, -scenes[scene_id].v);
				}
				else if (keyD) {      //向右行走
					now_scene.v = 10;
					role.v = 10;
					role.orientation = RIGHT;
					role.state = MOVE;
					role.move_num++;

					if (role.x <= w / 4 * 3) {
						now_scene.v = 0;
					}
					else if (!now_scene.lock[scene_stage]) {
						if (!(scene_stage == 2 && now_scene.x + w + now_scene.v >= now_scene.w)) {
							role.v = 0;
						}

					}
					else {
						now_scene.v = 0;
					}

					//如果遇到边界，速度设置为0
					if (now_scene.x + w + now_scene.v >= now_scene.w) {
						now_scene.v = 0;
					}
					if (x + 100 + role.v >= w) {
						role.v = 0;
					}

					scenes[scene_id].Change(wndconf, scenes[scene_id].v);
				}

				//判断角色是否进入新场景
				if (now_scene.x / (now_scene.w / 3) != scene_stage) {
					//小怪重新出现
					enemy_num = ENEMIES_MAXNUM;
					if (now_scene.x / (now_scene.w / 3) != 2) {
						for (int i = 0; i < ENEMIES_MAXNUM; i++) {
							enemies[scene_id][i].is_dead = false;
							enemies[scene_id][i].x = 500 + i * 100;
							enemies[scene_id][i].attack_last = time_now;
							enemies[scene_id][i].cooling = true;
						}
					}
					else {
						boss[scene_id].is_dead = false;
						boss[scene_id].x = 300;
					}
				}
				
				//角色移动
				role.Move(role.v, role.orientation);
			}
			//停止
			else if (!role.is_jump && !role.is_attack) {               
				role.move_num = 0;
				role.state = STOP;
				role.v = 0;
				scenes[scene_id].v = 0;
			}
		}

		//按键跳跃
		if (keyK) {
			role.is_jump = true;
		}

		// 跳跃过程
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

		//攻击过程
		if (role.is_attack) {
			role.move_num = 0; //攻击时不能移动
			role.state = ATTACK;

			role.attack_num++; //继续更新攻击动作
			if (role.attack_stage != 2) {
				if (role.attack_num == 8) {
					role.attack_num = 0;
					role.is_attack = false;
					role.state = STOP;
					//if (role.orientation == LEFT) {
					//	role.x += 60;
					//}
				}
			}
			else {
				if (role.attack_num == 10) {
					role.attack_num = 0;
					role.is_attack = false;
					role.state = STOP;
					//if (role.orientation == LEFT) {
					//	role.x += 60;
					//}
				}
			}

		}

		//释放技能过程
		if (role.is_skill) {
			role.skill_num++;

			if (role.skill_type == 1) {
				if (role.skill_num == 15) {
					role.is_skill = false;
					role.skill_num = 0;
				}
			}
			else if (role.skill_type == 2) {
				//role.x += (role.orientation == LEFT) ? -20 : 20;

				if (role.skill_num == 15) {
					role.is_skill = false;
					role.skill_num = 0;
					role.x += (role.orientation == LEFT) ? - 300 : 300;
				}
			}
			else if (role.skill_type == 3) {
				role.x += (role.orientation == LEFT) ? -50 : 50;
				role.center_x += (role.orientation == LEFT) ? -50 : 50;

				if (role.skill_num == 9) {
					role.is_skill = false;
					role.skill_num = 0;
				}
			}
		}

//------------------------------小怪---------------------------------------------------
		//将背景的移动速度抵消
		for (int i = 0; i < ENEMIES_MAXNUM; i++) {
			enemies[scene_id][i].Move((role.orientation == LEFT) ? -scenes[scene_id].v : scenes[scene_id].v, LEFT);
		}
		
		for (int i = 0; i < ENEMIES_MAXNUM; i++) {
			//是否被角色攻击
			if (role.is_skill) {
				if (role.skill_type == 1) {
					if (
						(role.orientation == LEFT 
						&& enemies[scene_id][i].center_x <= role.center_x && role.center_x - enemies[scene_id][i].center_x <= 150 
						&& role.y >= now_scene.horizon - 50)
						|| (role.orientation == RIGHT 
							&& enemies[scene_id][i].center_x >= role.center_x 
							&& enemies[scene_id][i].center_x - role.center_x <= 150 
							&& role.y >= now_scene.horizon - 50)
						) {
						enemies[scene_id][i].state = INJURIED;
						enemies[scene_id][i].blood -= 10 * role.damage;
						enemies[scene_id][i].blood = max(0, enemies[scene_id][i].blood);
						enemies[scene_id][i].move_num = 0;
						enemies[scene_id][i].is_jump = true;
						enemies[scene_id][i].attack_last = time_now;
						enemies[scene_id][i].cooling = true;
					}

				}
				if (role.skill_type == 2) {
					if (
						(role.orientation == LEFT
							&& enemies[scene_id][i].center_x <= role.center_x
							&& role.center_x - enemies[scene_id][i].center_x <= 150
							&& abs(role.y - 300 - enemies[scene_id][i].y) <= 100)
						|| 
						(role.orientation == RIGHT
							&& enemies[scene_id][i].center_x >= role.center_x
							&& enemies[scene_id][i].center_x - role.center_x <= 150
							&& role.y >= now_scene.horizon - 50)
						) {
						enemies[scene_id][i].state = INJURIED;
						enemies[scene_id][i].blood -= 10 * role.damage;
						enemies[scene_id][i].blood = max(0, enemies[scene_id][i].blood);
						enemies[scene_id][i].move_num = 0;
						enemies[scene_id][i].is_jump = true;
						enemies[scene_id][i].attack_last = time_now;
						enemies[scene_id][i].cooling = true;
					}
				}
				if (role.skill_type == 3) {
					if (abs(role.center_x - enemies[scene_id][i].center_x) <= 30 && role.y >= now_scene.horizon - 30) {
						enemies[scene_id][i].state = INJURIED;
						enemies[scene_id][i].blood -= 1000 * role.damage;
						enemies[scene_id][i].blood = max(0, enemies[scene_id][i].blood);
						enemies[scene_id][i].move_num = 0;
						enemies[scene_id][i].is_jump = true;
						enemies[scene_id][i].attack_last = time_now;
						enemies[scene_id][i].cooling = true;
					}
				}
			}
			else if (role.is_attack) {
				if ((role.orientation == LEFT && enemies[scene_id][i].center_x <= role.center_x && role.center_x - enemies[scene_id][i].center_x <= 150 && role.y >= now_scene.horizon - 50)
					|| (role.orientation == RIGHT && enemies[scene_id][i].center_x >= role.center_x && enemies[scene_id][i].center_x - role.center_x <= 150 && role.y >= now_scene.horizon - 50)) {
					enemies[scene_id][i].state = INJURIED;
					enemies[scene_id][i].blood -= role.damage;
					enemies[scene_id][i].blood = max(0, enemies[scene_id][i].blood);
					enemies[scene_id][i].move_num = 0;
					enemies[scene_id][i].attack_last = time_now;
					enemies[scene_id][i].cooling = true;
					enemies[scene_id][i].x += (enemies[scene_id][i].orientation == LEFT) ? 2 : -2;
					continue;
				}
			}

			//小怪被击飞过程
			if (!enemies[scene_id][i].is_reachup && enemies[scene_id][i].is_jump) {
				enemies[scene_id][i].jump_num++;
				enemies[scene_id][i].Jump((enemies[scene_id][i].orientation == LEFT) ? 15 : -15, -3 * (15 - enemies[scene_id][i].jump_num));

				if (enemies[scene_id][i].jump_num == 8) {
					enemies[scene_id][i].is_reachup = true;
				}
				continue;
			}
			else if (enemies[scene_id][i].is_jump) {
				enemies[scene_id][i].jump_num--;
				enemies[scene_id][i].Jump((enemies[scene_id][i].orientation == LEFT) ? 15 : -15, 3 * (14 - enemies[scene_id][i].jump_num));
				if (enemies[scene_id][i].jump_num == 0) {
					enemies[scene_id][i].is_jump = false;
					enemies[scene_id][i].is_reachup = false;
				}
				continue;
			}

			//判断小怪是否死亡
			if (enemies[scene_id][i].blood <= 0) {
				enemies[scene_id][i].is_dead = true;
				enemies[scene_id][i].blood = enemies[scene_id][i].blood_up;
				enemy_num--;
			}


			//判断小怪是否全部死亡
			if (enemy_num == 0) {
				now_scene.lock[scene_stage] = false;
			}

			//是否刷新冷却
			if (time_now - enemies[scene_id][i].attack_last >= 1500 && ((rand() % 10) == 1)) {
				enemies[scene_id][i].cooling = false;
			}

			//移动
			if (role.center_x + enemies[scene_id][i].attack_scale <= enemies[scene_id][i].center_x 
				&& !enemies[scene_id][i].is_jump) {
				enemies[scene_id][i].Move(4, LEFT);
				enemies[scene_id][i].orientation = LEFT;
				enemies[scene_id][i].move_num++;
				enemies[scene_id][i].state = MOVE;
			}
			else if (role.center_x > enemies[scene_id][i].center_x + enemies[scene_id][i].attack_scale 
				&& !enemies[scene_id][i].is_jump) {
				enemies[scene_id][i].Move(4, RIGHT);
				enemies[scene_id][i].orientation = RIGHT;
				enemies[scene_id][i].move_num++;
				enemies[scene_id][i].state = MOVE;
			}
			//攻击
			else if (
				!enemies[scene_id][i].cooling 
				&& !enemies[scene_id][i].is_dead 
				&& role.y >= now_scene.horizon - 50 
				&& !enemies[scene_id][i].is_jump
				&& !role.is_skill
				) {
				enemies[scene_id][i].state = ATTACK;
				enemies[scene_id][i].is_attack = true;
				enemies[scene_id][i].attack_last = time_now;
				enemies[scene_id][i].move_num = 0; //攻击时不能移动
				enemies[scene_id][i].attack_num++;
				enemies[scene_id][i].x += (enemies[scene_id][i].orientation == LEFT) ? -4 : 4;
				//角色处于受伤状态
				role.state = INJURIED; 
				role.blood -= enemies[scene_id][i].damage;
				role.blood = max(0, role.blood);

				//攻击完成后停止,不再循环
				if (enemies[scene_id][i].attack_num == 12) {
					enemies[scene_id][i].attack_num = 0;
					enemies[scene_id][i].is_attack = false;
					enemies[scene_id][i].cooling = true;
					enemies[scene_id][i].state = STOP;
					enemies[scene_id][i].x -= (enemies[scene_id][i].orientation == LEFT) ? -4 * 12 : 4 * 12;

					//将角色的状态恢复成非受伤
					if (role.state == INJURIED) {
						role.state = STOP;
					}
				}
			}
			//停止
			else {
				enemies[scene_id][i].state = STOP;
				enemies[scene_id][i].move_num = 0;
			}
		}

		//------------------------------BOSS---------------------------------------------------
		//将背景的移动速度抵消
		boss[scene_id].Move((role.orientation == LEFT) ? -scenes[scene_id].v : scenes[scene_id].v, LEFT);

		bool tmp = false;
		//是否被角色攻击
		if (role.is_skill && !boss[scene_id].is_dead) {
			if (role.skill_type == 1) {
				if (
					(role.orientation == LEFT
						&& boss[scene_id].center_x <= role.center_x && role.center_x - boss[scene_id].center_x <= 150
						&& role.y >= now_scene.horizon - 50)
					|| (role.orientation == RIGHT
						&& boss[scene_id].center_x >= role.center_x
						&& boss[scene_id].center_x - role.center_x <= 150
						&& role.y >= now_scene.horizon - 50)
					) {
					boss[scene_id].state = INJURIED;
					boss[scene_id].blood -= 10 * role.damage;
					boss[scene_id].blood = max(0, boss[scene_id].blood);
					boss[scene_id].move_num = 0;
					boss[scene_id].is_jump = true;
					boss[scene_id].attack_last = time_now;
					boss[scene_id].cooling = true;
				}

			}
			if (role.skill_type == 2) {
				if (
					(role.orientation == LEFT
						&& boss[scene_id].center_x <= role.center_x
						&& role.center_x - boss[scene_id].center_x <= 150
						&& abs(role.y - 300 - boss[scene_id].y) <= 100)
					||
					(role.orientation == RIGHT
						&& boss[scene_id].center_x >= role.center_x
						&& boss[scene_id].center_x - role.center_x <= 150
						&& role.y >= now_scene.horizon - 50)
					) {
					boss[scene_id].state = INJURIED;
					boss[scene_id].blood -= 10 * role.damage;
					boss[scene_id].blood = max(0, boss[scene_id].blood);
					boss[scene_id].move_num = 0;
					boss[scene_id].is_jump = true;
					boss[scene_id].attack_last = time_now;
					boss[scene_id].cooling = true;
				}
			}
			if (role.skill_type == 3) {
				if (abs(role.center_x - boss[scene_id].center_x) <= 30 && role.y >= now_scene.horizon - 30) {
					boss[scene_id].state = INJURIED;
					boss[scene_id].blood -= 1000 * role.damage;
					boss[scene_id].blood = max(0, boss[scene_id].blood);
					boss[scene_id].move_num = 0;
					boss[scene_id].is_jump = true;
					boss[scene_id].attack_last = time_now;
					boss[scene_id].cooling = true;
				}
			}
		}
		//被普通攻击
		else if (role.is_attack && !boss[scene_id].is_dead) {
			if ((role.orientation == LEFT 
				&& boss[scene_id].center_x <= role.center_x 
				&& role.center_x - boss[scene_id].center_x <= 150 
				&& role.y >= now_scene.horizon - 50)
				|| (role.orientation == RIGHT 
					&& boss[scene_id].center_x >= role.center_x 
					&& boss[scene_id].center_x - role.center_x <= 150 
					&& role.y >= now_scene.horizon - 50)) {
				boss[scene_id].state = INJURIED;
				boss[scene_id].blood -= role.damage;
				boss[scene_id].blood = max(0, boss[scene_id].blood);
				boss[scene_id].move_num = 0;
				boss[scene_id].attack_last = time_now;
				boss[scene_id].cooling = true;
				boss[scene_id].x += (boss[scene_id].orientation == LEFT) ? 2 : -2;
				tmp = true;
			}
		}

		//Boss被击飞过程
		if (!boss[scene_id].is_reachup && boss[scene_id].is_jump) {
			boss[scene_id].jump_num++;
			boss[scene_id].Jump((boss[scene_id].orientation == LEFT) ? 15 : -15, -3 * (15 - boss[scene_id].jump_num));

			if (boss[scene_id].jump_num == 8) {
				boss[scene_id].is_reachup = true;
			}
			tmp = true;
		}
		else if (boss[scene_id].is_jump) {
			boss[scene_id].jump_num--;
			boss[scene_id].Jump((boss[scene_id].orientation == LEFT) ? 15 : -15, 3 * (14 - boss[scene_id].jump_num));
			if (boss[scene_id].jump_num == 0) {
				boss[scene_id].is_jump = false;
				boss[scene_id].is_reachup = false;
			}
			tmp = true;
		}
		//非受伤状态下
		if (!tmp) {
			//判断Boss是否死亡
			if (boss[scene_id].blood <= 0 && !boss[scene_id].is_dead) {
				boss[scene_id].is_dead = true;
				boss[scene_id].blood = boss[scene_id].blood_up;
					
				//通关场景
				win = true;
			}

			//是否刷新冷却
			if (time_now - boss[scene_id].attack_last >= 1500 && ((rand() % 10) == 1)) {
				boss[scene_id].cooling = false;
			}

			//移动
			if (role.center_x + boss[scene_id].attack_scale <= boss[scene_id].center_x
				&& !boss[scene_id].is_jump) {
				boss[scene_id].Move(4, LEFT);
				boss[scene_id].orientation = LEFT;
				boss[scene_id].move_num++;
				boss[scene_id].state = MOVE;
			}
			else if (role.center_x > boss[scene_id].center_x + boss[scene_id].attack_scale
				&& !boss[scene_id].is_jump) {
				boss[scene_id].Move(4, RIGHT);
				boss[scene_id].orientation = RIGHT;
				boss[scene_id].move_num++;
				boss[scene_id].state = MOVE;
			}
			//攻击
			else if (
				!boss[scene_id].cooling
				&& !boss[scene_id].is_dead
				&& role.y >= now_scene.horizon - 50
				&& !boss[scene_id].is_jump
				&& !role.is_skill
				) {
				boss[scene_id].state = ATTACK;
				boss[scene_id].is_attack = true;
				boss[scene_id].attack_last = time_now;
				boss[scene_id].move_num = 0; //攻击时不能移动
				boss[scene_id].attack_num++;
				boss[scene_id].x += (boss[scene_id].orientation == LEFT) ? -4 : 4;
				//角色处于受伤状态
				role.state = INJURIED;
				role.blood -= boss[scene_id].damage;
				role.blood = max(0, role.blood);

				//攻击完成后停止,不再循环
				if (boss[scene_id].attack_num == 8) {
					boss[scene_id].attack_num = 0;
					boss[scene_id].is_attack = false;
					boss[scene_id].cooling = true;
					boss[scene_id].state = STOP;
					boss[scene_id].x -= (boss[scene_id].orientation == LEFT) ? -4 * 12 : 4 * 12;

					//将角色的状态恢复成非受伤
					if (role.state == INJURIED) {
						role.state = STOP;
					}
				}
			}
			//停止
			else {
				boss[scene_id].state = STOP;
				boss[scene_id].move_num = 0;
			}
		}

		//判断人物是否死亡
		if (role.blood <= 0) {
			role.is_dead = true;
			role.blood = role.blood_up;
			backup_scene_id = scene_id;
			scene_id = FAILED;
		}

		//----------------------------------------------------------------------------------
		if (!role.is_dead) {
			//加载小怪
			for (int i = 0; i < ENEMIES_MAXNUM; i++) {
				if (enemies[scene_id][i].is_dead) {
					continue;
				}
				enemies[scene_id][i].LoadEnemy(wndconf, enemies[scene_id][i].state);
			}

			//加载Boss
			if (!boss[scene_id].is_dead) {
				boss[scene_id].LoadBoss(wndconf, boss[scene_id].state);
			}

			//加载UI
			ui.LoadUI(wndconf, role, enemies[scene_id], boss[scene_id], win, win_num, scene_id);

			//加载人物
			role.LoadRole(wndconf, role.state);
		}
	}

	wndconf.tPre = GetTickCount();
}

void MainScene::Choose(int id) {
	scene_id = id;
	role.y = scenes[scene_id].horizon;
	for (int i = 0; i < ENEMIES_MAXNUM; i++) {
		enemies[scene_id][i].y = scenes[scene_id].horizon;
	}

	//开始时间
	if (!is_replay) {
		level_start_time = GetTickCount();
	}


	//恢复主场景
	win = false;
	now_handle = 0;

	//恢复场景
	scenes[scene_id].lock[0] = scenes[scene_id].lock[1] = scenes[scene_id].lock[2] = true;
	scenes[scene_id].x = 0;

	//恢复角色
	role.blood = role.blood_up;
	role.is_dead = false;
	role.state = STOP;
	role.x = 0;
	role.center_x = role.x + 40;
	role.mana = role.mana_up;

	//恢复小怪
	for (int i = 0; i < ENEMIES_MAXNUM; i++) {
		enemies[scene_id][i].blood = enemies[scene_id][i].blood_up;
		enemies[scene_id][i].is_dead = false;
		enemies[scene_id][i].x = 500 + 100 * i;
		enemies[scene_id][i].center_x = enemies[scene_id][i].x + 50;
		enemies[scene_id][i].orientation = LEFT;
		enemies[scene_id][i].state = STOP;
	}
	//恢复boss
	boss[scene_id].blood = boss[scene_id].blood_up;
	boss[scene_id].orientation = LEFT;
}