#pragma once
#include "api/IEccoScriptSystem.h"

class CEccoScriptSystem : public IEccoScriptSystem {
public:
	virtual chaiscript::ChaiScript* GetScriptEngine() const;
};