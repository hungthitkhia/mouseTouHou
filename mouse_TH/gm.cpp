#include "pch.h"
#include "gm.h"
#define DATA_SIZE 21
HANDLE gameHandle;
int pid;
HWND windowHandle;
GameData data[DATA_SIZE];
int normalSpeed;
int slowSpeed;
GameData currentGame;
/*typedef NTSTATUS (NTAPI *_NtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);
_NtWriteVirtualMemory NtWriteVirtualMemory = (_NtWriteVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory");
template <typename T>
void WriteMemory(HANDLE ProcessHandle, DWORD BaseAddress, const T &Value)
{
    BYTE buffer[sizeof(T)];
    memcpy(buffer, &Value, sizeof(T));
    NtWriteVirtualMemory(ProcessHandle, (PVOID)BaseAddress, buffer, sizeof(T), NULL);
}*/
void init()
{
	if (!SystemParametersInfo(SPI_GETMOUSESPEED, 0, &normalSpeed, SPIF_SENDCHANGE))
        normalSpeed = -1;

	slowSpeed = floor(normalSpeed * 0.4f + 0.5f);

	data[0]  = GameData(6 ,		0x006CAA68, 0x0  , 0x006C6EC0, {L"th06.exe", L"東方紅魔郷.exe", L"搶曽峠杺嫿.exe"}, 1.0f  , GEN1_POSRECT, GEN1_STAGERECT);
	data[1]  = GameData(7 ,		0x004BE408, 0x0  , 0x00575AC8, {L"th07.exe"                                     }, 1.0f  , GEN1_POSRECT, GEN1_STAGERECT);
	data[2]  = GameData(8 ,		0x017D61AC, 0x0  , 0x017CE8E0, {L"th08.exe"                                     }, 1.0f  , GEN1_POSRECT, GEN1_STAGERECT);
	data[3]  = GameData(10,		0x00477834, 0x3CC, 0x00476F78, {L"th10.exe", L"th10c.exe"                       }, 100.0f);
	data[4]  = GameData(11,		-1		  , 0x0  , 0x004A7948, {L"th11.exe"                                     });
	data[5]  = GameData(12,     0x004B4514, 0x988, 0x004B2ED0, {L"th12.exe", L"th12c.exe"                       });
	data[6]  = GameData(13,		0x004C22C4, 0x5C4, 0x004C0A28, {L"th13.exe", L"th13c.exe"                       });
	data[7]  = GameData(14,		0x004DB67C, 0x5EC, 0x004D8F58, {L"th14.exe"                                     });
	data[8]  = GameData(15,		0x004E9BB8, 0x624, 0x004E73E8, {L"th15.exe"                                     });
	data[9]  = GameData(16,		0x004A6EF8, 0x61C, 0x004A5788, {L"th16.exe"                                     });
	data[10] = GameData(17,		0x004B77D0, 0x61C, -1        , {L"th17.exe"                                     });
	data[11] = GameData(18,		0x004CF410, 0x62C, -1        , {L"th18.exe"                                     });

	data[12] = GameData(9, 0x004A7D94, 0x1B88, 0x004B36B8, {L"th09.exe"}, 1.0f, {0.04f, 0.125f, 0.46f, 0.94125f}, {-136.0f, 136.0f, 32.0f, 436.0f});

	data[13] = GameData(95 , 0x004C3E34, 0x1E30, 0x004BCED8, {L"th095.exe"               }, 1.0f  , {0.215f, 0.125f, 0.785f, 0.94125f}, {-184.0f, 32.0f, 184.0f, 436.0f});
	data[14] = GameData(125, 0x004B68C8, 0x604 , 0x004B5308, {L"th125.exe", L"th125c.exe"}, 128.0f, SPINOFF_POSRECT);
	data[15] = GameData(128, 0x004B8A80, 0xED8 , 0x004B6F58, {L"th128.exe"               }, 128.0f, SPINOFF_POSRECT);
	data[16] = GameData(143, 0x004E6B88, 0x5EC , 0x004E40B0, {L"th143.exe"               }, 128.0f, SPINOFF_POSRECT);
	data[17] = GameData(165, 0x004B5654, 0x618 , 0x004B39D0, {L"th165.exe"               }, 128.0f, SPINOFF_POSRECT);

	data[18] = GameData(-100, 0x00FE2350, 0x0, -1, {L"thsg.exe"       }, 1.0f, {0.065f, 0.125f, 0.635f, 0.93125f}, {10.0f  , 374.0f, 32.0f  , 433.0f});
	data[19] = GameData(-101, 0x00A8CDF8, 0x0, -1, {L"东方海惠堂.exe"  }, 1.0f, {0.065f, 0.125f, 0.635f, 0.93125f}, {8.0f   , 376.0f, 16.0f  , 432.0f});
	data[20] = GameData(-102, 0x017D2EA0, 0x0, -1, {L"QP Shooting.exe"}, 1.0f, {0.19f , 0.03f , 0.81f , 0.99f   }, {-250.0f, 250.0f, -300.0f, 300.0f});
}

void GetGameData()
{
	for (int i = 0; i < DATA_SIZE; i++)
	{
		if (data[i].id == -1)
            continue;
        for(wstring name: data[i].exeName)
        {
            pid = FindProcessPid(name.c_str());
            if (pid)
                break;
        }
		if (!pid)
            continue;
		currentGame = data[i];
		break;
	}
	if ((gameHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid)) == NULL)
		MessageBox(NULL, L"Unable to get access to the game process.", NULL, MB_OK);

	windowHandle = ReturnWnd(pid);

	if(currentGame.id == 11)
        initSA();
}

void initSA()
{
    BYTE th11Chg1[12] = {0xE9, 0xD6, 0xA6, 0x05, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    BYTE th11Chg2[28] = {0x8B, 0x35, 0x34, 0xAE, 0x48, 0x00, 0xA1, 0x38, 0xAE, 0x48, 0x00, 0x89, 0xB5, 0x88, 0x08, 0x00, 0x00, 0x89, 0x85, 0x8C, 0x08, 0x00, 0x00, 0xE9, 0x15, 0x59, 0xFA, 0xFF};
    DWORD oldProtect, oldProtect2;
    VirtualProtect((LPVOID)0x0043073A, 0x20, PAGE_EXECUTE_READWRITE, &oldProtect);
    WriteProcessMemory(gameHandle, (LPVOID)0x0043073A, th11Chg1, sizeof(th11Chg1), NULL);
    VirtualProtect((LPVOID)0x0043073A, 0x20, oldProtect, &oldProtect2);

    VirtualProtect((LPVOID)0x0048AE15, 0x20, PAGE_EXECUTE_READWRITE, &oldProtect);
    WriteProcessMemory(gameHandle, (LPVOID)0x0048AE15, th11Chg2, sizeof(th11Chg2), NULL);
}

DWORD GameData::isInGame()
{
	DWORD ptPl = 0;
	switch (id)
	{
        case 9:
        case 95:
        case 10:
        case 12:
        case 125:
        case 128:
        case 13:
        case 14:
        case 143:
        case 15:
        case 16:
        case 17:
        case 18:
        case 165:
            ReadProcessMemory(gameHandle, (LPCVOID)playerPtr, &ptPl, sizeof(DWORD), NULL);
            break;
        case -102:
        case -101:
        case -100:
        case 11:
        case 8:
        case 7:
        case 6:
            ptPl = -1;
            break;
        default:
            ReadProcessMemory(gameHandle, (LPCVOID)playerPtr, &ptPl, sizeof(DWORD), NULL);
            break;
	}
	return ptPl;
}

bool GameData::isPause()
{
	float spd;
	if (!pausePtr)
        return 0;

	ReadProcessMemory(gameHandle, (float*)pausePtr, &spd, sizeof(DWORD), NULL);

	return (spd == 0.0f);
}

int GameData::MouseControl()
{
	DWORD ptPl;

	if (!(ptPl = isInGame()))
		return NOT_IN_GAME_FLAG;

	if (isPause())
		return PAUSE_FLAG;

	RECT windRect;
	POINT mousePos;
	GetCursorPos(&mousePos);
	GetWindowRect(windowHandle, &windRect);

	float relX, relY;
	relX = (float)(mousePos.x - windRect.left) / (float)(windRect.right - windRect.left);
	relY = (float)(mousePos.y - windRect.top) / (float)(windRect.bottom - windRect.top);

	relX = std::clamp(relX, positionBound.left, positionBound.right);
	relY = std::clamp(relY, positionBound.top, positionBound.bottom);

	relX = (relX - positionBound.left) / positionBound.width;
	relY = (relY - positionBound.top) / positionBound.height;
    /*double finalX, finalY;
    finalX = (stageBound.left + relX * stageBound.width) * ratio;
    finalY = (stageBound.top + relY * stageBound.height) * ratio;*/

	switch (id)
	{
        case 9:
        case 95:
        {
            float finalX, finalY;
            finalX = (stageBound.left + relX * stageBound.width) * ratio;
            finalY = (stageBound.top + relY * stageBound.height) * ratio;
            WriteProcessMemory(gameHandle, (LPVOID)(ptPl + offsetPtr), &finalX, sizeof(DWORD), NULL);
            WriteProcessMemory(gameHandle, (LPVOID)(ptPl + offsetPtr + 4), &finalY, sizeof(DWORD), NULL);
            //WriteMemory(gameHandle, ptPl + offsetPtr, (float)finalX);
            //WriteMemory(gameHandle, ptPl + offsetPtr + 4, (float)finalY);
        }
        break;
        case -100:
        case -101:
        {
            double finalX, finalY;
            finalX = (stageBound.left + relX * stageBound.width) * ratio;
            finalY = (stageBound.top + relY * stageBound.height) * ratio;
            WriteProcessMemory(gameHandle, (LPVOID)playerPtr, &finalX, sizeof(DWORD), NULL);
            WriteProcessMemory(gameHandle, (LPVOID)(playerPtr + 8), &finalY, sizeof(DWORD), NULL);
            //WriteMemory(gameHandle, playerPtr, finalX);
            //WriteMemory(gameHandle, playerPtr + 8, finalY);
        }
        break;
        case 11:
        {
            int finalX, finalY;
            finalX = (stageBound.left + relX * stageBound.width) * ratio;
            finalY = (stageBound.top + relY * stageBound.height) * ratio;
            WriteProcessMemory(gameHandle, (LPVOID)0x0048AE34, &finalX, sizeof(DWORD), NULL);
            WriteProcessMemory(gameHandle, (LPVOID)0x0048AE38, &finalY, sizeof(DWORD), NULL);
            //WriteMemory(gameHandle, 0x0048AE34, (int)finalX);
            //WriteMemory(gameHandle, 0x0048AE38, (int)finalY);
        }
            break;
        case 10:
        case 12:
        case 125:
        case 128:
        case 13:
        case 14:
        case 143:
        case 15:
        case 16:
        case 17:
        case 18:
        case 165:
        {
            int finalX, finalY;
            finalX = (stageBound.left + relX * stageBound.width) * ratio;
            finalY = (stageBound.top + relY * stageBound.height) * ratio;
            WriteProcessMemory(gameHandle, (LPVOID)(ptPl + offsetPtr), &finalX, sizeof(DWORD), NULL);
            WriteProcessMemory(gameHandle, (LPVOID)(ptPl + offsetPtr + 4), &finalY, sizeof(DWORD), NULL);
            //WriteMemory(gameHandle, ptPl + offsetPtr, (int)finalX);
            //WriteMemory(gameHandle, ptPl + offsetPtr + 4, (int)finalY);
        }
            break;
        case -102:
        case 8:
        case 7:
        case 6:
        {
            float finalX, finalY;
            finalX = (stageBound.left + relX * stageBound.width) * ratio;
            finalY = (stageBound.top + relY * stageBound.height) * ratio;
            WriteProcessMemory(gameHandle, (LPVOID)playerPtr, &finalX, sizeof(DWORD), NULL);
            WriteProcessMemory(gameHandle, (LPVOID)(playerPtr + 4), &finalY, sizeof(DWORD), NULL);
            //WriteMemory(gameHandle, playerPtr, (float)finalX);
            //WriteMemory(gameHandle, playerPtr + 4, (float)finalY);
        }
            break;
        default:
            break;
	}

	return NORMAL_FLAG;
}
