#pragma once
#include <string>

struct scriptBase {
	virtual void start() {}
	virtual void update() {}
	virtual void end() {}
	std::string scrName;
	void* vThisObj = nullptr;
};