#pragma once
#include<vector>
#include<windef.h>
#include<math.h>
#include<stdio.h>
#include<string>
#include "Proc.h"
#define GEN1_POSRECT {0.065f, 0.085f, 0.635f, 0.93125f}
#define GEN1_STAGERECT {8.0f, 16.0f, 376.0f, 432.0f}
#define SPINOFF_POSRECT {0.215f, 0.125f, 0.785f, 0.93125f}
using std::vector;
using std::wstring;
void GetGameData();
void init();
void initSA();
struct Rect
{
    float left, top, right, bottom, width, height;
    Rect(float a, float b, float c, float d): left(a), top(b), right(c), bottom(d), width(c - a), height(d - b) {}
    Rect(): Rect(0, 0, 0, 0) {}
};
struct GameData
{
    int id;
    DWORD playerPtr, offsetPtr, pausePtr;
    vector<wstring> exeName;
    float ratio;
    Rect positionBound, stageBound;

	int MouseControl();
	bool isPause();
	DWORD isInGame();

	GameData(int num, DWORD ptr, DWORD offset, DWORD pauseAdd, vector<wstring> name = {}, float ratio = 128.0f, Rect pos = {0.065f, 0.125f, 0.635f, 0.93125f}, Rect stage = {-184.0f, 32.0f, 184.0f, 432.0f})
	: id(num), playerPtr(ptr), offsetPtr(offset), pausePtr(pauseAdd), exeName(name), ratio(ratio), positionBound(pos), stageBound(stage)
    {}
	GameData(): id(0) {}
};

