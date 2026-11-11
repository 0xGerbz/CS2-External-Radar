#include "./ImGUI/imgui.h"
#include "./ImGUI/imgui_impl_win32.h"
#include "./ImGUI/imgui_impl_dx11.h"
#include "./WICTextureLoader11.h"
#include "game.h"

#include <d3d11.h>
#include <unordered_map>
#include <iostream>
#include <thread>

#pragma comment(lib, "d3d11.lib")

// Forward declare message handler from imgui_impl_win32.cpp - given in imgui example
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//The offsets and scale are values we can find in game to normalize our world position. We subtract these values and divide them by the scale to convert our position.
struct map {
	float xOffset;
	float yOffset;
	float scale;

	ID3D11ShaderResourceView* texture;

	//constructors
	map() : xOffset(0.0f), yOffset(0.0f), scale(0.0f), texture(nullptr) {}
	map(float x, float y, float s, ID3D11ShaderResourceView* t);
};

struct icon {
	ID3D11ShaderResourceView* texture;

	//constructors
	icon() : texture(nullptr) {}
	icon(ID3D11ShaderResourceView* t);
};

struct GUI {
	// Data - given in imgui example
	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pd3dDeviceContext;
	IDXGISwapChain* g_pSwapChain;
	static UINT g_ResizeWidth;
	static UINT	g_ResizeHeight;
	ID3D11RenderTargetView* g_mainRenderTargetView;
	bool exitRequested;
	WNDCLASSEXW wc;
	HWND hwnd;

	//unordered map to hold our map objects - the key for our map is the string name, and the key for our icons is its' ID
	std::unordered_map<std::string, map> maps;
	std::unordered_map<int, icon> icons;
	//Constructor
	GUI();

	//Helper Function Declarations - given in imgui example
	bool CreateDeviceD3D(HWND);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

	//Image loading function
	ID3D11ShaderResourceView* LoadImageTexture(ID3D11Device* device, const wchar_t* filename);

	//Rendering Functions
	void initializeGUI();
	void Render();

	//load our map/icon images
	void loadMaps();
	void loadIcons();

	ID3D11ShaderResourceView* fetchMap(std::string);
};

