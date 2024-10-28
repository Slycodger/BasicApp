#pragma once
#include <string>
#include "Objects.h"

struct scriptBase {
	virtual void start() {}
	virtual void update() {}
	virtual void end() {}
	std::string scrName;
	Object* thisObj = nullptr;
};