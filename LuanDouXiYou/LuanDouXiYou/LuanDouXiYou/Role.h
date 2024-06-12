#pragma once

#include "Common.h"
#include "WindowConfig.h"

typedef struct Role {
	HBITMAP attack[3]; //����״̬λͼ
	HBITMAP move; //����״̬λͼ
	HBITMAP run; //����״̬λͼ
	HBITMAP injuried; //����״̬λͼ
	HBITMAP skill[4]; //����λͼ
	int blood; //Ѫ��
	int blood_up; //Ѫ������
	int mana; //ħ��ֵ
	int mana_up; //ħ������
	int empiric; //����ֵ
	int empiric_up; //�������辭��ֵ
	int level; //�ȼ�
	int x; //λ��
	int center_x; //��������λ�� 
	int y; //λ��
	int v; //�ƶ��ٶ�
	int orientation; //����
	int state; // �ƶ�״̬
	bool is_jump; //�Ƿ���Ծ
	bool is_reachup; //��Ծ�Ƿ񵽶�
	int jump_num; // ��Ծ֡��
	bool is_dead; //�Ƿ�����

	int damage; //�˺�
	bool is_skill; //�Ƿ��ͷż���
	bool is_attack; //�Ƿ���ͨ����
	int skill_type; //��������

	char attack_path[50]; //����λͼ·��
	char move_path[50]; //����λͼ·��
	char run_path[50]; //����λͼ·��
	char injuried_path[50]; //����λͼ·��
	char skill1_path[50]; //1����λͼ·��
	char skill2_path[50]; //2����λͼ·��
	char skill3_path[50]; //3����λͼ·��
	
	int attack_stage; //��ͨ�����׶�
	int attack_num; //��ͨ��������״̬
	int move_num; //�ƶ�����״̬
	int run_num; //���ܼ���״̬
	int skill_num; //���ܼ���״̬

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
	void LoadRole(WindowConfig& wndconf, int state); //��������
	void Injuried(); //����
	void Dead(); //����
	void Destroy(); //���ٽ�ɫ
	void LevelUp(); //����
	void Move(int stepx, int move_orientation); //�ƶ�
	void Run(int stepx, int move_orientation); //����
	void Jump(int stepy); //��Ծ
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
	//��Ծ����״̬
	if (is_jump && state == ATTACK) {
		SelectObject(wndconf.bufdc, attack[2]);

		//���ҷ����ж�
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x - 60, y, 150, 130, wndconf.bufdc, attack_num * 150, 0, 150, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 130, 150, 130, RGB(0, 0, 0));
		}
	}
	//����״̬
	else if (state == ATTACK) {
		SelectObject(wndconf.bufdc, attack[attack_stage]);

		//���ҷ����ж�
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x - 60, y, 150, 130, wndconf.bufdc, attack_num * 150, 0, 150, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 150, 130, wndconf.bufdc, attack_num * 150, 130, 150, 130, RGB(0, 0, 0));
		}
	}
	//����
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
	//��Ծ״̬
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
	//����״̬
	else if (state == MOVE) {
		SelectObject(wndconf.bufdc, move);

		//���ҷ����ж�
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 130, 100, 130, RGB(0, 0, 0));
		}
	}
	//����״̬
	else if (state == RUN) {
		SelectObject(wndconf.bufdc, run);

		//���ҷ����ж�
		if (orientation == LEFT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 0, 100, 130, RGB(0, 0, 0));
		}
		else if (orientation == RIGHT) {
			MyTransparentBlt(wndconf.mdc, x, y, 100, 130, wndconf.bufdc, move_num * 100, 130, 100, 130, RGB(0, 0, 0));
		}
	}
	//ֹͣ״̬
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
	//����״̬
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