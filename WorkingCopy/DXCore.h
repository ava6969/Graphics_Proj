#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <dwrite.h>
#include <d2d1.h>
#include <Winuser.h>
#include <d2d1helper.h>
#include <dxgidebug.h>
#define INITIALX_96DPI 50 
#define INITIALY_96DPI 50 

// We can include the correct library files here
// instead of in Visual Studio settings if we want
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class DXCore
{
public:
	DXCore(
		HINSTANCE hInstance,		// The application's handle
		const char* titleBarText,	// Text for the window's title bar
		unsigned int windowWidth,	// Width of the window's client area
		unsigned int windowHeight,	// Height of the window's client area
		bool debugTitleBarStats);	// Show extra stats (fps) in title bar?
	~DXCore();

	// Static requirements for OS-level message processing
	static DXCore* DXCoreInstance;
	static LRESULT CALLBACK WindowProc(
		HWND hWnd,		// Window handle
		UINT uMsg,		// Message
		WPARAM wParam,	// Message's first parameter
		LPARAM lParam	// Message's second parameter
		);


	// Internal method for message handling
	LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Initialization and game-loop related methods
	HRESULT InitWindow();
	HRESULT InitDirectX();
	HRESULT Run();				
	void Quit();
	virtual void OnResize();
	
	// Pure virtual methods for setup and game functionality
	virtual void Init()										= 0;
	virtual void Update(float deltaTime, float totalTime)	= 0;
	virtual void Draw(float deltaTime, float totalTime)		= 0;

	// Convenience methods for handling mouse input, since we
	// can easily grab mouse input from OS-level messages
	virtual void OnMouseDown (WPARAM buttonState, int x, int y) { }
	virtual void OnMouseUp	 (WPARAM buttonState, int x, int y) { }
	virtual void OnMouseMove (WPARAM buttonState, int x, int y) { }
	virtual void OnMouseWheel(float wheelDelta,   int x, int y) { }
	
protected:
	HINSTANCE	hInstance;		// The handle to the application
	HWND		hWnd;			// The handle to the window itself
	std::string titleBarText;	// Custom text in window's title bar
	bool		titleBarStats;	// Show extra stats in title bar?
	
	// Size of the window's client area
	unsigned int width;
	unsigned int height;
	
	// DirectX related objects and variables
	D3D_FEATURE_LEVEL		dxFeatureLevel;
	IDXGISwapChain*			swapChain;
	ID3D11Device*			device;
	ID3D11Debug*			m_d3dDebug;
	ID3D11DeviceContext*	context;

	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

	// Text Rendering Interfaces
	IDWriteFactory* pDWriteFactory_;
	IDWriteTextFormat* pTextFormat_;

	// Direct2D objects
	ID2D1Factory* pD2DFactory_;
	ID2D1RenderTarget* pRT_;
	ID2D1SolidColorBrush* pBlackBrush_;



	HRESULT CreateDeviceResourcesForTextRendering();
	void DiscardDeviceResources();
	HRESULT DrawText();


	// Text Strings to render and its lenght
	const wchar_t* wszText_;
	UINT32 cTextLength_; 

	// Helper function for allocating a console window
	void CreateConsoleWindow(int bufferLines, int bufferColumns, int windowLines, int windowColumns);

private:


	// Timing related data
	double perfCounterSeconds;
	float totalTime;
	float deltaTime;
	__int64 startTime;
	__int64 currentTime;
	__int64 previousTime;

	// FPS calculation
	int fpsFrameCount;
	float fpsTimeElapsed;
	
	void UpdateTimer();			// Updates the timer for this frame
	void UpdateTitleBarStats();	// Puts debug info in the title bar

	// Device Independent resource 
	void CreateDeviceIndependentResources();

};

