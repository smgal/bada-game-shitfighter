
#ifndef __SFWV_CONFIG_H__
#define __SFWV_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include "avej_lite.h"

////////////////////////////////////////////////////////////////////////////////
// type

namespace miku
{
	typedef unsigned char  byte;
	typedef unsigned char  TImage;

	struct TRect
	{
		int x, y, w, h;
	};
	
	////////////////////////////////////////////////////////////////////////////
	// state

	enum TState
	{
		STATE_TITLE = 0,
		STATE_MENU_ABOUT,
		STATE_MENU_STORY,
		STATE_MENU_OPTION,
		STATE_GAME_PLAY,
		STATE_ENDING_BAD,
		STATE_ENDING_HAPPY,
		STATE_EXIT
	};

	extern void g_ChangeState(TState state);
}

////////////////////////////////////////////////////////////////////////////////
// constant

#define TILE_X_SIZE    48
#define TILE_Y_SIZE    48

#define SCREEN_X_MAX   800
#define SCREEN_Y_MAX   400

#define VIEWPORT_X1    0
#define VIEWPORT_Y1    0
#define VIEWPORT_X2    SCREEN_X_MAX
#define VIEWPORT_Y2    399

#define MARGIN         96

#define FOOT_STEP      6
#define Y_DOWN         17

#define OBJECTS_No     100
#define FRIEND_No      2
#define ENEMY_No       10

#define MIN_OBJECTS    1
#define MAX_OBJECTS    MIN_OBJECTS + (OBJECTS_No-1)
#define MIN_FRIEND     (MAX_OBJECTS+1)
#define MAX_FRIEND     MIN_FRIEND + (FRIEND_No-1)
#define MIN_ENEMY      (MAX_FRIEND+1)
#define MAX_ENEMY      MIN_ENEMY + (ENEMY_No-1)

#define MAX_LEVEL      3

#define GLYPH_W        12
#define GLYPH_H        24

#if (TARGET_DEVICE == TARGET_BADA)
#define SCROLL_SPEED   4
#else
#define SCROLL_SPEED   2
#endif

#define SCROLL_DELAY_CALC(a)  int(8.0f * (a) / SCROLL_SPEED + 0.5f)
#define SCROLL_SPEED_CALC(a)  ((a) * 1.0f * SCROLL_SPEED / 8.0f)


#define STORY_FILE_NAME "stroy.dat"


const miku::TRect REGION_X_SLIDE = { 0, 480-MARGIN, 480, MARGIN };
const miku::TRect REGION_Y_SLIDE = { 800-MARGIN, 0, MARGIN, 480 };
const miku::TRect REGION_SPECIAL = { 520, 480-MARGIN, MARGIN, MARGIN };

////////////////////////////////////////////////////////////////////////////////
// multi-character

#define USE_WCHAR

#if defined(USE_WCHAR)
#	include <wchar.h>
	typedef wchar_t tchar;
#	define TCHAR(s)   L##s

#else
	typedef char tchar;
#	define TCHAR(s)   s

#endif


#endif // #ifndef __SFWV_CONFIG_H__

