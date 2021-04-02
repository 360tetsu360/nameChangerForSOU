#pragma once
#include "MinHook.h"
#include "Memory.h"
#include "TextHolder.h"
#include <map>
class Hook
{
	Hook();
public:
	static Hook* getHook();
	std::map<uint64_t, bool> keys;
	std::map<uint64_t, bool> Mouses;
	void Install();
	void Disable();
private:

};
