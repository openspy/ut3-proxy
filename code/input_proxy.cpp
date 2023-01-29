#include <Windows.h>
#include <stdio.h>
// global variables
typedef HRESULT (__stdcall *DINPUT8_CREATE_FUNC) (HINSTANCE hinst, DWORD dwVersion, const IID* riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

#pragma data_seg (".dinput8_shared")
HINSTANCE           gl_hOriginalDll;
HINSTANCE           gl_hThisInstance;
#pragma data_seg ()

void LoadOriginalDll(void);


HRESULT  __stdcall DirectInput8Create(
	HINSTANCE hinst, DWORD dwVersion, const IID* riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter
) {
	if (!gl_hOriginalDll) LoadOriginalDll();


	DINPUT8_CREATE_FUNC funcAddress_DirectInput8Create = (DINPUT8_CREATE_FUNC)GetProcAddress(gl_hOriginalDll, "DirectInput8Create");

	
	//char dumb_msg[1024];
	//sprintf_s(dumb_msg, sizeof(dumb_msg), "func pointer is: %p, thread: %d\n", funcAddress_DirectInput8Create, GetCurrentThreadId());
	//MessageBoxA(NULL, dumb_msg, "Injected", MB_OK);

	if (!funcAddress_DirectInput8Create)
	{
		OutputDebugString("PROXYDLL: Pointer to original DirectInput8Create function not received ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}

	return  funcAddress_DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

void LoadOriginalDll(void)
{
	char buffer[MAX_PATH];

	// Getting path to system dir and to d3d8.dll
	::GetSystemDirectory(buffer, MAX_PATH);

	// Append dll name
	strcat_s(buffer, sizeof(buffer), "\\dinput8.dll");

	// try to load the system's d3d9.dll, if pointer empty
	if (!gl_hOriginalDll) gl_hOriginalDll = ::LoadLibrary(buffer);
	// Debug
	if (!gl_hOriginalDll)
	{
		OutputDebugString("PROXYDLL: Original d3d9.dinput8 not loaded ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}
}
