#include "Hooks.h"
#include "framework.h"
Hook* instance;

Hook::Hook() {

}
Hook* Hook::getHook() {
	if (!instance) {
		instance = new Hook();
	}
	return instance;
}
typedef void(WINAPI* AVKeyItem)(uint64_t key, bool isDown);
AVKeyItem _AVKeyItem;
const char* te = "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 E0 FE FF FF 48 81 EC 20 02 00 00 48 8B 05 AA";
void KeyItemCallback(uint64_t key, bool isDown) {
	bool cancel = false;
	Hook::getHook()->keys[key] = isDown;
	if (!cancel) _AVKeyItem(key, isDown);
}
typedef void(__fastcall* ChatHook)(void*, void*);
ChatHook _ChatHook;
uintptr_t ae = 0;
void ChatCallback(void* a1, void* a2) {
	bool cancelSend = false;
	TextHolder* messageHolder = reinterpret_cast<TextHolder*>(a2);
	Command(messageHolder, &cancelSend);
	if (!cancelSend) _ChatHook(a1, messageHolder);
}

void Hook::Install() {
	MH_Initialize();
	uintptr_t sigAddr = Mem::getMem()->findSig("48 89 5C 24 ?? ?? 48 83 EC ?? 8B 05 ?? ?? ?? ?? 8B DA");
	if (sigAddr) {
		if (MH_CreateHook((void*)sigAddr, &KeyItemCallback, reinterpret_cast<LPVOID*>(&_AVKeyItem)) == MH_OK) {
			MH_EnableHook((void*)sigAddr);
		}
		else {
		}
	}
	sigAddr = Mem::getMem()->findSig(te);
	if (sigAddr) {
		if (MH_CreateHook((void*)sigAddr, &ChatCallback, reinterpret_cast<LPVOID*>(&_ChatHook)) == MH_OK) {
			MH_EnableHook((void*)sigAddr);
		}
		else {
		}
	}
	else {
	}
}
void Hook::Disable() {
	uintptr_t sigAddr = Mem::getMem()->findSig("48 89 5C 24 ?? ?? 48 83 EC ?? 8B 05 ?? ?? ?? ?? 8B DA");
	MH_RemoveHook((LPVOID)sigAddr);
	sigAddr = Mem::getMem()->findSig(te);
	MH_RemoveHook((LPVOID)sigAddr);
	MH_Uninitialize();
}