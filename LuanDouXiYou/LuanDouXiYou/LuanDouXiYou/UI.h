#pragma once

#include "Common.h"

typedef struct UI {
	HBITMAP state; //״̬��λͼ
	HBITMAP boss_state; //bossѪ��λͼ
	HBITMAP skill_state; //������λͼ
	HBITMAP win_item; //ͨ�ش�����λͼ
	HBITMAP replay_item; //�طŰ�ť
	char state_path[50]; //״̬��λͼ·��
	char boss_state_path[50]; //bossѪ��λͼ·��
	char skill_state_path[50]; //������λͼ·��
	char win_path[50]; //ͨ�ش�����λͼ·�� 
	char replay_path[50]; //�طŰ�ťλͼ·��

	UI();
	UI(const char* _state_path, const char* _boss_state_path, const char* _skill_state_path, const char* _win_path, const char* _replay_path);
	void LoadUI(WindowConfig& wndconf, const Role& role, Enemy* enemies, const Boss& boss, bool win, int win_num, int scene_id);
};

UI::UI() {
	strcpy_s(state_path, "");
	strcpy_s(boss_state_path, "");
	strcpy_s(skill_state_path, "");
	strcpy_s(win_path, "");
	strcpy_s(replay_path, "");
}

UI::UI(const char* _state_path, const char* _boss_state_path, const char* _skill_state_path, const char* _win_path, const char* _replay_path) {
	strcpy_s(state_path, _state_path);
	strcpy_s(boss_state_path, _boss_state_path);
	strcpy_s(skill_state_path, _skill_state_path);
	strcpy_s(win_path, _win_path);
	strcpy_s(replay_path, _replay_path);
	
	state = (HBITMAP)LoadImage(NULL, state_path, IMAGE_BITMAP, 345, 127, LR_LOADFROMFILE);
	boss_state = (HBITMAP)LoadImage(NULL, boss_state_path, IMAGE_BITMAP, 628, 82, LR_LOADFROMFILE);
	skill_state = (HBITMAP)LoadImage(NULL, skill_state_path, IMAGE_BITMAP, 201, 72, LR_LOADFROMFILE);
	win_item = (HBITMAP)LoadImage(NULL, win_path, IMAGE_BITMAP, 4000, 200, LR_LOADFROMFILE);
	replay_item = (HBITMAP)LoadImage(NULL, replay_path, IMAGE_BITMAP, 298, 270, LR_LOADFROMFILE);
}

void UI::LoadUI(WindowConfig& wndconf, const Role& role, Enemy* enemies, const Boss& boss, bool win, int win_num, int scene_id) {
	//��ɫѪ��������
	SelectObject(wndconf.bufdc, state);
	//BitBlt(wndconf.mdc, 0, 0, 345, 127, wndconf.bufdc, 0, 0, SRCCOPY);
	MyTransparentBlt(wndconf.mdc, 0, 0, 345, 127, wndconf.bufdc, 0, 0, 345, 127, RGB(255, 255, 255));
	
	hP = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hBr = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(wndconf.mdc, hP);
	SelectObject(wndconf.mdc, hBr);
	Rectangle(wndconf.mdc, 324 - (role.blood_up - role.blood) * 1.0 / role.blood_up * 196, 25, 324, 35);
	Rectangle(wndconf.mdc, 326 - (role.mana_up - role.mana) * 1.0 / role.mana_up * 196, 55, 326, 65);


	//С��Ѫ��
	hP = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	hBr = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(wndconf.mdc, hP);
	SelectObject(wndconf.mdc, hBr);
	for (int i = 0; i < ENEMIES_MAXNUM; i++) {
		if (!enemies[i].is_dead) {
			if (enemies[i].type == 1)
				Rectangle(wndconf.mdc, enemies[i].x, enemies[i].y + 10, enemies[i].x + (1 - (enemies[i].blood_up - enemies[i].blood) * 1.0 / enemies[i].blood_up) * 100, enemies[i].y + 20);
			if (enemies[i].type == 2)
				Rectangle(wndconf.mdc, enemies[i].x, enemies[i].y + 20, enemies[i].x + (1 - (enemies[i].blood_up - enemies[i].blood) * 1.0 / enemies[i].blood_up) * 100, enemies[i].y + 30);
		}
	}
	
	//BossѪ��
	if (!boss.is_dead) {
		SelectObject(wndconf.bufdc, boss_state);
		MyTransparentBlt(wndconf.mdc, 300, 0, 628, 82, wndconf.bufdc, 0, 0, 628, 82, RGB(255, 255, 255));

		hP = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hBr = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(wndconf.mdc, hP);
		SelectObject(wndconf.mdc, hBr);
		Rectangle(wndconf.mdc, 884 - (boss.blood_up - boss.blood) * 1.0 / boss.blood_up * 472, 40, 884, 52);
	}

	//��ɫ����
	SelectObject(wndconf.bufdc, skill_state);
	MyTransparentBlt(wndconf.mdc, 40, 115, 201, 72, wndconf.bufdc, 0, 0, 201, 72, RGB(255, 255, 255));

	//ͨ��
	if (win) {
		SelectObject(wndconf.bufdc, win_item);
		MyTransparentBlt(wndconf.mdc, 700, 380, 200, 200, wndconf.bufdc, win_num * 200, 0, 200, 200, RGB(0, 0, 0));
	}

	//ɾ�����ʻ�ˢ
	DeleteObject(hP);
	DeleteObject(hBr);
}