#pragma once
#include <string>
#include <cstdint>
#include <extdll.h>

#pragma region HUD_ENUM
// HUD_EFFECT
#define HUD_EFFECT_NONE 0 // No effect.
#define HUD_EFFECT_RAMP_UP 1 // Linear ramp up from color1 to color2.
#define HUD_EFFECT_RAMP_DOWN 2 // Linear ramp down from color2 to color1.
#define HUD_EFFECT_TRIANGLE 3 // Linear ramp up and ramp down from color1 through color2 back to color1.
#define HUD_EFFECT_COSINE_UP 4 // Cosine ramp up from color1 to color2.
#define HUD_EFFECT_COSINE_DOWN 5 // Cosine ramp down from color2 to color1.
#define HUD_EFFECT_COSINE 6 // Cosine ramp up and ramp down from color1 through color2 back to color1.
#define HUD_EFFECT_TOGGLE 7 // Toggle between color1 and color2.
#define HUD_EFFECT_SINE_PULSE 8 // Sine pulse from color1 through zero to color2.

// HUD_ELEM
#define HUD_ELEM_ABSOLUTE_X 1 // X position in pixels.
#define HUD_ELEM_ABSOLUTE_Y 2 // Y position in pixels.
#define HUD_ELEM_SCR_CENTER_X 4 // X position relative to the center of the screen.
#define HUD_ELEM_SCR_CENTER_Y 8 // Y position relative to the center of the screen.
#define HUD_ELEM_NO_BORDER 16 // Ignore the client-side HUD border (hud_bordersize).
#define HUD_ELEM_HIDDEN 32 // Create a hidden element.
#define HUD_ELEM_EFFECT_ONCE 64 // Play the effect only once.
#define HUD_ELEM_DEFAULT_ALPHA 128 // Use the default client-side HUD alpha (hud_defaultalpha).
#define HUD_ELEM_DYNAMIC_ALPHA 256 // Use the default client-side HUD alpha and flash the element when updated.

// HUD_SPR
#define HUD_SPR_OPAQUE 65536 // Draw opaque sprite.
#define HUD_SPR_MASKED 131072 // Draw masked sprite.
#define HUD_SPR_PLAY_ONCE 262144 // Play the animation only once.
#define HUD_SPR_HIDE_WHEN_STOPPED 524288 // Hide the sprite when the animation stops.

// HUD_NUM
#define HUD_NUM_RIGHT_ALIGN 65536 // Draw right aligned element.
#define HUD_NUM_SEPARATOR 131072 // Draw separator.
#define HUD_NUM_DONT_DRAW_ZERO 262144 // Hide the element if the value is zero.
#define HUD_NUM_LEADING_ZEROS 524288 // Draw leading zeros.
#define HUD_NUM_NEGATIVE_NUMBERS 1048576 // Allow negative values.
#define HUD_NUM_PLUS_SIGN 2097152 // Draw sign for positive values.

// HUD_TIME
#define HUD_TIME_RIGHT_ALIGN 65536 // Draw right aligned element.
#define HUD_TIME_HOURS 131072 // Draw hours.
#define HUD_TIME_MINUTES 262144 // Draw minutes.
#define HUD_TIME_SECONDS 524288 // Draw seconds.
#define HUD_TIME_MILLISECONDS 1048576 // Draw milliseconds.
#define HUD_TIME_ZERO_HOURS 2097152 // Draw hours even if the value is zero.
#define HUD_TIME_FREEZE 4194304 // Freeze the displayed value.
#define HUD_TIME_COUNT_DOWN 8388608 // Count down.
#pragma endregion

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
	uint8_t m_iChannel; // channel to draw on
	/*
		Flags
		See HUD_ELEM, HUD_NUM (numeric display only) and HUD_TIME (time display only) enums.
	*/
	int m_bitsFlags;
	//Default number of digits (numeric display only)
	uint8_t m_iDefDigits;
	//	Maximum number of digits (numeric display only)
	uint8_t m_iMaxDigits;
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
	uint32_t m_iColor1; // color to draw
	//Color 2
	uint32_t m_iColor2; // color to draw
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
	uint8_t m_iEffect; // effect to use
	//Sprite name
	std::string m_szSpriteName;
	/*
		Sprite left offset
		Range: 0-255
	*/
	uint8_t m_iLeft;
	/*
		Sprite top offset
		Range: 0-255
	*/
	uint8_t m_iTop;
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