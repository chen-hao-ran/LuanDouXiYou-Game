#pragma once

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>
#include "Tools.h"
//#include "WindowConfig.h"
//#include "MainScene.h"
//#include "Scene.h"
//#include "Role.h"
//#include "Enemy.h"

#define MAINSCENE 10
#define FAILED 11
#define LEFT 18
#define RIGHT 19
#define MOVE 20
#define RUN 21
#define ATTACK 22
#define STOP 23
#define SKILL 24
#define INJURIED 25
#define LEVEL_MAXNUM 3 //�ؿ�����
#define ENEMIES_MAXNUM 7 //ÿ��С���е�С������ 
#define HANDLE_MAXNUM 100000 //�ط���������

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
bool keyY;
bool keyL;
bool keyU;

HPEN hP; //����
HBRUSH hBr; //��ˢ