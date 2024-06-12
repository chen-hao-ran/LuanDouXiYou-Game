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
#define LEVEL_MAXNUM 3 //关卡数量
#define ENEMIES_MAXNUM 7 //每个小怪中的小怪数量 
#define HANDLE_MAXNUM 100000 //回放最多操作数

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

HPEN hP; //画笔
HBRUSH hBr; //画刷