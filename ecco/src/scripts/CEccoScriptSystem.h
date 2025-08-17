#pragma once
#include "api/IEccoScriptSystem.h"

class CEccoScriptSystem : public IEccoScriptSystem {
public:
	ADD_METHODS(b)
	ADD_METHODS(p)
	ADD_METHODS(s)
	ADD_METHODS(n)
	ADD_METHODS(f)

	virtual void* GetScriptEngine() const;
};