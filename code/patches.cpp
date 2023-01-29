#include <Windows.h>
#include <assert.h>

#include "CDetour.h"
#define EXE_BASE_ADDRESS 0x400000

void GSMD5Print(unsigned char digest[16], char output[33])
{
	static const char hex_digits[] = "0123456789abcdef";
	unsigned int i;

	for (i = 0; i < 16; i++)
	{
		output[i * 2] = hex_digits[digest[i] / 16];
		output[i * 2 + 1] = hex_digits[digest[i] % 16];
	}
	output[32] = '\0';
}


void write_string_offset(void* address, int string_len, const char* string) {

	void* write_address = (void*)((ptrdiff_t) (ptrdiff_t)address);
	DWORD oldProtect;

	BOOL success = VirtualProtect(write_address, string_len, PAGE_READWRITE, &oldProtect);


	assert(success);
	SIZE_T numWritten = 0;
	success = WriteProcessMemory(GetCurrentProcess(), write_address, string, string_len, &numWritten);
	assert(success);

	DWORD protect;
	success = VirtualProtect(write_address, string_len, oldProtect, &protect);

	assert(success);

}
void perform_ut3_patches() {
	write_string_offset((void*)0x01D2AE70, 17, "gpsp.openspy.net"); //updated

	write_string_offset((void*)0x01D2A738, 17, "gpcm.openspy.net"); //updated
	write_string_offset((void*)0x01D27DB8, 25, "%s.available.openspy.net"); //updated
	write_string_offset((void*)0x01D2E18C, 20, "%s.ms%d.openspy.net"); //updated

	write_string_offset((void*)0x01B3D338, 64, "http://%s.sake.openspy.net/SakeStorageServer/StorageServer.asmx"); //updated
	write_string_offset((void*)0x01D27CA0, 64, "http://%s.sake.openspy.net/SakeStorageServer/StorageServer.asmx"); //new
	write_string_offset((void*)0x01B3D3E8, 77, "http://%s.comp.pubsvs.openspy.net/CompetitionService/CompetitionService.asmx"); //updated
	write_string_offset((void*)0x01D27AD0, 77, "http://%s.comp.pubsvs.openspy.net/CompetitionService/CompetitionService.asmx"); //new
	write_string_offset((void*)0x01D29634, 22, "%s.master.openspy.net"); //updated 
	write_string_offset((void*)0x01D27E90, 22, "http://motd.openspy.net/motd/motd.asp"); //updated
	write_string_offset((void*)0x01D27E28, 42, "http://motd.openspy.net/motd/vercheck.asp"); //updated
	write_string_offset((void*)0x01D27F08, 42, "http://motd.openspy.net/motd/vercheck.asp"); //new

	write_string_offset((void*)0x01D27380, 64, "http://%s.auth.pubsvs.openspy.net/AuthService/AuthService.asmx\x00"); //updated

	write_string_offset((void*)0x01B47550, 257, "afb5818995b3708d0656a5bdd20760aee76537907625f6d23f40bf17029e56808d36966c0804e1d797e310fedd8c06e6c4121d963863d765811fc9baeb2315c9a6eaeb125fad694d9ea4d4a928f223d9f4514533f18a5432dd0435c5c6ac8e276cf29489cb5ac880f16b0d7832ee927d4e27d622d6a450cd1560d7fa882c6c13"); //updated

	CDetour detour;

	if (detour.Create((BYTE*)0x01243B08, (const BYTE*)GSMD5Print, DETOUR_TYPE_CALL_FUNC, 5) == 0) {
		OutputDebugString("Failed to detour GSMD5Print (#1)");
		::ExitProcess(0); // exit the hard way
	}

	if (detour.Create((BYTE*)0x0123E9B2, (const BYTE*)GSMD5Print, DETOUR_TYPE_CALL_FUNC, 5) == 0) {
		OutputDebugString("Failed to detour GSMD5Print (#2)");
		::ExitProcess(0); // exit the hard way
	}

	if (detour.Create((BYTE*)0x0123D33A, (const BYTE*)GSMD5Print, DETOUR_TYPE_CALL_FUNC, 5) == 0) {
		OutputDebugString("Failed to detour GSMD5Print (#3)");
		::ExitProcess(0); // exit the hard way
	}
}