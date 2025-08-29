#include "CHudNumber.h"

#include "meta_utility.h"

extern int g_msgNumDisplay;
extern int g_msgUpdateNum;

void CHudNumber::Close(edict_t* pent){
	if (pent == nullptr)
		MESSAGE_BEGIN(MSG_ALL, g_msgNumDisplay);
	else
		MESSAGE_BEGIN(MSG_ONE, g_msgNumDisplay, nullptr, pent);
	WRITE_BYTE(0);
	MESSAGE_END();
}

void CHudNumber::Send(edict_t* pent, float value){
	if (pent == nullptr)
		MESSAGE_BEGIN(MSG_ALL, g_msgNumDisplay);
	else
		MESSAGE_BEGIN(MSG_ONE, g_msgNumDisplay, nullptr, pent);
	WRITE_BYTE(m_iChannel);
	WRITE_LONG(m_bitsFlags);
	WRITE_FLOAT(value);
	WRITE_BYTE(m_iDefDigits);
	WRITE_BYTE(m_iMaxDigits);
	WRITE_FLOAT(m_fX);
	WRITE_FLOAT(m_fY);
	WRITE_RGBA(m_iColor1);
	WRITE_RGBA(m_iColor2);
	WRITE_STRING(m_szSpriteName.c_str());
	WRITE_BYTE(m_iLeft);
	WRITE_BYTE(m_iTop);
	WRITE_SHORT(m_iWidth);
	WRITE_SHORT(m_iHeight);
	WRITE_FLOAT(m_flFadeinTime);
	WRITE_FLOAT(m_flFadeoutTime);
	WRITE_FLOAT(m_flHoldTime);
	WRITE_FLOAT(m_flFXTime);
	WRITE_BYTE(m_iEffect);
	MESSAGE_END();
}

void CHudNumber::Update(edict_t* pent, float value) const{
	
	if(pent == nullptr)
		MESSAGE_BEGIN(MSG_ALL, g_msgUpdateNum);
	else
		MESSAGE_BEGIN(MSG_ONE, g_msgUpdateNum, nullptr, pent);
	WRITE_BYTE(m_iChannel);
	WRITE_FLOAT(value);
	MESSAGE_END();
}
