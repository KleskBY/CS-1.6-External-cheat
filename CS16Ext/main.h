#pragma once
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "ws2_32.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include <chrono>
#include <thread>
#include <dwmapi.h>
#define DIRECTINPUT_VERSION 0x0800

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static HWND g_hwnd = NULL;
static char WINNAME[16] = " ";

static HWND GameHWND = NULL;
static RECT tSize;

static int Width = 640;
static int Height = 480;
static bool open = true;
static bool ShowMenu = true;

static MARGINS MARGIN = { 0, 0, Width, Height };

using namespace std;

static HRESULT CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) return E_FAIL;

	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	g_d3dpp.BackBufferHeight = Height;
	g_d3dpp.BackBufferWidth = Width;
	g_d3dpp.MultiSampleQuality = DEFAULT_QUALITY;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present without vsync, maximum unthrottled framerate 
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0) return E_FAIL;

	return S_OK;
}

static void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

static void ResetDevice()
{
	g_d3dpp.BackBufferHeight = Height;
	g_d3dpp.BackBufferWidth = Width;
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}


static const char* Keys[] =
{
	"none", "MOUSE 1", "MOUSE 2", "CANCEL", "MOUSE 3", "MOUSE 4", "MOUSE 5", "None", "BACKSPACE",
	"TAB", "None", "None", "CLEAR", "ENTER", "None",  "None",  "SHIFT",  "CTRL",  "ALT",  "PAUSE",
	 "CAPS LOCK",  "KANA",  "HANGUEL",  "HANGUL",  "JUNIA",  "FINAL",  "HANJA",
		"ESC",  "CONVERT",  "NONCONVERT",  "ACCEPT",  "MODE",  "SPACE",  "PAGE UP",  "PAGE DOWN",
	   "END", "HOME", "LEFT ARROW", "UP ARROW", "RIGHT ARROW", "DOWN ARROW", "SELECT", "PRINT", "EXECUTE",
	   "PRINT SCREEN", "INSERT", "DELETE", "HELP", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	   "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "A", "B", "C", "D", "E", "F", "G", "H",
	   "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	   "WIN", "WIN", "APPS", "NONE", "SLEEP", "NUMPAD 0", "NUMPAD 1", "NUMPAD 2", "NUMPAD 3", "NUMPAD 4",
	   "NUMPAD 5", "NUMPAD 6", "NUMPAD 7", "NUMPAD 8", "NUMPAD 9", "MULTIPLY", "ADD", "SEPARATOR", "SUBTRACT",
	   "DECIMIAL", "DIVIDE", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	   "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
	   "NUM LOCK", "SCROLL LOCK", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
	   "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
	   "LSHIFT", "RSHIFT", "LCTRL", "RCTRL", "LALT", "RALT", "BACK", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE"
};
