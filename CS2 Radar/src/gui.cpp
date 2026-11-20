#include "gui.h"

//Constructors
GUI::GUI() {
	g_pd3dDevice = NULL;
	g_pd3dDeviceContext = NULL;
	g_pSwapChain = NULL;
	g_mainRenderTargetView = NULL;
	exitRequested = false;
	wc = {};
	hwnd = NULL;
}

map::map(float x, float y, float s, ID3D11ShaderResourceView* t) {
	xOffset = x;
	yOffset = y;
	scale = s;
	texture = t;
}

icon::icon(ID3D11ShaderResourceView* t) {
	texture = t;
}

// Variables
static bool show_window = true;
static bool enable_feature = false;
static float background_opacity = 1.0f;
static float radar_size = 1000.0f;
static float radar_x = 540.0f;
static float radar_y = 960.0f;
static bool show_player_names = true;
static bool show_health = true;
static bool show_aim_lines = true;
static bool show_weapons = true;

//We have to declare this globaly for our WndProc function
UINT GUI::g_ResizeWidth = 0;
UINT GUI::g_ResizeHeight = 0;

//All of our rendering
void GUI::Render() {
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.f);
	loadMaps();
	loadIcons();
	// Main loop - the structure of the main loop is given in the imgui example
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
		renderGame();
		exitButton(done);

		// Window with changes to the radar
		ImGui::SetNextWindowBgAlpha(background_opacity);
		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver); // Where the radar window will start.
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver); // Radar window a starting size.

		if (show_window) {
			if (ImGui::Begin("CS2Radar", &show_window)) { 	// Create a window called "CS2Radar" and append into it
				if (ImGui::BeginTabBar("TabBar")) { // Create a tab bar container
					if (ImGui::BeginTabItem("Radar")) { // Create a tab item called "Radar"
						ImGui::SliderFloat("Radar X", &radar_x, 0.0f, 1080.0f); // Slider for radar X
						ImGui::SliderFloat("Radar Y", &radar_y, 0.0f, 1920.0f); // Slider for radar Y
						ImGui::SliderFloat("Radar Size", &radar_size, 100.0f, 2000.0f); // Slider for radar size
						// Checkboxes for various radar features
						if (ImGui::Checkbox("Player Names", &show_player_names)) {
							
						}
						if (ImGui::Checkbox("Health", &show_health)) {
							
						}
						if (ImGui::Checkbox("Aim Lines", &show_aim_lines)) {
							
						}
						if (ImGui::Checkbox("Weapons", &show_weapons)) {
							
						}
						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Settings")) {
						ImGui::Text("Overlay Settings");
						ImGui::SliderFloat("Background Opacity", &background_opacity, 0.0f, 1.0f);
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			}
			ImGui::End(); // Every Begin() needs a matching End().
		}

		//Render - given in imgui example
		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// Cleanup - given in imgui example
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	return;
}

//Turn our 3D world position into our 2D map position
void GUI::worldToRadarPos(float& x, float& y, std::string mapName) {
	map currentMap = maps[mapName];
	//Subtract our offsets from our world position
	x -= currentMap.xOffset;
	y -= currentMap.yOffset;
	//Divide by the maps scale
	x /= currentMap.scale;
	y /= currentMap.scale;
	//This lets us resize the radar. By default it is 1024x1024 pixels
	x *= radar_size / 1024.f;
	y *= radar_size / 1024.f;
	y *= -1;
}

void GUI::renderGame() {
	Game g;
	g.update();
	std::string mapName = g.map;
	//We need to check our local players Z position to determine what floor we are on in multi floor maps
	//I found the exact Z position to change floors by testing in game and monotoring my world position value
	if (mapName == "de_vertigo") {
		if (g.localPlayer.position.z <= 11700) mapName = "de_vertigo_lower";
	}
	if (mapName == "de_nuke") {
		if (g.localPlayer.position.z <= -495) mapName = "de_nuke_lower";
	}
	//Map rendering
	int radarSize = int(radar_size);
	ImVec2 display = ImGui::GetIO().DisplaySize;
	map currentMap = maps[mapName];
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2((display.x / 2 - int(radar_x)), (display.y / 2 - int(radar_y) / 2)));
	ImGui::SetNextWindowSize(ImVec2(radarSize, radarSize));
	ImGui::Begin("MAP", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Image((void*)currentMap.texture, ImVec2(radarSize, radarSize));
	ImGui::PopStyleVar(2);

	//Player rendering will be a child of this window so we do it all here
	for (int i = 0; i < g.players.size(); i++) {
		Player currentPlayer = g.players[i];

		//Skip dead players
		if (currentPlayer.controller == NULL) continue;
		if (currentPlayer.health == 0) continue;

		//grab our position and convert to 2D with our world to radar function
		float x = currentPlayer.position.x;
		float y = currentPlayer.position.y;
		worldToRadarPos(x, y, g.map);
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImU32 dotColor;
		float opacity = 255;
	
		//Set our opacity - we only need this for multi-level maps (nuke/vertigo)
		//People on different levels than the local with have a lower opacity
		if (mapName == "de_nuke" || mapName == "de_nuke_lower") {
			if (g.localPlayer.position.z < -495 && currentPlayer.position.z >= -495) opacity = 155;
			if (g.localPlayer.position.z >= -495 && currentPlayer.position.z < -495) opacity = 155;
		}
		if (mapName == "de_vertigo" || mapName == "de_vertigo_lower") {
			if (g.localPlayer.position.z < 11700 && currentPlayer.position.z >= 11700) opacity = 155;
			if (g.localPlayer.position.z >= 11700 && currentPlayer.position.z < 11700) opacity = 155;
		}

		//Set our dot color - only for team, enemies will always be red
		if (currentPlayer.team == g.localPlayer.team) {
			//Grey
			if(currentPlayer.color == -1) dotColor = IM_COL32(128, 128, 128, opacity);
			//Blue
			if(currentPlayer.color == 0) dotColor = IM_COL32(60, 140, 255, opacity);
			//Green
			if(currentPlayer.color == 1) dotColor = IM_COL32(40, 200, 90, opacity);
			//yellow
			if(currentPlayer.color == 2) dotColor = IM_COL32(255, 255, 0, opacity);
			//Orange
			if(currentPlayer.color == 3) dotColor = IM_COL32(250, 130, 2, opacity);
			//Purple
			if(currentPlayer.color == 4) dotColor = IM_COL32(180, 80, 230, opacity);
			
			if(currentPlayer.controller == g.localPlayer.controller) dotColor = IM_COL32(255, 255, 255, opacity);
		}


		//aim lines. We render them before the player dot so they remain under the dot
		float angle = currentPlayer.viewAngle.y;
		//convert to radians
		angle = angle * 3.14159265f / 180.0f;
		float xDirection = cos(angle);
		//Have to flip our Y sign
		float yDirection = sin(angle) * -1;
		float length = 60;
		float xEndpoint = windowPos.x + x + xDirection * length;
		float yEndpoint = windowPos.y + y + yDirection * length;
		ImGui::GetForegroundDrawList()->AddLine(ImVec2((windowPos.x + x), (windowPos.y + y)), ImVec2(xEndpoint, yEndpoint), IM_COL32(0, 0, 0, opacity), 7.5f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2((windowPos.x + x), (windowPos.y + y)), ImVec2(xEndpoint, yEndpoint), IM_COL32(255, 255, 255, opacity), 4.0f);


		//We will be representing players as dots
		//Add a black outline to player circles. We do this first so we draw a smaller colored circle on top of it
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2((windowPos.x + x), (windowPos.y + y)), 13.0f, IM_COL32(0, 0, 0, 255));
		//Draw a red dot for enemies
		if (currentPlayer.team != g.localPlayer.team) {
			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2((windowPos.x + x), (windowPos.y + y)), 10.0f, IM_COL32(255, 0, 0, opacity));
		}
		else {
			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2((windowPos.x + x), (windowPos.y + y)), 10.0f, dotColor);
		}

		//Weapons will go here
	}

	ImGui::End();
}

void GUI::exitButton(bool& exit) {
	//Simple exit button example
	//Define our size and margin(how far from the edge of the screen we want it)
	const ImVec2 buttonSize(100, 30);
	const float margin = 10.0f;
	ImVec2 display = ImGui::GetIO().DisplaySize;
	//Set the window position x grows from left to right and y grows from TOP TO BOTTOM - kind of weird
	ImGui::SetNextWindowPos(ImVec2(display.x - buttonSize.x - margin, margin));
	//Style options we can use. here we are turning setting a border and turning off padding on the button window
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	//Every imgui window/element will begin with a ImGui::Begin() call and end with ImGui::End()
	ImGui::Begin("ExitButton", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
	//we have to call popstyle var to reset to default after we changed the style variable
	ImGui::PopStyleVar();
	//Set colors for the button
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.10f, 0.10f, 1.0f)); // normal
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.0f)); // hovered
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.20f, 0.20f, 1.0f)); // held down
	//Make our button, will say exit
	if (ImGui::Button("Exit", buttonSize)) {
		exit = true;
	}
	//Pop colors
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
	//End our window
	ImGui::End();
}

//Loading images
ID3D11ShaderResourceView* GUI::LoadImageTexture(ID3D11Device* device, const wchar_t* filename)
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

//Helpers - All of the following functions are given in imgui example
//Init
void GUI::initializeGUI() {
	// Create application window
	wc = { sizeof(wc), CS_VREDRAW | CS_HREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"CS2 Radar", nullptr };
	::RegisterClassExW(&wc);
	hwnd = ::CreateWindowExW(WS_EX_APPWINDOW, wc.lpszClassName, L"CS2 Radar", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, wc.hInstance, nullptr);

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
bool GUI::CreateDeviceD3D(HWND hWnd) {
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
void GUI::CleanupDeviceD3D() {
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}
void GUI::CreateRenderTarget() {
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}
void GUI::CleanupRenderTarget() {
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
//Handles resizing, minimizing, exiting window
LRESULT WINAPI GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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