#pragma once
#include <string>
#include <extdll.h>

class CHudNumber{
public:
	void Close(edict_t* pent);
	void Send(edict_t* pent, float value);
	void Update(edict_t* pent, float value) const;
public:
	/*
		Channel.
		Range: 0-15 (each module type has its own channel group).
	*/
	unsigned _int8 m_iChannel; // channel to draw on
	/*
		Flags
		See HUD_ELEM, HUD_NUM (numeric display only) and HUD_TIME (time display only) enums.
	*/
	int m_bitsFlags;
	//Default number of digits (numeric display only)
	unsigned __int8 m_iDefDigits;
	//	Maximum number of digits (numeric display only)
	unsigned __int8 m_iMaxDigits;
	/*
		Horizontal position on the screen.
		<0, 1.0> = left to right
		(-1.0, 0) = right to left
		-1.0 = centered
	*/
	float m_fX;
	/*
		Vertical position on the screen.
		<0, 1.0> = top to bottom
		(-1.0, 0) = bottom to top
		-1.0 = centered
	*/
	float m_fY;
	//	Color 1
	unsigned int m_iColor1; // color to draw
	//Color 2
	unsigned int m_iColor2; // color to draw
	//	Fade in time
	float m_flFadeinTime;
	//Fade out time
	float m_flFadeoutTime;
	//	Hold time
	float m_flHoldTime;
	//Effect time
	float m_flFXTime;
	/*
		Effect
		See HUD_EFFECT enum.
	*/
	unsigned __int8 m_iEffect; // effect to use
	//Sprite name
	std::string m_szSpriteName;
	/*
		Sprite left offset
		Range: 0-255
	*/
	unsigned __int8 m_iLeft;
	/*
		Sprite top offset
		Range: 0-255
	*/
	unsigned __int8 m_iTop;
	/*
		Sprite width
		Range : 0 - 512 (0: auto; use total width of the sprite)
	*/
	short m_iWidth;
	/*
		Sprite height
		Range: 0-512 (0: auto; use total height of the sprite)
	*/
	short m_iHeight;
private:
	int m_iSpriteIndex;
};