/* =============================================================
INTRODUCTION TO GAME PROGRAMMING SE102

SAMPLE 00 - INTRODUCTORY CODE

This sample illustrates how to:

1/ Create a window
2/ Initiate DirectX 9, Direct3D, DirectX Sprite
3/ Draw a static brick sprite to the screen
4/ Create frame rate independent movements

5/ Some good C programming practices
- Use constants whenever possible
- 0 Warnings

6/ Debug using __FILE__ __LINE__

WARNING: This one file example has a hell LOT of *sinful* programming practices
================================================================ */
#pragma once
#include "Engine.h"
#include "Debug.h"
#include "Core/Coordinator.h"
#include"Component/PositionComponent.h"
#include"Component/SpriteComponent.h"
#include "System/GraphicSystem.h"
#include "TextureDatabase.h"
#include "EventHandler/EventHandling.h"
#include <fstream>
#include <iostream>

using namespace std;

Engine* engine;
TextureDatabase* textureDb;
Coordinator coordinator;
EventHandling eventHandling;
EntityID tileset[150];
EntityID tilemap[17000];


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


void LoadResource() 
{

	int count = 0;
	Sprite sp[150];
	Position pos[150];
	textureDb = TextureDatabase::GetInstance();
	textureDb->LoadTextureFromPath(TextureID::Brick ,12,12,16,L"lvl2_side.png");

	std::shared_ptr<GraphicSystem> graphicSystem = coordinator.GetSystem<GraphicSystem>(SystemType::Graphic);

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			tileset[count] = coordinator.CreateEntity();

			/*pos[count].x = j * 16;
			pos[count].y = i * 16;
			coordinator.AddComponent(tileset[count], pos[count], ComponentType::Position);*/

			sp[count].textureID = (unsigned int)TextureID::Brick;
			sp[count].area.left = j * 16;
			sp[count].area.top = i * 16;
			sp[count].area.right = j * 16 + 16;
			sp[count].area.bottom = i*16 + 16;
			coordinator.AddComponent(tileset[count], sp[count], ComponentType::Sprite);

			graphicSystem->AddEntity(tileset[count]);

			count++;
		}
	}


	int number;
	int tempcount = 0;
	Position posTile[17000];
	Sprite spriteTile[17000];
	int rowNumber = 0;
	int colNumber = 0;

	ifstream inFile;

	//inFile.open("lvl2_side_tilemap.txt");
	inFile.open("test.txt");
	if (!inFile)
	{
		cout << "Unable to open file";
		exit(1);
	}

	while (inFile >> number)
	{
		for (int i = 0; i < 144; i++)
		{
			if (number == tileset[i])
			{
				tilemap[tempcount] = coordinator.CreateEntity();

				spriteTile[tempcount] = coordinator.GetComponent<Sprite>(tileset[i], ComponentType::Sprite);
				coordinator.AddComponent(tilemap[tempcount], spriteTile[tempcount], ComponentType::Sprite);

				posTile[tempcount].x = colNumber * 16;
				posTile[tempcount].y = rowNumber * 16;
				coordinator.AddComponent(tilemap[tempcount], posTile[tempcount], ComponentType::Position);

				colNumber++;

				tempcount++;

				graphicSystem->AddEntity(tilemap[tempcount]);

				break;
			}
		}
		if (colNumber == 129)
		{
			colNumber = 0;
			rowNumber++;
		}
	}

	inFile.close();

}
/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame

	IMPORTANT: no render-related code should be used inside this function.
*/
void Update(DWORD dt) {

}

/*
	Render a frame
	IMPORTANT: world status must NOT be changed during rendering
*/

void Render() 
{
	LPDIRECT3DDEVICE9 d3ddv = engine->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = engine->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = engine->GetSpriteHandler();
	std::shared_ptr<GraphicSystem> graphicSystem = coordinator.GetSystem<GraphicSystem>(SystemType::Graphic);

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		graphicSystem->SpriteRender();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	//Try this to see how the debug function prints out file and line 
	//wc.hInstance = (HINSTANCE)-100; 

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hWnd = CreateGameWindow(hInstance, nCmdShow, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (hWnd == 0) return 0;
	engine = Engine::GetInstance();
	engine->InitDirectX(hWnd);
	LoadResource();

	Run();	

	return 0;
}