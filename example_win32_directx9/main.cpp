#include "main.h"
#include <Windows.h>
#include <vector>
#include <shellapi.h>
#include "MemoryManager.h"
#include "Settings.h"
#include "Config.h"
#include "Renderer.h"
#include "Structs.h"
#include "Offsets.h"

MemoryManager* m;

int Page = 0;
int SelectedGroup = 0;
int SelectedWeapon = 0;
int SelectedWeaponID = 0;
bool lastAutoPistolState = 0;

int WeaponID = 0;
int PlayerTeam;
View view;
int InMenu;
float fovscale1;
float fovscale2;
float recoil;

std::vector<float> Anims;
std::vector<float> OldAnims;
std::vector<float> OldOldAnims;
std::vector<Vector3> Targets;
std::vector<Vector3> TargetsWS;
std::vector<std::string> TargetModels;
std::vector<std::string> TargetNames;

bool SetMenuKey = false;
std::string MenuKeyLabel = "INSERT";
bool SetAimbotKey = false;
std::string AimbotKeyLabel = "MOUSE 1";
bool SetAimbot2Key = false;
std::string Aimbot2KeyLabel = "MOUSE 2";
bool SetBhopKey = false;
std::string BhopKeyLabel = "SPACE";
bool SetDDrunKey = false;
std::string DDrunKeyLabel = "ALT";

void InitCheat()
{
	std::string configDir = std::string(getenv("appdata")) + std::string("\\INTERIUM");
	CreateDirectory(configDir.c_str(), 0);
	configDir = configDir + std::string("\\CS16Ext");
	CreateDirectory(configDir.c_str(), 0);

	Sleep(100);
	//	LoadConfig("KleskBY.ini");
	m = new MemoryManager;
	DWORD PID = 0;
	DWORD client = getModuleAddress(PID, "client.dll");
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) return 0; // Disable ALT application menu
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Vector3 W2S(Vector3 WorldPos)
{
	Vector3 screen = Vector3(0, 0, 0);
	Vector3 vector = Vector3(0, 0, 0);
	Vector3 LocalPlayerPos = Vector3(view.vOrigin.x, view.vOrigin.y, view.vOrigin.z - 12.f);
	Vector3 worldLocation = WorldPos - LocalPlayerPos;
	vector.x = worldLocation.Dot(view.vRight);
	vector.y = worldLocation.Dot(view.vUpward);
	vector.z = worldLocation.Dot(view.vForward);
	if ((double)vector.z >= 0.01)
	{
		screen.x = (float)(Width / 2) + (float)(Width / 2) / vector.z * fovscale1 * vector.x;
		screen.y = (float)(Height / 2) - (float)(Height / 2) / vector.z * fovscale2 * vector.y;
		return screen;
	}
	else return Vector3(0, 0, 0);
}

void Hack()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		PlayerTeam = m->ReadMem<int>(m->cDll.base + Offsets::PlayerTeam);
		InMenu = m->ReadMem<int>(m->eDll.base + Offsets::InMenu);
		fovscale1 = m->ReadMem<float>(m->eDll.base + Offsets::fovscale1); //hw.dll+EC80E0
		fovscale2 = m->ReadMem<float>(m->eDll.base + Offsets::fovscale2); //hw.dll+EC80F4
		view = m->ReadMem<View>(m->cDll.base + Offsets::view);
		recoil = m->ReadMem<float>(m->eDll.base + Offsets::recoil);
		WeaponID = m->ReadMem<int>(m->eDll.base + Offsets::WeaponID);

		Aimbot::FOV = Weapons[WeaponID].FOV;
		Aimbot::Smooth = Weapons[WeaponID].Smootch;
		Aimbot::RCS = Weapons[WeaponID].RCS;

		Targets.clear();
		Anims.clear();
		for (int i = 0; i < 30; i++)
		{
			float AnimState = m->ReadMem<float>(m->eDll.base + Offsets::AnimState + i * 592);
			Anims.push_back(AnimState);
			Vector3 Posithion = m->ReadMem<Vector3>(m->eDll.base + Offsets::Posithion + i * 592); // 0x04A42A60
			Targets.push_back(Posithion);
		}

		if (GetForegroundWindow() == FindWindow("SDL_app", NULL) && !InMenu && !ShowMenu)
		{
			if (MISC::Bhop && GetAsyncKeyState(KEYS::BhopKey))
			{
				int OnGround = m->ReadMem<int>(m->eDll.base + Offsets::OnGround);
				if (OnGround == 1)
				{
					//m->WriteMem<int>(m->cDll.base + Offsets::dwForceJump, 4);
					//std::this_thread::sleep_for(std::chrono::milliseconds(5));
					m->WriteMem<int>(m->cDll.base + Offsets::dwForceJump, 5);
					std::this_thread::sleep_for(std::chrono::milliseconds(5));
					m->WriteMem<int>(m->cDll.base + Offsets::dwForceJump, 4);
				}
			}
			if (MISC::AutoPistol)
			{
				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (WeaponID == 17 || WeaponID == 16 || WeaponID == 1 || WeaponID == 26 || WeaponID == 10 || WeaponID == 11)
					{
						m->WriteMem<int>(m->cDll.base + Offsets::dwForceAttack, 5);
						std::this_thread::sleep_for(std::chrono::milliseconds(15));
						m->WriteMem<int>(m->cDll.base + Offsets::dwForceAttack, 4);
					}
				}
				else m->WriteMem<int>(m->cDll.base + Offsets::dwForceAttack, 4);
			}

			if (MISC::DDrun && GetAsyncKeyState(KEYS::DDrunKey))
			{
				int OnGround = m->ReadMem<int>(m->eDll.base + Offsets::OnGround);
				if (OnGround == 1)
				{
					m->WriteMem<int>(m->cDll.base + Offsets::dwForceDuck, 5);
					std::this_thread::sleep_for(std::chrono::milliseconds(2));
					m->WriteMem<int>(m->cDll.base + Offsets::dwForceDuck, 4);
				}
			}
		}
		if (GetAsyncKeyState(KEYS::MenuKey))
		{
			if (!ShowMenu)
			{
				if (WeaponID == 17) { SelectedGroup = 0; SelectedWeapon = 0; }
				if (WeaponID == 16) { SelectedGroup = 0; SelectedWeapon = 1; }
				if (WeaponID == 1 ) { SelectedGroup = 0; SelectedWeapon = 2; }
				if (WeaponID == 26) { SelectedGroup = 0; SelectedWeapon = 3; }
				if (WeaponID == 10) { SelectedGroup = 0; SelectedWeapon = 4; }
				if (WeaponID == 11) { SelectedGroup = 0; SelectedWeapon = 5; }

				if (WeaponID == 7) { SelectedGroup = 1; SelectedWeapon = 0; }
				if (WeaponID == 23) { SelectedGroup = 1; SelectedWeapon = 1; }
				if (WeaponID == 19) { SelectedGroup = 1; SelectedWeapon = 2; }
				if (WeaponID == 12) { SelectedGroup = 1; SelectedWeapon = 3; }
				if (WeaponID == 30) { SelectedGroup = 1; SelectedWeapon = 4; }

				if (WeaponID == 14) { SelectedGroup = 2; SelectedWeapon = 0; }
				if (WeaponID == 15) { SelectedGroup = 2; SelectedWeapon = 1; }
				if (WeaponID == 28) { SelectedGroup = 2; SelectedWeapon = 2; }
				if (WeaponID == 22) { SelectedGroup = 2; SelectedWeapon = 3; }
				if (WeaponID == 3) { SelectedGroup = 2; SelectedWeapon = 4; }
				if (WeaponID == 27) { SelectedGroup = 2; SelectedWeapon = 5; }
				if (WeaponID == 8) { SelectedGroup = 2; SelectedWeapon = 6; }
				if (WeaponID == 18) { SelectedGroup = 2; SelectedWeapon = 7; }
				if (WeaponID == 24) { SelectedGroup = 2; SelectedWeapon = 8; }
				if (WeaponID == 13) { SelectedGroup = 2; SelectedWeapon = 9; }

				if (WeaponID == 21) { SelectedGroup = 3; SelectedWeapon = 0; }
				if (WeaponID == 5) { SelectedGroup = 3; SelectedWeapon = 1; }
				if (WeaponID == 20) { SelectedGroup = 3; SelectedWeapon = 2; }

				SetForegroundWindow(g_hwnd);
				SetWindowLong(g_hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
				SetForegroundWindow(g_hwnd);
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				SetForegroundWindow(g_hwnd);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);	
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				SetForegroundWindow(g_hwnd);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
			else
			{
				SetForegroundWindow(GameHWND);
				SetWindowLong(g_hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
				SetForegroundWindow(GameHWND);
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				SetForegroundWindow(GameHWND);
				mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				SetForegroundWindow(GameHWND);
				mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
			}
			ShowMenu = !ShowMenu;

			while (GetAsyncKeyState(KEYS::MenuKey)) 
			{
				if (!ShowMenu) SetForegroundWindow(g_hwnd);
				else SetForegroundWindow(GameHWND);
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
		else if (GetAsyncKeyState(VK_END)) exit(1);
	}
}

void Updater()
{
	while (true)
	{
		GameHWND = FindWindow("SDL_app", NULL);
		if (GameHWND)
		{
			GetWindowRect(GameHWND, &tSize);
			Width = tSize.right - tSize.left;
			Height = tSize.bottom - tSize.top;
			DWORD dwStyle = GetWindowLong(GameHWND, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				tSize.top += 23;
				Height -= 23;
			}
			MoveWindow(g_hwnd, tSize.left, tSize.top + yoffset, Width, Height, true);
		}
		else
		{
			m->~MemoryManager();
			delete m;
			Sleep(3000);
			GameHWND = FindWindow("SDL_app", NULL);
			if (GameHWND) 
			{
				InitCheat();
				DWORD PID = 0;
				GetWindowThreadProcessId(GameHWND, &PID);
				DWORD client = getModuleAddress(PID, "client.dll");
			}
			else exit(1);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		OldOldAnims.clear();
		for (int i = 0; i < 30; i++)
		{
			float AnimState = m->ReadMem<float>(m->eDll.base + Offsets::AnimState + i * 592);
			OldOldAnims.push_back(AnimState);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		TargetModels.clear();
		for (int i = 0; i < 30; i++)
		{
			char PlayerModel[3];
			m->Read(m->eDll.base + Offsets::Model + i * 592, PlayerModel, 3);
			std::string Model = PlayerModel;
			if (strlen(Model.c_str()) > 3) Model.substr(0, Model.size() - 2);
			TargetModels.push_back(Model);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		TargetNames.clear();
		for (int i = 0; i < 30; i++)
		{
			char PlayerName[31];
			m->Read(m->eDll.base + Offsets::Name + i * 592, PlayerName, 31);
			std::string Name = PlayerName;
			TargetNames.push_back(Name);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		OldAnims.clear();
		for (int i = 0; i < 30; i++)
		{
			float AnimState = m->ReadMem<float>(m->eDll.base + Offsets::AnimState + i * 592);
			OldAnims.push_back(AnimState);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (MISC::FpsUnlock)
		{ 
			m->WriteMem<float>(m->eDll.base + Offsets::dwMaxFps, 999.f);
			m->WriteMem<float>(m->eDll.base + Offsets::dwMaxFps + 104, 999.f);
		}
	}
}

int main(int, char**)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = WINNAME;
	wc.lpszMenuName = WINNAME;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&wc);
	g_hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT , WINNAME, WINNAME, WS_POPUP, 0, 0, Width, Height, 0, 0, 0, 0);
	
	SetLayeredWindowAttributes(g_hwnd, 0, 255, LWA_ALPHA);
	SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	DwmExtendFrameIntoClientArea(g_hwnd, &MARGIN);

	if (CreateDeviceD3D(g_hwnd) < 0)
	{
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	ShowWindow(g_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(g_hwnd);
	SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFont* youFontM = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	ImFont* fontEsp = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeuib.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	
	ImGui_ImplWin32_Init(g_hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	if (!FindWindow("SDL_app", NULL)) {MessageBox(NULL, "START CS 1.6 BEFORE STARTING THE CHEAT", NULL, NULL); ExitProcess(EXIT_SUCCESS);}
	InitCheat();
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Updater, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Hack, 0, 0, 0);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		if (!open) ExitProcess(0);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::SetNextWindowPosCenter();

			if (ShowMenu)
			{
				ImGui::Begin("INTERIUM", &open, ImVec2(600, 480), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, 110.f);

				if (ImGui::Button(("AIMBOT"), ImVec2(100, 25))) Page = 0;
				if (ImGui::Button(("ESP"), ImVec2(100, 25))) Page = 1;
				if (ImGui::Button(("MISC"), ImVec2(100, 25))) Page = 2;
				if (ImGui::Button(("KEYS"), ImVec2(100, 25))) Page = 4;
				if (ImGui::Button(("CFG"), ImVec2(100, 25))) Page = 3;
				ImGui::TextWrapped("This cheat was made by KleskBY in 2019");
				ImGui::Dummy(ImVec2(0, 10));
				ImGui::TextWrapped(u8"INTERIUM LLC © 2020");

				ImGui::PopStyleVar(3);
				ImGui::NextColumn();
				
				if (Page == 0)
				{
					ImGui::Spacing(0, 5);
					ImGui::Checkbox(("Aimbot"), &Aimbot::Enabled); ImGui::SameLine(); ImGui::Checkbox(("Deathmatch"), &Aimbot::Deathmatch);

					ImGui::Combo(("Group"), &SelectedGroup, ("Pistols\0SMG\0Rifles\0Heavy\0"));
					if (SelectedGroup == 0) ImGui::Combo(("Weapon"), &SelectedWeapon, ("Glock\0USP\0P250\0Desert Eagle\0Dual berettas\0Five Seven\0"));
					else  if (SelectedGroup == 1) ImGui::Combo(("Weapon"), &SelectedWeapon, ("MAC-10\0TMP\0MP-5\0UMP\0P-90\0"));
					else if (SelectedGroup == 2) ImGui::Combo(("Weapon"), &SelectedWeapon, ("Galil\0Famas\0AK-47\0M4A1\0Scout\0SG\0AUG\0AWP\0AUTO T\0AUTO CT\0"));
					else if (SelectedGroup == 3) ImGui::Combo(("Weapon"), &SelectedWeapon, ("NOVA\0XM-ATUO\0M249\0"));
					
					if (SelectedGroup == 0 && SelectedWeapon == 0) SelectedWeaponID = 17;
					else if (SelectedGroup == 0 && SelectedWeapon == 1) SelectedWeaponID = 16;
					else if (SelectedGroup == 0 && SelectedWeapon == 2) SelectedWeaponID = 1;
					else if (SelectedGroup == 0 && SelectedWeapon == 3) SelectedWeaponID = 26;
					else if (SelectedGroup == 0 && SelectedWeapon == 4) SelectedWeaponID = 10;
					else if (SelectedGroup == 0 && SelectedWeapon == 5) SelectedWeaponID = 11;

					else if (SelectedGroup == 1 && SelectedWeapon == 0) SelectedWeaponID = 7;
					else if (SelectedGroup == 1 && SelectedWeapon == 1) SelectedWeaponID = 23;
					else if (SelectedGroup == 1 && SelectedWeapon == 2) SelectedWeaponID = 19;
					else if (SelectedGroup == 1 && SelectedWeapon == 3) SelectedWeaponID = 12;
					else if (SelectedGroup == 1 && SelectedWeapon == 4) SelectedWeaponID = 30;

					else if (SelectedGroup == 2 && SelectedWeapon == 0) SelectedWeaponID = 14;
					else if (SelectedGroup == 2 && SelectedWeapon == 1) SelectedWeaponID = 15;
					else if (SelectedGroup == 2 && SelectedWeapon == 2) SelectedWeaponID = 28;
					else if (SelectedGroup == 2 && SelectedWeapon == 3) SelectedWeaponID = 22;
					else if (SelectedGroup == 2 && SelectedWeapon == 4) SelectedWeaponID = 3;
					else if (SelectedGroup == 2 && SelectedWeapon == 5) SelectedWeaponID = 27;
					else if (SelectedGroup == 2 && SelectedWeapon == 6) SelectedWeaponID = 8;
					else if (SelectedGroup == 2 && SelectedWeapon == 7) SelectedWeaponID = 18;
					else if (SelectedGroup == 2 && SelectedWeapon == 8) SelectedWeaponID = 24;
					else if (SelectedGroup == 2 && SelectedWeapon == 9) SelectedWeaponID = 13;

					else if (SelectedGroup == 3 && SelectedWeapon == 0) SelectedWeaponID = 21;
					else if (SelectedGroup == 3 && SelectedWeapon == 1) SelectedWeaponID = 5;
					else if (SelectedGroup == 3 && SelectedWeapon == 2) SelectedWeaponID = 20;

					ImGui::SliderFloat(("FOV"), &Weapons[SelectedWeaponID].FOV, 0.f, 12.f);
					ImGui::SliderFloat(("Smooth"), &Weapons[SelectedWeaponID].Smootch, 1.f, 12.f);
					ImGui::SliderFloat(("RCS"), &Weapons[SelectedWeaponID].RCS, 0.f, 12.f);

					ImGui::Checkbox(("Square FOV"), &Aimbot::SquareFov);

					ImGui::Checkbox(("Draw FOV"), &Aimbot::DrawFov);
					ImGui::ColorEdit4(("FOV color"), Aimbot::DrawFovColor, ImGuiColorEditFlags_NoInputs);
					ImGui::SliderFloat(("FOV thickness"), &Aimbot::DrawFovWidth, 0.f, 10.f);
				}
				else if (Page == 1)
				{
					ImGui::Spacing(0, 5);
					ImGui::Checkbox(("Box"), &ESP::Box); 
					ImGui::SameLine(); ImGui::Dummy(ImVec2(20, 0)); ImGui::SameLine();
					ImGui::PushItemWidth(120.f);
					ImGui::Combo(("Type"), &ESP::BoxType, ("Classic\0Corner\0"));
					ImGui::PopItemWidth();
					ImGui::ColorEdit4(("Box color"), ESP::BoxColor, ImGuiColorEditFlags_NoInputs);
					ImGui::SliderFloat(("Box thickness"), &ESP::BoxWidth, 1.f, 10.f);
					if(ESP::BoxType == 0) ImGui::SliderFloat(("Box rounding"), &ESP::BoxRounding, 0.f, 10.f);

					ImGui::Checkbox(("Draw distance"), &ESP::Dist);
					ImGui::ColorEdit4(("Dist color"), ESP::DistColor, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox(("Draw names"), &ESP::Dist);
					ImGui::ColorEdit4(("Names color"), ESP::NamesColor, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox(("Crosshair"), &ESP::Crosshair);
					ImGui::ColorEdit4(("Crosshair color"), ESP::CrosshairColor, ImGuiColorEditFlags_NoInputs);
					ImGui::SliderFloat(("Crosshair thickness"), &ESP::CrosshairWidth, 0.f, 10.f);
					ImGui::SliderFloat(("Crosshair size"), &ESP::CrosshairSize, 0.f, 10.f);
				}
				else if (Page == 2)
				{
					ImGui::Spacing(0, 5);
					ImGui::Checkbox(("BunnyHop"), &MISC::Bhop);
					ImGui::Checkbox(("DDRun"), &MISC::DDrun);
					ImGui::Checkbox(("AutoPistol"), &MISC::AutoPistol);
					ImGui::Checkbox(("FPS Unlock"), &MISC::FpsUnlock);
				}
				else if (Page == 3)
				{
					ImGui::Spacing(0, 5);

					ImGui::BeginChild(("##config"), ImVec2(457,300));
					RefreshSettings();
					for (int i = 0; i < SettingsList.size(); i++)
					{
						std::string configname = SettingsList[i];
						configname = configname.substr(0, configname.size() - 4);
						std::string pathtocfg = configname.substr(0, configname.find_last_of("\\/"));
						configname = configname.erase(0, pathtocfg.size() + 1);

						if (ImGui::Selectable(configname.c_str(), &IsConfigSelected[i]))
						{
							for (int j = 0; j < 50; j++)
							{
								IsConfigSelected[j] = false;
							}
							IsConfigSelected[i] = true;
						}
					}
					ImGui::EndChild();

					ImGui::Spacing(0, 5);
					if (ImGui::Button(("SAVE"), ImVec2(225, 25)))
					{
						for (int j = 0; j < 50; j++)
						{
							if (IsConfigSelected[j])
							{
								SaveConfig(SettingsList[j]);
							}
						}
						Beep(300, 100);
						Sleep(30);
						Beep(300, 100);
					}

					ImGui::PushItemWidth(225);
					ImGui::SameLine();  ImGui::InputText(("##cfgname"), cfgname, sizeof(cfgname));
					ImGui::PopItemWidth();

					if (ImGui::Button(("LOAD"), ImVec2(225, 25)))
					{
						for (int j = 0; j < 50; j++)
						{
							if (IsConfigSelected[j])
							{
								LoadConfig(SettingsList[j]);

								if (Keys[KEYS::MenuKey] != NULL) MenuKeyLabel = Keys[KEYS::MenuKey];
								else  MenuKeyLabel = ("unknown");
								if (Keys[KEYS::AimbotKey1] != NULL) AimbotKeyLabel = Keys[KEYS::AimbotKey1];
								else  AimbotKeyLabel = ("unknown");
								if (Keys[KEYS::AimbotKey2] != NULL) Aimbot2KeyLabel = Keys[KEYS::AimbotKey2];
								else  Aimbot2KeyLabel = ("unknown");
								if (Keys[KEYS::BhopKey] != NULL) BhopKeyLabel = Keys[KEYS::BhopKey];
								else  BhopKeyLabel = ("unknown");
								if (Keys[KEYS::DDrunKey] != NULL) DDrunKeyLabel = Keys[KEYS::DDrunKey];
								else  DDrunKeyLabel = ("unknown");
							}
						}
						Beep(300, 100);
						Sleep(30);
						Beep(300, 100);
					}

					ImGui::SameLine();
					if (ImGui::Button(("CREATE AND SAVE"), ImVec2(225, 25)))
					{
						if (strlen(cfgname) > 0)
						{
							std::string configDir = std::string(getenv("appdata")) + std::string(("\\INTERIUM\\CS16Ext\\"));
							SaveConfig(configDir + cfgname + ".ini");
						}
					}

					if (ImGui::Button(("DELETE"), ImVec2(225, 25)))
					{
						for (int j = 0; j < 50; j++)
						{
							if (IsConfigSelected[j])
							{
								if (remove(SettingsList[j].c_str()))
								{
									Beep(300, 100);
									Sleep(30);
									Beep(300, 100);
								}
							}
						}
					}

					ImGui::SameLine();
					if (ImGui::Button(("OPEN FOLDER"), ImVec2(225, 25)))
					{
						std::string agagag = std::string(getenv("appdata")) + std::string(("\\INTERIUM\\CS16Ext\\"));
						ShellExecute(NULL, "open", agagag.c_str(), NULL, NULL, SW_SHOWNORMAL);
					}

				}
				else if (Page == 4)
				{
					ImGui::Spacing(0, 5);
					if (ImGui::Button((MenuKeyLabel + "##MenuKeyLabel").c_str(), ImVec2(200, 25))) SetMenuKey = true;
					while (SetMenuKey)
					{
						for (int i = 0; i < 256; i++)
						{
							if (GetAsyncKeyState(i) & 0x8000)
							{
								KEYS::MenuKey = i;
								MenuKeyLabel = Keys[i];
								SetMenuKey = false;
								std::this_thread::sleep_for(std::chrono::milliseconds(300));
							}
						}
					}
					ImGui::SameLine();  ImGui::Text("Menu key");

					ImGui::Spacing(0, 5);
					if (ImGui::Button((AimbotKeyLabel + "##AimbotKeyLabel").c_str(), ImVec2(200, 25))) SetAimbotKey = true;
					while (SetAimbotKey)
					{
						for (int i = 0; i < 256; i++)
						{
							if (GetAsyncKeyState(i) & 0x8000)
							{
								KEYS::AimbotKey1 = i;
								AimbotKeyLabel = Keys[i];
								SetAimbotKey = false;
							}
						}
					}
					ImGui::SameLine();  ImGui::Text("Aimbot key");

					ImGui::Spacing(0, 5);
					if (ImGui::Button((Aimbot2KeyLabel + "##Aimbot2KeyLabel").c_str(), ImVec2(200, 25))) SetAimbot2Key = true;
					while (SetAimbot2Key)
					{
						for (int i = 0; i < 256; i++)
						{
							if (GetAsyncKeyState(i) & 0x8000)
							{
								KEYS::AimbotKey2 = i;
								Aimbot2KeyLabel = Keys[i];
								SetAimbot2Key = false;
							}
						}
					}
					ImGui::SameLine();  ImGui::Text("Aimbot2 key");

					ImGui::Spacing(0, 5);
					if (ImGui::Button((BhopKeyLabel + "##BhopKeyLabel").c_str(), ImVec2(200, 25))) SetBhopKey = true;
					while (SetBhopKey)
					{
						for (int i = 0; i < 256; i++)
						{
							if (GetAsyncKeyState(i) & 0x8000)
							{
								KEYS::BhopKey = i;
								BhopKeyLabel = Keys[i];
								SetBhopKey = false;
							}
						}
					}
					ImGui::SameLine();  ImGui::Text("Bhop key");

					ImGui::Spacing(0, 5);
					if (ImGui::Button((DDrunKeyLabel + "##DDrunKeyLabel").c_str(), ImVec2(200, 25))) SetDDrunKey = true;
					while (SetDDrunKey)
					{
						for (int i = 0; i < 256; i++)
						{
							if (GetAsyncKeyState(i) & 0x8000)
							{
								KEYS::DDrunKey = i;
								DDrunKeyLabel = Keys[i];
								SetDDrunKey = false;
							}
						}
					}
					ImGui::SameLine();  ImGui::Text("DDrun key");
				}
				ImGui::Columns();
				ImGui::End();
			}
			else if(GetForegroundWindow() == FindWindow("SDL_app", NULL) && !InMenu)
			{
				ImGuiIO& io = ImGui::GetIO();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
				ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
				ImGui::Begin("##ESP", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

				ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Once);
				ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Once);

				ImGui::Text("  INTERIUM.OOO");
				ImGui::Text(std::to_string(WeaponID).c_str());

				int BestTarget = -1;
				double ClosestPos = 9999999;
				float ScreenCenterX = Width / 2;
				float ScreenCenterY = Height / 2;
				float radiusx = Aimbot::FOV * (ScreenCenterX / 100);
				float radiusy = Aimbot::FOV * (ScreenCenterY / 100);
				if (Aimbot::SquareFov) radiusy = radiusx;

				if (ESP::Crosshair)
				{
					RenderLine(ImVec2(Width / 2 - ESP::CrosshairSize, Height / 2), ImVec2(Width / 2 + ESP::CrosshairSize, Height / 2), ImVec4(ESP::CrosshairColor[0], ESP::CrosshairColor[1], ESP::CrosshairColor[2], ESP::CrosshairColor[3]), ESP::CrosshairWidth);
					RenderLine(ImVec2(Width / 2, Height / 2 - ESP::CrosshairSize), ImVec2(Width / 2, Height / 2 + ESP::CrosshairSize), ImVec4(ESP::CrosshairColor[0], ESP::CrosshairColor[1], ESP::CrosshairColor[2], ESP::CrosshairColor[3]), ESP::CrosshairWidth);
				}
				if (Aimbot::DrawFov) RenderRect(ImVec2(ScreenCenterX - radiusx, ScreenCenterY - radiusy), ImVec2(ScreenCenterX + radiusx, ScreenCenterY + radiusy), ImVec4(Aimbot::DrawFovColor[0], Aimbot::DrawFovColor[1], Aimbot::DrawFovColor[2], Aimbot::DrawFovColor[3]), 2.f, ImDrawCornerFlags_All, Aimbot::DrawFovWidth);
				for (int i = 0; i < Targets.size(); i++)
				{
					if (Anims.size() < Targets.size()) Anims.push_back(0.f);
					if (OldAnims.size() < Targets.size()) OldAnims.push_back(0.f);
				}

				TargetsWS.clear();
				for (int i = 0; i < Targets.size(); i++)
				{

					if (Anims[i] == OldAnims[i] && Anims[i] == OldOldAnims[i]) TargetsWS.push_back(Vector3(0, 0, 0));
					else if(Targets[i].x != 0)
					{
						Vector3 Draw = W2S(Vector3(Targets[i].x, Targets[i].y, Targets[i].z));
						TargetsWS.push_back(Vector3(Draw.x, Draw.y + abs(recoil) * Aimbot::RCS, 0));
						if (Draw.x < 4) continue;
						if (!Aimbot::Deathmatch)
						{
							int HisTeam = 2;
							std::string modelname = TargetModels[i];
							if (modelname == "arc" || modelname == "gue" || modelname == "lee" || modelname == "ter")
							{
								HisTeam = 1;
							}
							if (HisTeam == PlayerTeam) continue;
						}
						//if (drawdot) RenderRectFilled(ImVec2(Draw.x - 2, Draw.y - 2), ImVec2(Draw.x + 2, Draw.y + 2), ImVec4(1.f, 0.f, 0.f, 1.f), 0, 0);// FillRGB(Draw.x - 2, Draw.y - 2, 4, 4, 255, 0, 0, 155);

						Vector3 Draw2 = W2S(Vector3(Targets[i].x, Targets[i].y, Targets[i].z - 50.f));
						int boxheight = (Draw2.y - Draw.y) * 1.25;
						if (ESP::Box)
						{
							if(ESP::BoxType == 0)
							RenderRect(ImVec2(Draw2.x - boxheight / 4, Draw.y - boxheight * 0.25), ImVec2(Draw2.x + boxheight / 3.2, Draw.y + boxheight * 0.85), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxRounding, ImDrawCornerFlags_All, ESP::BoxWidth);// boxheight / 2, boxheight, 1, 255, 0, 0, 255);
							else
							{
								float boxwidth = (Draw2.x - boxheight / 4) - (Draw2.x + boxheight / 3.2);
								float lineW = (boxwidth / 5);
								float lineH = (boxheight / 6);

								RenderLine(ImVec2(Draw2.x - boxheight / 4, Draw.y - boxheight * 0.25), ImVec2(Draw2.x - boxheight / 4 - lineW, Draw.y - boxheight * 0.25), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);
								RenderLine(ImVec2(Draw2.x - boxheight / 4, Draw.y - boxheight * 0.25), ImVec2(Draw2.x - boxheight / 4, Draw.y - boxheight * 0.25 + lineH), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);

								RenderLine(ImVec2(Draw2.x - boxheight / 4, Draw.y - boxheight * 0.25 + boxheight - lineH), ImVec2(Draw2.x - boxheight / 4, Draw.y - boxheight * 0.25 + boxheight), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);
								RenderLine(ImVec2(Draw2.x - boxheight / 4, Draw.y - boxheight * 0.25 + boxheight), ImVec2(Draw2.x - boxheight / 4-lineW, Draw.y - boxheight * 0.25 + boxheight), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);

								RenderLine(ImVec2(Draw2.x - boxheight / 4 - boxwidth + lineW, Draw.y - boxheight * 0.25), ImVec2(Draw2.x - boxheight / 4 - boxwidth, Draw.y - boxheight * 0.25), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);
								RenderLine(ImVec2(Draw2.x - boxheight / 4 - boxwidth, Draw.y - boxheight * 0.25), ImVec2(Draw2.x - boxheight / 4 - boxwidth, Draw.y - boxheight * 0.25 + lineH), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);

								RenderLine(ImVec2(Draw2.x - boxheight / 4 - boxwidth, Draw.y - boxheight * 0.25 + boxheight - lineH), ImVec2(Draw2.x - boxheight / 4 - boxwidth, Draw.y - boxheight * 0.25 + boxheight), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);
								RenderLine(ImVec2(Draw2.x - boxheight / 4 - boxwidth, Draw.y - boxheight * 0.25 + boxheight), ImVec2(Draw2.x - boxheight / 4 - boxwidth + lineW, Draw.y - boxheight * 0.25 + boxheight), ImVec4(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], ESP::BoxColor[3]), ESP::BoxWidth);
							}
						}
						float dist = Targets[i].DistTo(view.vOrigin) * 0.254;
						if (ESP::Dist)
						{
							Vector3 Draw3 = W2S(Vector3(Targets[i].x, Targets[i].y, Targets[i].z - 55.f));
							RenderText(std::to_string((int)dist), ImVec2(Draw3.x, Draw3.y), 16.f, ImVec4(ESP::DistColor[0], ESP::DistColor[1], ESP::DistColor[2], ESP::DistColor[3]), true, fontEsp);
						}
						if (ESP::Names)
						{
							Vector3 Draw4 = W2S(Vector3(Targets[i].x, Targets[i].y, Targets[i].z + 25.f));
							RenderText(TargetNames[i], ImVec2(Draw4.x, Draw4.y - 8), 16.f, ImVec4(ESP::NamesColor[0], ESP::NamesColor[1], ESP::NamesColor[2], ESP::NamesColor[3]), true, fontEsp);
						}

						if (Aimbot::Enabled && !InMenu)
						{
							float CrosshairDistance = Draw.DistTo(Vector3(ScreenCenterX, ScreenCenterY, 0));
							if (CrosshairDistance > 200) continue;

							if (Draw.x >= ScreenCenterX - radiusx && Draw.x <= ScreenCenterX + radiusx && Draw.y >= ScreenCenterY - radiusy && Draw.y <= ScreenCenterY + radiusy)
							{
								if (CrosshairDistance < ClosestPos) 
								{
									ClosestPos = CrosshairDistance;
									BestTarget = i;
								}
							}
						}
					}
				}
				if (Aimbot::Enabled)
				{
					if (BestTarget != -1)
					{
						double DistX = (double)TargetsWS[BestTarget].x - ScreenCenterX;
						double DistY = (double)TargetsWS[BestTarget].y - ScreenCenterY;

						DistX /= Aimbot::Smooth;
						DistY /= Aimbot::Smooth;

						if (GetAsyncKeyState(KEYS::AimbotKey1) & 0x8000 || GetAsyncKeyState(KEYS::AimbotKey2) & 0x8000) mouse_event(MOUSEEVENTF_MOVE, (int)DistX, (int)DistY, NULL, NULL);
					}
				}

				ImDrawList* Draw = ImGui::GetWindowDrawList();
				Draw->PushClipRectFullScreen();

				ImGui::End();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar(2);
			}

			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ResetDevice();
			InitCheat();
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(g_hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}
