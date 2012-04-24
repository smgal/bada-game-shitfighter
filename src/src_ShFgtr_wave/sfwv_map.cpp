
#include "sfwv_util.h"
#include "sfwv_sys_desc.h"
#include "sfwv_res.h"
#include "sfwv_map.h"
#include "sfwv_obj.h"
#include "sfwv_obj_enemy.h" // back-call

namespace miku
{

////////////////////////////////////////////////////////////////////////////////
// string resource

#if defined(USE_WCHAR)
	// 방귀 맨
	static const tchar szText000[] = { 0xBC29, 0xADC0, 0x0020, 0xB9E8, 0x0000 };
	// 코딱지 맨
	static const tchar szText001[] = { 0xCF54, 0xB531, 0xC9C0, 0x0020, 0xB9E8, 0x0000 };
	// 비듬 파이터
	static const tchar szText002[] = { 0xBE44, 0xB4EC, 0x0020, 0xD30C, 0xC774, 0xD130, 0x0000 };
	// 오바이트 맨
	static const tchar szText003[] = { 0xC624, 0xBC14, 0xC774, 0xD2B8, 0x0020, 0xB9E8, 0x0000 };
	// 대변 파이터
	static const tchar szText004[] = { 0xB300, 0xBCC0, 0x0020, 0xD30C, 0xC774, 0xD130, 0x0000 };
#else
	static const tchar* szText000 = "방귀 맨";
	static const tchar* szText001 = "코딱지 맨";
	static const tchar* szText002 = "비듬 파이터";
	static const tchar* szText003 = "오바이트 맨";
	static const tchar* szText004 = "대변 파이터";
#endif

static const tchar* RES_STR_BOSS_NAME[] =
{
	szText000,
	szText001,
	szText002,
	szText003,
	szText004,
};

#define MAX_STAGE        5
#define MAP_X_MAX      400
#define MAP_Y_MAX        8

typedef unsigned char TMapArray[MAP_X_MAX][MAP_Y_MAX + 1];

static struct CStageMap_TImpl
{
	TMapArray* map_data;
	TMapArray* map_data_buffer[MAX_STAGE + 1];
	int        map_absolute_x, map_absolute_y;
	int        map_local_x, map_local_y;
	bool       end_of_map;

	CStageMap_TImpl()
		: map_data(0)
	{
	}

	void Reset(void)
	{
		map_data = 0;
	}
} s_impl;


CStageMap::CStageMap()
{
	SF_ASSERT(s_impl.map_data == 0);
/*
	FILE* fp;

	fp = fopen("map.dat", "rb");
	if (fp)
	{
		for (int i = 1; i <= MAX_STAGE; i++)
		{
			s_impl.map_data_buffer[i] = (TMapArray*)new unsigned char[sizeof(TMapArray)];
			fread(s_impl.map_data_buffer[i], sizeof(TMapArray), 1, fp);
		}

		fclose(fp);
	}
	else
*/
	{
		for (int i = 1; i <= MAX_STAGE; i++)
		{
			s_impl.map_data_buffer[i] = (TMapArray*)new unsigned char[sizeof(TMapArray)];
			memcpy(s_impl.map_data_buffer[i], &p_res_map[sizeof(TMapArray) * (i - 1)], sizeof(TMapArray));
		}
	}

	s_impl.map_absolute_x = 0;
	s_impl.map_absolute_y = 3;
	s_impl.map_local_x    = 0;
	s_impl.map_local_y    = 0;
	s_impl.end_of_map     = false;
}

CStageMap::~CStageMap()
{
	for (int i = 1; i <= MAX_STAGE; i++)
		delete[] (unsigned char*)s_impl.map_data_buffer[i];

	s_impl.Reset();
}

void CStageMap::SetCurrentMap(int stage)
{
	s_impl.map_data       = s_impl.map_data_buffer[stage];
	s_impl.map_absolute_x = 0;
	s_impl.map_absolute_y = 1;
	s_impl.map_local_x    = 0;
	s_impl.map_local_y    = 0;
	s_impl.end_of_map     = false;
}

unsigned char CStageMap::GetTile(int a_x, int a_y)
{
	a_x = (a_x + s_impl.map_local_x + 8) / TILE_X_SIZE;
	a_y = (a_y + s_impl.map_local_y    ) / TILE_Y_SIZE;

	if (s_impl.map_absolute_x + a_x < 0)
		return 0;
	if (s_impl.map_absolute_x + a_x >= MAP_X_MAX)
		return 1;

	return (*s_impl.map_data)[s_impl.map_absolute_x + a_x][s_impl.map_absolute_y + a_y];
}

void CStageMap::Move(int x, int y)
{
	const int APPEAR_X = 780 - 150;

	int i, result;

	//?? 상수가 들어 갔음
	if (s_impl.map_absolute_x + 17 >= MAP_X_MAX)
	{
		s_impl.end_of_map = true;
		return;
	}

	s_impl.map_local_x = s_impl.map_local_x + x;
	s_impl.map_local_y = s_impl.map_local_y + y;

	while ((s_impl.map_local_x + x) < 0)
	{
		s_impl.map_local_x += TILE_X_SIZE;
		--s_impl.map_absolute_x;
	}

	while ((s_impl.map_local_x + x) >= TILE_X_SIZE)
	{
		s_impl.map_local_x -= TILE_X_SIZE;
		++s_impl.map_absolute_x;

		result = -1;
		if (s_impl.map_absolute_x + 13 < MAP_X_MAX)
		{
			switch ((*s_impl.map_data)[s_impl.map_absolute_x + 12][0])
			{
				case 1:
					result = CreateEnemy(TILENAME_ZACO_FLY, 5, APPEAR_X, 50, true);
					break;
				case 2:
					result = CreateEnemy(TILENAME_ZACO_ROACH, 10, APPEAR_X, 150, false);
					break;
				case 3:
					result = CreateEnemy(TILENAME_ZACO_BIG_SHIT, 5, APPEAR_X, 100, false);
					break;
				case 4:
					result = CreateEnemy(TILENAME_ZACO_TEETH, 10, APPEAR_X, 150, false);
					break;
				case 5 :
				{
					SetSysDesc().stage.message.remained_ticks = 100;

					tchar stage_message[256];
					util::ComposeString(stage_message, TCHAR("'STAGE @ BOSS : "), GetSysDesc().stage.current_stage);
					// -> sprintf(stage_message, "'STAGE %i BOSS : ", GetSysDesc().stage.current_stage);

					switch (GetSysDesc().stage.current_stage % 5)
					{
						case 1 :
							result = CreateEnemy(TILENAME_BOSS_GAS_MAN, 100, APPEAR_X, 10, false);
							STRCAT(stage_message, RES_STR_BOSS_NAME[0]);
							break;
						case 2 :
							result = CreateEnemy(TILENAME_BOSS_NOSE_WAX_MAN, 200, APPEAR_X, 10, false);
							STRCAT(stage_message, RES_STR_BOSS_NAME[1]);
							break;
						case 3 :
							result = CreateEnemy(TILENAME_BOSS_HAIR_WAX_MAN2, 200, APPEAR_X, 10, false);
							STRCAT(stage_message, RES_STR_BOSS_NAME[2]);
							break;
						case 4 :
							result = CreateEnemy(TILENAME_BOSS_OVEREAT_MAN, 300, APPEAR_X, 10, false);
							STRCAT(stage_message, RES_STR_BOSS_NAME[3]);
							break;
						case 0 :
							result = CreateEnemy(TILENAME_BOSS_SHIT_FIGHTER, 500, APPEAR_X + 20, 10, false);
							STRCAT(stage_message, RES_STR_BOSS_NAME[4]);
							break;
						default:
							//??
							break;
					}

					SetSysDesc().stage.message.Set(stage_message);
				}
				break;
				default:
					//??
					break;
			}
		}

		if (result == 0)
		{
			s_impl.map_local_x += TILE_X_SIZE;
			--s_impl.map_absolute_x;

			s_impl.map_local_x = s_impl.map_local_x - x;
			s_impl.map_local_y = s_impl.map_local_y - y;

			return;
		}
		else if ((*s_impl.map_data)[s_impl.map_absolute_x + 12][0] == 5)
		{
			if (GetSysDesc().use_sound)
			{
				// endTMF;
				// playTMF('Boss');
			}
		}
	}

	while ((s_impl.map_local_y + y) < 0)
	{
		s_impl.map_local_y += TILE_Y_SIZE;
		--s_impl.map_absolute_y;
	}

	while ((s_impl.map_local_y + y) >= TILE_Y_SIZE)
	{
		s_impl.map_local_y -= TILE_Y_SIZE;
		++s_impl.map_absolute_y;
	}

	for (i = MIN_ENEMY; i <= MAX_ENEMY; i++)
	{
		if (objects[i])
		{
			objects[i]->Move(x, y);
			objects[i]->Move(-x, -y);
		}
	}

	for (i = MIN_FRIEND; i <= MAX_FRIEND; i++)
	{
		if (objects[i])
		{
			objects[i]->Move(x, y);
			objects[i]->Move(-x, -y);
		}
	}
}

bool CStageMap::NotOnPlatform(int x, int y)
{
	return (GetTile(x, y) == 0);
}

int  CStageMap::GetGravity(int x, int y)
{
	int _x = x;
	int _y = y;

	if (GetTile(_x, _y) > 0)
	{
		while (GetTile(_x, _y) > 0)
			--_y;

		++_y;

		return (_y - y);
	}
	else
	{
		while (GetTile(_x, _y) == 0)
			++_y;

		return (_y - y);
	}
}

int  CStageMap::GetXOffset(void)
{
	return s_impl.map_local_x;
}

int  CStageMap::GetYOffset(void)
{
	return s_impl.map_local_y;
}

bool CStageMap::IsEndOfMap(void)
{
	return s_impl.end_of_map;
}

} // namespace miku

