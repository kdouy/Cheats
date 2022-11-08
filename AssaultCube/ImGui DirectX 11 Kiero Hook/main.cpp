#include "includes.h"
#include "functions.h"
#include "mem.h"
#include <vector>
#include <thread>
#include <TlHelp32.h>
#include <tchar.h>
#include <string>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace std;

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
bool uiVisible = true;
int width = 1920;
int height = 1080;
float Matrix[16];
Vec2 lastPos = { 0, 0 };

void themeInitilization()
{
	auto& io = ImGui::GetIO();
	// io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 16.5f, NULL, io.Fonts->GetGlyphRangesJapanese());

	ImGuiStyle* style = &ImGui::GetStyle();

	// style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	// style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.0f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}


void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	themeInitilization();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = uiVisible;

	if (uMsg == WM_KEYUP && wParam == VK_INSERT)
		uiVisible = !uiVisible;
		 
	if (uiVisible)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
			return true;
		}

		switch (uMsg) {
		case WM_LBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[0];
			return 0;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[1];
			return 0;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = !io.MouseDown[2];
			return 0;
		case WM_MOUSEWHEEL:
			return 0;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return 0;
		}
	}
		
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

DWORD moduleBase = (DWORD)GetModuleHandleA(NULL);
DWORD localPlayerAddress = *(DWORD*)(moduleBase + mem::localPlayer);

bool esp_enabled = false;
bool esp_boxes = false;
bool esp_healthbars = false;
bool esp_names = false;
bool esp_distances = false;
bool aimbot = false;
bool aimbotTeammates = false;
bool esp_teammates = false;
bool fovEnabled = false;
bool infAmmo = false;
bool infHealth = false;
bool noRecoil = false;
bool lowFireRate = false;
bool speedEnabled = false;
bool rapidKnife = false;
bool infJump = false;
int fovSize = 100;
int speedAmount = 2;
int killValue = 0;
float enemyColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float teamColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
float fovColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
int selectedTeam = 0;
const char* teams[]{ "Red", "Blue" };

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (uiVisible)
	{
		ImGui::SetNextWindowPos(ImVec2(15, 15));
		ImGui::SetNextWindowSize(ImVec2(500, 300));
		ImGui::Begin("Vaultality", &uiVisible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (ImGui::BeginTabBar(""))
		{
			if (ImGui::BeginTabItem("Aimbot"))
			{
				ImGui::Checkbox("Enabled", &aimbot); 
				ImGui::Checkbox("Teammates", &aimbotTeammates);
				ImGui::Checkbox("FOV Circle", &fovEnabled);
				ImGui::SliderInt("Size", &fovSize, 1, 350);
				ImGui::ColorEdit4("Color", fovColor);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Visuals"))
			{
				ImGui::Checkbox("Enabled", &esp_enabled);
				ImGui::Checkbox("Teammates", &esp_teammates);
				ImGui::Checkbox("Boxes", &esp_boxes);
				ImGui::Checkbox("Health Bars", &esp_healthbars);
				ImGui::Checkbox("Names", &esp_names);
				ImGui::Checkbox("Distance", &esp_distances);
				ImGui::Separator();
				ImGui::ColorEdit4("Enemy Color", enemyColor);
				ImGui::ColorEdit4("Team Color", teamColor);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Misc"))
			{
				ImGui::Checkbox("Speed", &speedEnabled);
				ImGui::SliderInt("Factor", &speedAmount, 1, 10);
				ImGui::Checkbox("Inf Jump", &infJump);
				ImGui::Separator();
				ImGui::Checkbox("Infinite Ammo", &infAmmo);
				ImGui::Checkbox("Infinite Health", &infHealth);
				ImGui::Separator();
				ImGui::Checkbox("Rapid Knife", &rapidKnife);
				ImGui::Checkbox("Low Firerate", &lowFireRate);
				ImGui::Separator();
				ImGui::Checkbox("No Recoil", &noRecoil);
				if (ImGui::Combo("Set Team", &selectedTeam, teams, IM_ARRAYSIZE(teams)))
				{
					*((int*)(localPlayerAddress + mem::team)) = selectedTeam;
				}
				if (ImGui::InputInt("Set Kill", &killValue))
				{
					*((int*)(localPlayerAddress + mem::kills)) = killValue;
				}
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	if (lowFireRate)
	{
		*((int*)(localPlayerAddress + mem::fireRate)) = 0;
	}

	if (noRecoil)
	{
		functions::nop((BYTE*)(moduleBase + 0x63786), 10);
	}
	else
	{
		functions::patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
	}

	if (infAmmo)
	{
		*((int*)(localPlayerAddress + mem::primaryAmmo)) = 9e9;
		*((int*)(localPlayerAddress + mem::secondaryAmmo)) = 9e9;
	}

	if (infHealth)
	{
		*((int*)(localPlayerAddress + mem::health)) = 9e9;
	}

	width = ImGui::GetIO().DisplaySize.x;
	height = ImGui::GetIO().DisplaySize.y;
	memcpy(&Matrix, (PBYTE*)(mem::viewMatrix), sizeof(Matrix));

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

	auto draw = ImGui::GetWindowDrawList();

	// *(int*)(localPlayerAddress + mem::fireRate) = 0;

	if (fovEnabled)
	{
		draw->AddCircle(ImVec2(width / 2, height / 2), fovSize, functions::reverseTableColor(fovColor), 100, 1.0f);
	}

	if (infJump)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			*(DWORD*)(localPlayerAddress + mem::onAir) = 1;
		}
	}

	if (rapidKnife)
	{
		DWORD getKnife = *(DWORD*)(localPlayerAddress + mem::knife);
		if (getKnife != NULL)
		{
			DWORD getKnifeData = *(DWORD*)(getKnife + mem::knifeData);
			if (getKnifeData != NULL)
			{
				*(BYTE*)(getKnifeData + mem::knifeCooldown) = 0;
			}
		}
	}

	if (speedEnabled)
	{
		DWORD X = *(DWORD*)(localPlayerAddress + mem::moveX);
		DWORD Z = *(DWORD*)(localPlayerAddress + mem::moveZ);

		if (GetAsyncKeyState(0x57) && X == 1) // W
		{
			*(DWORD*)(localPlayerAddress + mem::moveX) = 1 + speedAmount;
		}
		else if (GetAsyncKeyState(0x53) && X == 255) // S
		{
			*(DWORD*)(localPlayerAddress + mem::moveX) = 255 - speedAmount;
		}
		else if (GetAsyncKeyState(0x41) && Z == 1) // A
		{
			*(DWORD*)(localPlayerAddress + mem::moveZ) = 1 + speedAmount;
		}
		else if (GetAsyncKeyState(0x44) && Z == 255) // D
		{
			*(DWORD*)(localPlayerAddress + mem::moveZ) = 255 - speedAmount;
		}
		else if (GetAsyncKeyState(0x57) && GetAsyncKeyState(0x41) && X == 257 && Z == 1) // W A
		{
			*(DWORD*)(localPlayerAddress + mem::moveX) = 256 + speedAmount;
		}
		else if (GetAsyncKeyState(0x57) && GetAsyncKeyState(0x44) && X == 65281 && Z == 255) // W D
		{
			*(DWORD*)(localPlayerAddress + mem::moveX) = 65281 + speedAmount;
		}
		else if (GetAsyncKeyState(0x53) && GetAsyncKeyState(0x41) && X == 511 && Z == 1) // S A
		{
			*(DWORD*)(localPlayerAddress + mem::moveX) = 511 + speedAmount;
		}
		else if (GetAsyncKeyState(0x53) && GetAsyncKeyState(0x44) && X == 65535 && Z == 255) // S D
		{
			*(DWORD*)(localPlayerAddress + mem::moveX) = 65535 + speedAmount;
		}
	}
 
	if (esp_enabled)
	{
		DWORD amountOfPlayers = *(DWORD*)(0x50F500);

		for (int i = 1; i < amountOfPlayers; ++i)
		{
			DWORD entityAddress = *(DWORD*)(moduleBase + mem::entityList);
			DWORD entity = *(DWORD*)(entityAddress + (0x4 * i));

			if (entity != NULL)
			{
				float entityHealth = *(int*)(entity + 0xF8);

				if (entityHealth > 0.0f)
				{
					float enemyXHead = *(float*)(entity + 0x4);
					float enemyYHead = *(float*)(entity + 0x8);
					float enemyZHead = *(float*)(entity + 0xC);

					float localXHead = *(float*)(localPlayerAddress + 0x4);
					float localYHead = *(float*)(localPlayerAddress + 0x8);
					float localZHead = *(float*)(localPlayerAddress + 0xC);

					Vec3 localPos = { localXHead, localYHead, localZHead };
					Vec3 enemyPos = { enemyXHead, enemyYHead, enemyZHead };
					Vec2 localPos2D;
					Vec2 entPos2D;

					int entityTeam = *(int*)(entity + mem::team);
					int localTeam = *(int*)(localPlayerAddress + mem::team);

					functions::WorldToScreen(localPos, localPos2D, Matrix, width, height);

					DWORD getWeapon = *(DWORD*)(entity + mem::currWeapon);
					DWORD getWeaponData = *(DWORD*)(getWeapon + mem::weaponData);

					if (functions::WorldToScreen(enemyPos, entPos2D, Matrix, width, height))
					{
						float distance3D = functions::GetDistance3D(localPos, enemyPos);
						float fovValue = *(float*)(moduleBase + mem::fieldOfView);

						Vec2 size = { 1200 , 4000 };

						float fovValueReal = fovValue - 75;
						float heightFactor = size.y - ((fovValueReal / 45) * (size.y / 2));
						float widthFactor = size.x - ((fovValueReal / 45) * (size.x / 2));

						float height = heightFactor / distance3D;
						float width = widthFactor / distance3D;

						float x = entPos2D.x;
						float y = entPos2D.y;

						if (esp_boxes)
						{
							if (localTeam != entityTeam)
							{
								draw->AddRect(ImVec2(x - width, y - (height / 4)), ImVec2(x + width, y + height), ImColor(0, 0, 0, 255), 0, 3.0f);
								draw->AddRect(ImVec2(x - width, y - (height / 4)), ImVec2(x + width, y + height), functions::reverseTableColor(enemyColor), 0, 1.0f);
							}
							else
							{
								if (esp_teammates)
								{
									draw->AddRect(ImVec2(x - width, y - (height / 4)), ImVec2(x + width, y + height), ImColor(0, 0, 0, 255), 0, 3.0f);
									draw->AddRect(ImVec2(x - width, y - (height / 4)), ImVec2(x + width, y + height), functions::reverseTableColor(teamColor), 0, 1.0f);
								}
								
							}
						}

						if (esp_healthbars)
						{
							if (localTeam != entityTeam || esp_teammates)
							{
								float t = 1.0f - (min(max(entityHealth, 0), 100) / 100.0f);
								float healthBarHeight = y - ((height / 4) - (t * (height + (height / 4))));

								draw->AddRectFilled(ImVec2((x - width) - 6, y - (height / 4)), ImVec2((x - width) - 4, y + height),
									ImColor(
										0,
										0,
										0,
										255
									),
									0, 1.0f);

								draw->AddRectFilled(ImVec2((x - width) - 6, healthBarHeight), ImVec2((x - width) - 4, y + height),
									ImColor(
										(int)(255 - ((entityHealth / 100) * 255)),
										(int)((entityHealth / 100) * 255),
										0,
										255
									),
									0, 1.0f);
								string healthText = to_string((int)entityHealth);
								ImVec2 textNameSize = ImGui::CalcTextSize(healthText.c_str());
								draw->AddText(ImVec2(((x - width) - 10) - textNameSize.x, healthBarHeight), ImColor(255, 255, 255, 255), healthText.c_str());
							}
						}

						if (esp_names)
						{
							if (localTeam != entityTeam || esp_teammates)
							{
								char* getName = (char*)(entity + 0x225);
								ImVec2 textNameSize = ImGui::CalcTextSize(getName);
								draw->AddText(ImVec2(x - (textNameSize.x / 2), (y - (height / 4)) - 15), ImColor(255, 255, 255, 255), getName);
							}
						}

						if (esp_distances)
						{
							if (localTeam != entityTeam || esp_teammates)
							{
								char* getWeaponName = (char*)(getWeaponData + mem::weaponName);
								string getDistance = "[" + (string)getWeaponName + "] [" + to_string((int)distance3D) + "ms]";
								ImVec2 textSize = ImGui::CalcTextSize(getDistance.c_str());
								draw->AddText(ImVec2(x - (textSize.x / 2), (y + height) + textSize.y - 10), ImColor(255, 255, 255, 255), getDistance.c_str());
							}
						}
					}
				}
			}
		}
	}

	ImGui::End();
	
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

void aimbotFunc();
void silentAimFunc();
void triggerBotFunc();

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		thread silentAimThread(silentAimFunc);
		thread aimbotThread(aimbotFunc);
		thread triggerBotThread(triggerBotFunc);
		silentAimThread.detach();
		aimbotThread.detach();
		triggerBotThread.detach();
		break;
	}
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}

void silentAimFunc()
{
	/*
	while (true)
	{
		unsigned char getIsShooting = *(unsigned char*)(localPlayerAddress + mem::shootBoolean);
		if (getIsShooting != NULL)
		{
			float lastDistance = 100000;
			char* name = NULL;
			Vec2 targetPos = { 0, 0 };
			DWORD amountOfPlayers = *(DWORD*)(0x50F500);

			for (int i = 1; i < amountOfPlayers; ++i)
			{
				DWORD entityAddress = *(DWORD*)(moduleBase + mem::entityList);
				DWORD entity = *(DWORD*)(entityAddress + (0x4 * i));

				if (entity != NULL)
				{
					float entityHealth = *(int*)(entity + 0xF8);

					if (entityHealth > 0.0f)
					{
						float enemyXHead = *(float*)(entity + 0x4);
						float enemyYHead = *(float*)(entity + 0x8);
						float enemyZHead = *(float*)(entity + 0xC);

						float localXHead = *(float*)(localPlayerAddress + 0x4);
						float localYHead = *(float*)(localPlayerAddress + 0x8);
						float localZHead = *(float*)(localPlayerAddress + 0xC);

						Vec3 localPos = { localXHead, localYHead, localZHead };
						Vec3 enemyPos = { enemyXHead, enemyYHead, enemyZHead };
						Vec2 entPos2D;

						int entityTeam = *(int*)(entity + mem::team);
						int localTeam = *(int*)(localPlayerAddress + mem::team);

						if (functions::WorldToScreen(enemyPos, entPos2D, Matrix, width, height) && (localTeam != entityTeam || aimbotTeammates))
						{
							float mouseX = *(float*)(localPlayerAddress + mem::crosshairX);
							float mouseY = *(float*)(localPlayerAddress + mem::crosshairY);

							Vec2 mouse = { mouseX, mouseY };
							Vec2 enemyAngle = functions::CalcAngle(localPos, enemyPos);

							float distance2D = functions::GetDistance2D(mouse, enemyAngle);

							if (lastDistance > distance2D && fovSize > distance2D)
							{
								lastDistance = distance2D;
								targetPos = { enemyAngle.x, enemyAngle.y };
								name = (char*)(entity + 0x225);
							}
						}
					}
				}
			}

			if (name != NULL)
			{
				*((float*)(localPlayerAddress + mem::crosshairX)) = targetPos.x;
				*((float*)(localPlayerAddress + mem::crosshairY)) = targetPos.y;

				*((float*)(localPlayerAddress + mem::crosshairX)) = lastPos.x;
				*((float*)(localPlayerAddress + mem::crosshairY)) = lastPos.y;
			}
		}
		else
		{
			lastPos.x = *((float*)(localPlayerAddress + mem::crosshairX));
			lastPos.y = *((float*)(localPlayerAddress + mem::crosshairY));
		}
	}
	*/
}

DWORD old_protect;

void triggerBotFunc()
{
	while (true)
	{
		/*
		DWORD getCursor = *(DWORD*)(moduleBase + mem::cursor);
		char* getCursorTarget = (char*)(getCursor + mem::cursorTarget);

		if (getCursorTarget != NULL)
		{
			DWORD amountOfPlayers = *(DWORD*)(0x50F500);

			for (int i = 1; i < amountOfPlayers; ++i)
			{
				DWORD entityAddress = *(DWORD*)(moduleBase + mem::entityList);
				DWORD entity = *(DWORD*)(entityAddress + (0x4 * i));

				if (entity != NULL)
				{
					char* getName = (char*)(entity + 0x225);
					int entityTeam = *(int*)(entity + mem::team);
					int localTeam = *(int*)(localPlayerAddress + mem::team);
					if (getName == getCursorTarget && entityTeam != localTeam)
					{
						
					}
					else
					{
						
					}
				}
			}
		}
		*/
	}
	Sleep(1);
}

void aimbotFunc()
{
	while (true)
	{
		if (aimbot && GetAsyncKeyState(VK_RBUTTON))
		{
			float lastDistance = 100000;
			char* name = NULL;
			Vec2 targetPos = { 0, 0 };
			DWORD amountOfPlayers = *(DWORD*)(0x50F500);

			for (int i = 1; i < amountOfPlayers; ++i)
			{
				DWORD entityAddress = *(DWORD*)(moduleBase + mem::entityList);
				DWORD entity = *(DWORD*)(entityAddress + (0x4 * i));

				if (entity != NULL)
				{
					float entityHealth = *(int*)(entity + 0xF8);

					if (entityHealth > 0.0f)
					{
						float enemyXHead = *(float*)(entity + 0x4);
						float enemyYHead = *(float*)(entity + 0x8);
						float enemyZHead = *(float*)(entity + 0xC);

						float localXHead = *(float*)(localPlayerAddress + 0x4);
						float localYHead = *(float*)(localPlayerAddress + 0x8);
						float localZHead = *(float*)(localPlayerAddress + 0xC);

						Vec3 localPos = { localXHead, localYHead, localZHead };
						Vec3 enemyPos = { enemyXHead, enemyYHead, enemyZHead };
						Vec2 entPos2D;

						int entityTeam = *(int*)(entity + mem::team);
						int localTeam = *(int*)(localPlayerAddress + mem::team);

						if (functions::WorldToScreen(enemyPos, entPos2D, Matrix, width, height) && (localTeam != entityTeam || aimbotTeammates))
						{
							float mouseX = *(float*)(localPlayerAddress + mem::crosshairX);
							float mouseY = *(float*)(localPlayerAddress + mem::crosshairY);

							Vec2 mouse = { mouseX, mouseY };
							Vec2 enemyAngle = functions::CalcAngle(localPos, enemyPos);

							float distance2D = functions::GetDistance2D(mouse, enemyAngle);

							if (lastDistance > distance2D && fovSize > distance2D)
							{
								lastDistance = distance2D;
								targetPos = { enemyAngle.x, enemyAngle.y };
								name = (char*)(entity + 0x225);
							}
						}
					}
				}
			}

			if (name != NULL)
			{
				*((float*)(localPlayerAddress + mem::crosshairX)) = targetPos.x;
				*((float*)(localPlayerAddress + mem::crosshairY)) = targetPos.y;
			}

			Sleep(1);
		}
	}
}

/*
* Vec2 target = nearestToCrosshair();
if (target.x != 0 && target.y != 0)
		{
			DWORD mouseX = functions::FindDMAAddy(localPlayerAddress, { mem::crosshairX });
			DWORD mouseY = functions::FindDMAAddy(localPlayerAddress, { mem::crosshairY });

			*(float*)mouseX = target.x;
			*(float*)mouseY = target.y;
		}
		*/
