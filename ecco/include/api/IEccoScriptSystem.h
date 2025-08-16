#pragma once
#include <extdll.h>
#include <chaiscript/chaiscript.hpp>

class IEccoScriptSystem {
public:
	virtual chaiscript::ChaiScript* GetScriptEngine() const = 0;
};