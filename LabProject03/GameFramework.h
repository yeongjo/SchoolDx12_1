#pragma once
#include "Timer.h"
#include "Player.h"
#include "Scene.h"
//#define RENDER_WIREFRAME

class CCamera;

class CGameFramework : public Singleton<CGameFramework>{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_nWndClientWidth;
	int m_nWndClientHeight;

	CGameTimer m_GameTimer;

	IDXGIFactory4 *m_pdxgiFactory;
	//DXGI 팩토리 인터페이스에 대한 포인터이다. 
	IDXGISwapChain3 *m_pdxgiSwapChain;
	//스왑 체인 인터페이스에 대한 포인터이다. 주로 디스플레이를 제어하기 위하여 필요하다. 
	ID3D12Device *m_pd3dDevice;
	//Direct3D 디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요하다. 
	bool m_bMsaa4xEnable = false;
	UINT m_nMsaa4xQualityLevels = 0;
	//MSAA 다중 샘플링을 활성화하고 다중 샘플링 레벨을 설정한다. 
	static const UINT m_nSwapChainBuffers = 2;
	//스왑 체인의 후면 버퍼의 개수이다. 
	UINT m_nSwapChainBufferIndex;
	//현재 스왑 체인의 후면 버퍼 인덱스이다. 
	ID3D12Resource *m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap *m_pd3dRtvDescriptorHeap;

	CTexture *m_pColorRenderTex;
	//렌더 타겟 버퍼, 서술자 힙 인터페이스 포인터, 렌더 타겟 서술자 원소의 크기이다.
	ID3D12Resource *m_pd3dDepthStencilBuffer;
	ID3D12DescriptorHeap *m_pd3dDsvDescriptorHeap;
	ID3D12DescriptorHeap *m_pd3dCbvSrvDescriptorHeap;
	//깊이-스텐실 버퍼, 서술자 힙 인터페이스 포인터, 깊이-스텐실 서술자 원소의 크기이다.
	ID3D12CommandQueue *m_pd3dCommandQueue;
	ID3D12CommandAllocator *m_pd3dCommandAllocator;
	ID3D12GraphicsCommandList *m_pd3dCommandList;
	//명령 큐, 명령 할당자, 명령 리스트 인터페이스 포인터이다. 
	ID3D12PipelineState *m_pd3dPipelineState;
	//그래픽스 파이프라인 상태 객체에 대한 인터페이스 포인터이다.
	ID3D12Fence *m_pd3dFence;
	UINT64 m_nFenceValues[m_nSwapChainBuffers];
	HANDLE m_hFenceEvent;
	//펜스 인터페이스 포인터, 펜스의 값, 이벤트 핸들이다.

	CScene *m_pScene;

	CObjectsShader* screenShader;


	_TCHAR m_pszFrameRate[128];
public:
	CCamera *m_pCamera = nullptr;

	//플레이어 객체에 대한 포인터이다.
	CPlayer *m_pPlayer = nullptr;
	//마지막으로 마우스 버튼을 클릭할 때의 마우스 커서의 위치이다. 
	POINT m_ptOldCursorPos; 

	CGameObject *m_pSelectedObject = nullptr;
public:
	CGameFramework();
	~CGameFramework(){
	}
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	//프레임워크를 초기화하는 함수이다(주 윈도우가 생성되면 호출된다).
	void OnDestroy();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();
	void ChangeSwapChainState();
	//스왑 체인, 디바이스, 서술자 힙, 명령 큐/할당자/리스트를 생성하는 함수이다. 
	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	//렌더 타겟 뷰와 깊이-스텐실 뷰를 생성하는 함수이다. 
	void BuildObjects();
	void ReleaseObjects();

	void UpdateShaderVariables();
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();
	void WaitForGpuComplete();
	//CPU와 GPU를 동기화하는 함수이다. 

	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam,
		LPARAM lParam);
	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 

	void ProcessSelectedObject(DWORD dwDirection, float cxDelta, float cyDelta);

	float GetTotalTime() { return m_GameTimer.GetTotalTime(); }
	float GetElaspedTime() { return m_GameTimer.GetTimeElapsed(); }
	CScene *GetScene() { return m_pScene; }
};