#pragma once
#pragma once

#include "Common.h"
#include "WindowConfig.h"

typedef struct Boss {
	HBITMAP attack; //����״̬λͼ
	HBITMAP move; //����״̬λͼ
	HBITMAP run; //����״̬λͼ
	HBITMAP injuried; //����״̬λͼ
	int blood; //Ѫ��
	int blood_up; //Ѫ������
	int x; //λ��
	int center_x; //С������λ��
	int y; //λ��
	int orientation; //����
	int state; // �ƶ�״̬
	bool is_dead; //�Ƿ�����

	bool is_attack; //�Ƿ���ͨ����
	bool cooling; //������ȴ״̬
	DWORD attack_last; //�ϴι���ʱ��
	int attack_scale; //������Χ
	int damage; //�˺�

	char attack_path[50]; //����λͼ·��
	char move_path[50]; //�ƶ�λͼ·��
	char injuried_path[50]; //����λͼ·��
	int attack_stage; //��ͨ�����׶�
	int attack_num; //��ͨ��������״̬
	int move_num; //�ƶ�����״̬

	bool is_reachup;
	bool is_jump;
	int jump_num;

	Boss();
	Boss(
		int _blood_up,
		int _x,
		int _y,
		int _orientation,
		int _attack_scale,
		int _damage,
		const char* _attack_path,
		const char* _move_path,
		const char* _injuried_path
	);
	void LoadBoss(WindowConfig& wndconf, int state); //����С��
	void Injuried(); //����
	void Dead(); //����
	void Destroy(); //����С��
	void Move(int stepx, int move_orientation); //�ƶ�
	void Jump(int stepx, int stepy); //��Ծ
};

Boss::Boss() {
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

Boss::Boss(
	int _blood_up,
	int _x,
	int _y,
	int _orientation,
	int _attack_scale,
	int _damage,
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
	is_dead = true;
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

	move = (HBITMAP)LoadImage(NULL, move_path, IMAGE_BITMAP, 1600, 400, LR_LOADFROMFILE);
	attack = (HBITMAP)LoadImage(NULL, attack_path, IMAGE_BITMAP, 1600, 400, LR_LOADFROMFILE);
	injuried = (HBITMAP)LoadImage(NULL, injuried_path, IMAGE_BITMAP, 200, 400, LR_LOADFROMFILE);
}

void Boss::LoadBoss(WindowConfig& wndconf, int state) {
	//����״̬
	if (state == MOVE) {
		SelectObject(wndconf.bufdc, move);

		//���ҷ����ж�
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, move_num * 200, 0, 200, 200, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, move_num * 200, 200, 200, 200, RGB(0, 0, 0));
		}
	}
	//����״̬
	else if (state == RUN) {
		SelectObject(wndconf.bufdc, run);

		//���ҷ����ж�
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, move_num * 200, 0, 200, 200, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, move_num * 200, 200, 200, 200, RGB(0, 0, 0));
		}
	}
	//����״̬
	else if (state == ATTACK) {
		SelectObject(wndconf.bufdc, attack);

		//���ҷ����ж�
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, attack_num * 200, 0, 200, 200, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, attack_num * 200, 200, 200, 200, RGB(0, 0, 0));
		}
	}
	//ֹͣ״̬
	else if (state == STOP) {
		SelectObject(wndconf.bufdc, move);
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, 0, 0, 200, 200, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 200, 200, wndconf.bufdc, 0, 200, 200, 200, RGB(0, 0, 0));
		}
	}
	//����״̬
	else if (state == INJURIED) {
		SelectObject(wndconf.bufdc, injuried);
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y - 10, 200, 200, wndconf.bufdc, 0, 0, 200, 200, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y - 10, 200, 200, wndconf.bufdc, 0, 200, 200, 200, RGB(0, 0, 0));
		}
	}

}

void Boss::Move(int stepx, int move_orientation) {
	if (move_orientation == LEFT) {
		x -= stepx;
	}
	else {
		x += stepx;
	};

	center_x = x + 100;
}

void Boss::Jump(int stepx, int stepy) {
	y += stepy;
	x += stepx;
	center_x = x + 100;
}