#include <Windows.h>
#include "Memory.h"
#include "Hooks.h"
#include "framework.h"
#include <vector>
HMODULE hMod;
std::vector<std::string> split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);

    std::vector<std::string> result;

    while (first < str.size()) {
        std::string subStr(str, first, last - first);

        result.push_back(subStr);

        first = last + 1;
        last = str.find_first_of(del, first);

        if (last == std::string::npos) {
            last = str.size();
        }
    }

    return result;
}
void eject() {
    Hook::getHook()->Disable();
    Sleep(300);
    FreeLibraryAndExitThread(hMod, 1);
}
uintptr_t namedirectadd = 0x03B4FB08;
std::vector<unsigned int> nameoffset = { 0x0,0x38,0x0,0xA0,0x18,0x40,0x20,0x28 };
void setName(std::string newName) {
    uintptr_t add = Mem::getMem()->FindAddress(Mem::getMem()->hProcess, Mem::getMem()->BaseAddress + namedirectadd, nameoffset);
    TextHolder* name = reinterpret_cast<TextHolder*>(add);
    name->setText(newName);
}
void Command(TextHolder* cmd, bool* cancel) {
    std::string cmdstr = cmd->getText();
    if (cmdstr[0] == '.') {
        std::string _Commandstr(cmdstr, 1, cmdstr.size());
        std::vector<std::string> args = split(_Commandstr,' ');
        if (args.size() == 2) {
            if (args[0] == "newname")
                if (args[1] != "") {
                    setName(args[1]);
                    *cancel = true;
                }
        }
    }
}
int Loaded() {
    Hook::getHook()->Install();
    while (true)
    {
        if (Hook::getHook()->keys[17] && Hook::getHook()->keys[76]) {
            eject();
        }
    }
    return 0;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hMod = hModule;
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Loaded, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

