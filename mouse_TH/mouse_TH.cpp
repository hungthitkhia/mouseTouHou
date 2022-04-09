// mouse_TH.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "gm.h"
#pragma GCC optimize(2)
using namespace std;

extern HANDLE gameHandle;
extern int pid;
extern HWND windowHandle;
extern int normalSpeed;
extern int slowSpeed;
extern GameData currentGame;

bool SlowChanged = false, NormalChanged = false;
void setMouseSpeed()
{
	if (GetAsyncKeyState(VK_LSHIFT))
	{
		if (!SlowChanged)
			NormalChanged = !(SlowChanged = SystemParametersInfo(SPI_SETMOUSESPEED, 0, &normalSpeed, SPIF_SENDCHANGE));
	}
	else if (!NormalChanged)
	{
		SlowChanged = !(NormalChanged = SystemParametersInfo(SPI_SETMOUSESPEED, 0, &slowSpeed, SPIF_SENDCHANGE));
	}

}

int main()
{
    wchar_t out_text[512];
	init();
	if (normalSpeed == -1)
	{
		MessageBox(NULL, L"Failed to get mouse speed", NULL, MB_OK);
		return 0;
	}
	GetGameData();
	if (gameHandle != NULL)
	{
		swprintf_s(out_text, L"Game process PID: %d\nGame ID: %d\nDefault mouse speed: %d", pid, currentGame.id, normalSpeed);
		MessageBox(NULL, out_text, L"Load successful", MB_OK);
		while (1){
			setMouseSpeed();
			switch (currentGame.MouseControl())
			{
                case PAUSE_FLAG:
                    Clear();
                    printf("Game paused.");
                    break;
                case NOT_IN_GAME_FLAG:
                    Clear();
                    printf("Failed to get player coordinates, the game is most likely in the main menu.");
                    Sleep(5);
                    break;
                default:
                case NORMAL_FLAG:
                    break;
			}
		}
	}
}
