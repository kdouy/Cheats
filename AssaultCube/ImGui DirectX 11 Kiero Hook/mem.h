#pragma once
#include <vector>
#include <windows.h>

using namespace std;

namespace mem
{
	DWORD localPlayer = 0x109B74;
	DWORD entityList = 0x10F4F8;
	DWORD fieldOfView = 0x10F1C4;
	DWORD viewMatrix = 0x501AE8;

	DWORD cursor = 0x10A214;
	DWORD cursorTarget = 0x225;

	DWORD shootBoolean = 0x224;

	DWORD crosshairX = 0x40;
	DWORD crosshairY = 0x44;

	DWORD primaryAmmo = 0x150;
	DWORD primaryStoredAmmo = 0x128;

	DWORD secondaryAmmo = 0x13C;
	DWORD secondaryStoredAmmo = 0x114;

	DWORD health = 0xF8;
	DWORD team = 0x32C;
	DWORD kills = 0x1FC;

	DWORD fireRate = 0x178;

	DWORD currWeapon = 0x374;
	DWORD weaponData = 0xC;
	DWORD weaponName = 0x0;
	DWORD weaponRecoil = 0x122;

	DWORD moveX = 0x80;
	DWORD moveZ = 0x81;

	DWORD onAir = 0x69;

	DWORD knife = 0x384;
	DWORD knifeData = 0x18;
	DWORD knifeCooldown = 0x1;

	DWORD posX = 0x34;
	DWORD posY = 0x38;
	DWORD posZ = 0x3C;
}