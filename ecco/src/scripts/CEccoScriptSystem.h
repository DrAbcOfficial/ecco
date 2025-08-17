#pragma once
#include "api/IEccoScriptSystem.h"

class CEccoScriptSystem : public IEccoScriptSystem {
public:
	virtual void AddMethod(const char* szName, fnScriptMethodsb pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbs1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbs2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbs3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbn1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbn2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbn3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1s1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1s2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1s3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1n1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1n2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1n3 pfnMethod);

	virtual void AddMethod(const char* szName, fnScriptMethodsn pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsns1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsns2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsns3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnn1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnn2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnn3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1s1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1s2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1s3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1n1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1n2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1n3 pfnMethod);

	virtual void AddMethod(const char* szName, fnScriptMethodsp pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsps1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsps2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsps3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspn1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspn2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspn3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp1s1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp1s2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp1s3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp1n1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp1n2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodspp1n3 pfnMethod);

	virtual void AddMethod(const char* szName, fnScriptMethodss pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsss1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsss2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodsss3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssn1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssn2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssn3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp1s1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp1s2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp1s3 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp1n1 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp1n2 pfnMethod);
	virtual void AddMethod(const char* szName, fnScriptMethodssp1n3 pfnMethod);

	virtual void* GetScriptEngine() const;
};