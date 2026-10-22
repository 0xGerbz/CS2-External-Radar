#include "gui.h"

namespace gui {
	ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
	bool exitRequested = false;
	WNDCLASSEXW wc = {};
	HWND hwnd = NULL;

}

namespace maps {
	//You can change radar size based on your resolution. Set at 1080p right now for 1920x1080
	float radarSize = 1080;
	float vertigoZBound = 11700;
	float nukeZBound = -495;
	std::unordered_map<std::string, ID3D11ShaderResourceView*> mapTextures;
	std::unordered_map<std::string, mapData> mapBounds;
}

namespace icons {
	int id;
	float scale = 0.4f;
	std::unordered_map<int, ID3D11ShaderResourceView*> iconTextures;
	std::unordered_map<int, int> iconWidths;
	std::unordered_map<int, int> iconHeights;
}

mapData::mapData() {
	xBound = 0.0f;
	yBound = 0.0f;
	scale = 0.0f;
}

mapData::mapData(float x, float y, float s) {
	xBound = x;
	yBound = y;
	scale = s;
}



//All of our rendering
void gui::Render() {
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.f);
	//game = new CGame;
	loadTextures();
	loadMapBounds();
	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT || exitRequested) {
				done = true;
			}

		}
		if (done)
			break;

		// Handle window resize (we don't resize directly in the WM_SIZE handler)
		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
			g_ResizeWidth = g_ResizeHeight = 0;
			CreateRenderTarget();
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		//Where the magic happens

		//Read in all info
        // if(in game){
        //     gameloop
        // }

		//Simple exit button example
		//Define our size and margin(how far from the edge of the screen we want it)
		const ImVec2 btnSize(100, 30);
		const float margin = 10.0f;
		ImVec2 display = ImGui::GetIO().DisplaySize;
		//Set the window position x grows from left to right and y grows from TOP TO BOTTOM - kind of weird
		ImGui::SetNextWindowPos(ImVec2(display.x - btnSize.x - margin, margin));
		//Style options we can use. here we are turning of any border/padding on the button window
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		//Every imgui window/element will begin with a ImGui::Begin() call and end with ImGui::End()
		ImGui::Begin("ExitButton", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
		//we have to call popstyle var to reset to default after we changed the style variable in lines 101-102
		ImGui::PopStyleVar();
		//Set colors for the button
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.10f, 0.10f, 1.0f)); // normal
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.0f)); // hovered
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.20f, 0.20f, 1.0f)); // held down
		//Make our button, will say exit
		if (ImGui::Button("Exit", btnSize)) {
			done = true;
		}
		//Pop colors
		ImGui::PopStyleColor(3);
		//End our window
		ImGui::End();





		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}


	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	return;
}




//Loading images
ID3D11ShaderResourceView* gui::LoadImageTexture(ID3D11Device* device, const wchar_t* filename)
{
	ID3D11ShaderResourceView* texture = nullptr;

	HRESULT hr = DirectX::CreateWICTextureFromFile(
		device,
		filename,
		nullptr,
		&texture
	);

	if (FAILED(hr))
	{
		// Failed to load - return null
		return nullptr;
	}

	return texture;
}
ID3D11ShaderResourceView* gui::LoadImageTexture(ID3D11Device* device, const wchar_t* filename, int* outWidth, int* outHeight)
{
	ID3D11ShaderResourceView* texture = nullptr;
	ID3D11Resource* resource = nullptr;

	HRESULT hr = DirectX::CreateWICTextureFromFile(
		device,
		filename,
		&resource,
		&texture
	);

	if (FAILED(hr))
		return nullptr;

	if (resource && outWidth && outHeight)
	{
		ID3D11Texture2D* tex2D = nullptr;
		if (SUCCEEDED(resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tex2D)))
		{
			D3D11_TEXTURE2D_DESC desc;
			tex2D->GetDesc(&desc);
			*outWidth = (int)desc.Width;
			*outHeight = (int)desc.Height;
			tex2D->Release();
		}
		resource->Release();
	}

	return texture;
}


//Helpers - All of the following functions are given in imgui example
//Init
void gui::initializeGUI() {
	// Create application window
	wc = { sizeof(wc), CS_VREDRAW | CS_HREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"CS2 Radar", nullptr };
	::RegisterClassExW(&wc);
	gui::hwnd = ::CreateWindowExW(WS_EX_APPWINDOW, wc.lpszClassName, L"CS2 Radar", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, wc.hInstance, nullptr);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return;
	}
	// Show the window
	::ShowWindow(hwnd, SW_MAXIMIZE);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

}
bool gui::CreateDeviceD3D(HWND hWnd){
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}
void gui::CleanupDeviceD3D(){
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}
void gui::CreateRenderTarget(){
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}
void gui::CleanupRenderTarget(){
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
//Handles resizing, minimizing, exiting window
LRESULT WINAPI gui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_GETMINMAXINFO:
		MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		minMaxInfo->ptMinTrackSize.x = 960;  // Set minimum width
		minMaxInfo->ptMinTrackSize.y = 540;  // Set minimum height
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);

}