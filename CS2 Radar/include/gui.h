#include "./ImGUI/imgui.h"
#include "./ImGUI/imgui_impl_win32.h"
#include "./ImGUI/imgui_impl_dx11.h"
#include "./WICTextureLoader11.h"

#include <d3d11.h>
#include <unordered_map>
#include <iostream>
#include <thread>

#pragma comment(lib, "d3d11.lib")

// Forward declare message handler from imgui_impl_win32.cpp - given in imgui example
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gui {
	// Data - given in imgui example
	extern ID3D11Device* g_pd3dDevice;
	extern ID3D11DeviceContext* g_pd3dDeviceContext;
	extern IDXGISwapChain* g_pSwapChain;
	extern UINT g_ResizeWidth, g_ResizeHeight;
	extern ID3D11RenderTargetView* g_mainRenderTargetView;
	extern bool exitRequested;
	extern WNDCLASSEXW wc;
	extern HWND hwnd;

	//Helper Function Declarations - given in imgui example
	bool CreateDeviceD3D(HWND);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

    //Image loading functions
	ID3D11ShaderResourceView* LoadImageTexture(ID3D11Device* device, const wchar_t* filename);
	ID3D11ShaderResourceView* LoadImageTexture(ID3D11Device* device, const wchar_t* filename, int* outWidth, int* outHeight);

	//Rendering Functions
	void initializeGUI();
	void Render();

	//Game related functions
	//extern CGame* game;
	void gameLoop();
	void renderMap(ID3D11ShaderResourceView*);
	void renderPlayers();
		
	//utility
	//Convert the world position we read in to the 2d position on the map
	void worldToRadar(float& x, float& y);
	//The map bounds are the values we use to determine 2d position in worldToRadar
	void loadMapBounds();
	void loadTextures();
	ID3D11ShaderResourceView* fetchMap(std::string);
}

//This class holds the values we use to find our 2d map postion from our world postion
class mapData {
public:
	float xBound;
	float yBound;
	float scale;

	mapData();
	mapData(float x, float y, float s);
};

//This holds an unordered map of our textures and mapData
namespace maps {
	extern float radarSize;
	//values for knowing if player is 1st or 2nd floor on multi level maps
	extern float vertigoZBound;
	extern float nukeZBound;
	extern std::unordered_map<std::string, ID3D11ShaderResourceView*> mapTextures;
	extern std::unordered_map<std::string, mapData> mapBounds;
}
//Similar to the maps, this holds our weapon/inventory icons
namespace icons {
	extern float scale;
	extern std::unordered_map<int, ID3D11ShaderResourceView*> iconTextures;
	extern std::unordered_map<int, int> iconWidths;
	extern std::unordered_map<int, int> iconHeights;
}

