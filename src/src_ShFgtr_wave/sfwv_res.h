
#ifndef __SFWV_RES_H__
#define __SFWV_RES_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_util.h"

namespace miku
{
	////////////////////////////////////////////////////////////////////////////
	// enum

	enum TTileName
	{
		TILENAME_MIN,

		TILENAME_ITEM_SMGAL1, TILENAME_ITEM_SMGAL2, TILENAME_ITEM_SMGAL3,
		TILENAME_ITEM_NETO1,TILENAME_ITEM_NETO2,TILENAME_ITEM_NETO3,
		TILENAME_ITEM_HP, TILENAME_ITEM_SP,

		TILENAME_SMGAL_SP, TILENAME_NETO_SP,
		TILENAME_SMGAL_WEAPON1, TILENAME_SMGAL_WEAPON2, TILENAME_SMGAL_WEAPON3,
		TILENAME_NETO_WEAPON1, TILENAME_NETO_WEAPON2, TILENAME_NETO_WEAPON3,

		TILENAME_SCREW_SHIT, TILENAME_SPIT, TILENAME_WORM, TILENAME_NOSE_WAX, TILENAME_HAIR_WAX, TILENAME_OVEREAT, TILENAME_GAS, TILENAME_SHIT,

		TILENAME_SMGAL1, TILENAME_SMGAL2, TILENAME_NETO1, TILENAME_NETO2,

		TILENAME_ZACO_BIG_SHIT, TILENAME_ZACO_TEETH, TILENAME_ZACO_ROACH, TILENAME_ZACO_FLY,

		TILENAME_BOSS_NOSE_WAX_MAN, TILENAME_BOSS_HAIR_WAX_MAN1, TILENAME_BOSS_HAIR_WAX_MAN2,
		TILENAME_BOSS_OVEREAT_MAN, TILENAME_BOSS_GAS_MAN, TILENAME_BOSS_SHIT_FIGHTER,

		TILENAME_TILE_SHIT_FIELD, TILENAME_TILE_SHIT_FIELD_SUB, TILENAME_TILE_FINGER, TILENAME_TILE_STATUE1, TILENAME_TILE_STATUE2,
		TILENAME_TILE_GROUND, TILENAME_TILE_SHIT_FLY, TILENAME_TILE_MAIN_TITLE, TILENAME_SMGAL_AND_NETO, TILENAME_TILE_STATUS_BAR,

		TILENAME_MAX
	};

	////////////////////////////////////////////////////////////////////////////
	// class

	struct TTile
	{
		unsigned short m_x_len, m_y_len, m_size;
		unsigned char* m_p_sprite;

		TTile();
		TTile(int x1, int y1, int x2, int y2);
		TTile(TTileName number);
		virtual ~TTile();
	};

	////////////////////////////////////////////////////////////////////////////
	// external

	extern TTile* tile_data[TILENAME_MAX]; // array[succ(START_OF_TILE_NAME)..pred(TILE_NAME_MAX)] of PTile;

	extern unsigned char p_res_story[];
	extern unsigned long sizeof_p_res_story(void);

	extern unsigned char p_res_map[];
	extern unsigned long sizeof_p_res_map(void);

	extern unsigned char p_res_shit_img[];
	extern unsigned long sizeof_p_res_shit_img(void);

	enum TResString
	{
		RESSTRING_TITLE,
		RESSTRING_TITLE_MENU1,
		RESSTRING_TITLE_MENU2,
		RESSTRING_TITLE_MENU3,
		RESSTRING_TITLE_MENU4,
		RESSTRING_MAX
	};

	extern void         UpdateResString(void);
	extern const tchar* GetResString(TResString res_string);

} // namespace miku

#endif // #ifndef __SFWV_RES_H__

