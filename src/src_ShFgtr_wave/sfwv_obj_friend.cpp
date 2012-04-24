
////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_config.h"
#include "sfwv_util.h"
#include "sfwv_sys_desc.h"
#include "sfwv_gfx.h"
#include "sfwv_obj_friend.h"

namespace miku
{

////////////////////////////////////////////////////////////////////////////////
// string resource

#if defined(USE_WCHAR)

	static const tchar RES_STR_ENERGY_INC[] =
	{
		// 에너지 증가
		0xC5D0, 0xB108, 0xC9C0, 0x0020, 0xC99D, 0xAC00, 0x0000
	};
	
	static const tchar RES_STR_LEVEL_UP[] =
	{
		// 레벨 업 !!
		0xB808, 0xBCA8, 0x0020, 0xC5C5, 0x0020, 0x0021, 0x0021, 0x0000
	};
	
	static const tchar RES_STR_WEAPON_EXCHANGE[] =
	{
		// 다른 무기로
		0xB2E4, 0xB978, 0x0020, 0xBB34, 0xAE30, 0xB85C, 0x0000
	};
	
	static const tchar RES_STR_SPECIAL_INC[] =
	{
		// 특수 무기 증가
		0xD2B9, 0xC218, 0x0020, 0xBB34, 0xAE30, 0x0020, 0xC99D, 0xAC00, 0x0000
	};

#else
	static const tchar* RES_STR_ENERGY_INC = "에너지 증가";
	static const tchar* RES_STR_LEVEL_UP = "레벨 업 !!";
	static const tchar* RES_STR_WEAPON_EXCHANGE = "다른 무기로";
	static const tchar* RES_STR_SPECIAL_INC = "특수 무기 증가";

#endif


////////////////////////////////////////////////////////////////////////////////
// enum

enum TKeyType
{
	KEYTYPE_LEFT_KEY,
	KEYTYPE_RIGHT_KEY,
	KEYTYPE_UP_KEY,
	KEYTYPE_DOWN_KEY,
	KEYTYPE_WEAPON_KEY,
	KEYTYPE_SPECIAL_KEY
};

////////////////////////////////////////////////////////////////////////////////
// constant

avej_lite::TInputKey key_data1[FRIEND_No][KEYTYPE_SPECIAL_KEY+1] =
{
	{ avej_lite::INPUT_KEY_LEFT, avej_lite::INPUT_KEY_RIGHT, avej_lite::INPUT_KEY_UP, avej_lite::INPUT_KEY_DOWN, avej_lite::INPUT_KEY_A, avej_lite::INPUT_KEY_B, },
	{ avej_lite::INPUT_KEY_LEFT, avej_lite::INPUT_KEY_RIGHT, avej_lite::INPUT_KEY_UP, avej_lite::INPUT_KEY_DOWN, avej_lite::INPUT_KEY_A, avej_lite::INPUT_KEY_B, }
};

avej_lite::TInputKey key_data2[FRIEND_No][KEYTYPE_SPECIAL_KEY+1] =
{
#if 0 // 이것은 WIZ의 2인용 키 배열
	{ avej_lite::INPUT_KEY_LEFT, avej_lite::INPUT_KEY_RIGHT, avej_lite::INPUT_KEY_UP, avej_lite::INPUT_KEY_DOWN, avej_lite::INPUT_KEY_END, avej_lite::INPUT_KEY_L1, },
	{ avej_lite::INPUT_KEY_D, avej_lite::INPUT_KEY_A, avej_lite::INPUT_KEY_C, avej_lite::INPUT_KEY_B, avej_lite::INPUT_KEY_END, avej_lite::INPUT_KEY_R1, }
#else // bada는 하나의 디바이스로 2인용이 불가능 하니...
	{ avej_lite::INPUT_KEY_LEFT, avej_lite::INPUT_KEY_RIGHT, avej_lite::INPUT_KEY_UP, avej_lite::INPUT_KEY_DOWN, avej_lite::INPUT_KEY_A, avej_lite::INPUT_KEY_B, },
	{ avej_lite::INPUT_KEY_LEFT, avej_lite::INPUT_KEY_RIGHT, avej_lite::INPUT_KEY_UP, avej_lite::INPUT_KEY_DOWN, avej_lite::INPUT_KEY_A, avej_lite::INPUT_KEY_B, }
#endif
};

////////////////////////////////////////////////////////////////////////////////
// type definition

typedef avej_lite::TInputKey TInputKeyList[FRIEND_No][KEYTYPE_SPECIAL_KEY+1];

////////////////////////////////////////////////////////////////////////////////
// static

static TInputKeyList* p_key_data = &key_data1;

////////////////////////////////////////////////////////////////////////////////
// class TFriend

TFriend::TFriend(TTileName number, int hit_point, bool is_flyable, byte max_frame, byte delay_frame, int absolute_number)
: TEnemy(number, hit_point, is_flyable, max_frame, delay_frame)
{
	m_weapon_delay  = 0;
	m_weapon_y      = 0;
	m_weapon.weapon = WEAPONTYPE_MAIN;

	for (TWeaponType TW = succ_(WEAPONTYPE_MIN); TW < WEAPONTYPE_MAX; inc_(TW))
		m_weapon.level[TW] = 1;

	m_absolute_number = absolute_number;
	m_special_weapon  = 3;
}

TFriend::~TFriend()
{
}


void SetPlayerKeyType(TPlayerKeyType player_key_type)
{
	switch (player_key_type)
	{
	case PLAYERKEYTYPE_SINGLE:
		p_key_data = &key_data1;
		break;
	case PLAYERKEYTYPE_DOUBLE:
		p_key_data = &key_data2;
		break;
	}
}

void TFriend::ProcessHit(const TPersistent& actee, TProjectileAttr attribute, unsigned short event_bit)
{
	if (attribute == PROJECTILEATTR_NO_DAMAGE)
	{
		gui::CFloatMessage& message = avej_lite::singleton<gui::CFloatMessage>::get();

		if ((event_bit & EVENTBIT_HP_UP) > 0)
		{
			if (actee.m_hit_point < 0)
				message.Register(GetPosX(),GetPosY(),TIndexColor(11), RES_STR_ENERGY_INC);
		}

		if ((event_bit & EVENTBIT_LEVEL_UP) > 0)
		{
			//?? 변수 이름 바꾸고, actee.m_hit_point를 왜 TILE_NAME으로 쓰는지 확인 필요.
			int j = 0;
			if (m_number == TILENAME_NETO1)
			{
				if ((TTileName(actee.m_hit_point) == TILENAME_ITEM_NETO1) || (TTileName(actee.m_hit_point) == TILENAME_ITEM_NETO2) || (TTileName(actee.m_hit_point) == TILENAME_ITEM_NETO3))
				{
					j = (actee.m_hit_point - ord_(TILENAME_ITEM_NETO1) + 1);
				}
			}
			else
			{
				if ((TTileName(actee.m_hit_point) == TILENAME_ITEM_SMGAL1) || (TTileName(actee.m_hit_point) == TILENAME_ITEM_SMGAL2) || (TTileName(actee.m_hit_point) == TILENAME_ITEM_SMGAL3))
				{
					j = (actee.m_hit_point - ord_(TILENAME_ITEM_SMGAL1) + 1);
				}
			}

			if (j > 0)
			{
				if (m_weapon.weapon == TWeaponType(j))
				{
					SetLevel(GetLevel()+1);
					message.Register(GetPosX(),GetPosY(),TIndexColor(11), RES_STR_LEVEL_UP);
				} 
				else
				{
					m_weapon.weapon = TWeaponType(j);
					message.Register(GetPosX(),GetPosY(),TIndexColor(11), RES_STR_WEAPON_EXCHANGE);
				}
			}
		}

		if ((event_bit & EVENTBIT_SPECIAL) > 0)
		{
			++m_special_weapon;
			message.Register(GetPosX(),GetPosY(),TIndexColor(11), RES_STR_SPECIAL_INC);
		}
	}
}

static bool s_InRect(int x, int y, const TRect& rect)
{
	return (x >= rect.x && x < (rect.x+rect.w) && y >= rect.y && y < (rect.y+rect.h));
}

#define IS_PRESSING(num, key) (input_device.IsKeyHeldDown((*p_key_data)[m_absolute_number-MIN_FRIEND][key]))
#define IS_PRESSED(num, key)  (input_device.WasKeyPressed((*p_key_data)[m_absolute_number-MIN_FRIEND][key]))

bool TFriend::ProcessByAI(void)
{
	avej_lite::CInputDevice& input_device = avej_lite::singleton<avej_lite::CInputDevice>::get();
/*
	// multi-touch
#if 0
	// 변동 기준 방식
	{
		avej_lite::TMultiTouchInfoArray mt_info_list;

		static bool s_x_slider_pressed = false;
		static int  s_x_slider_pos     = 0;

		static bool s_y_slider_pressed = false;
		static int  s_y_slider_pos     = 0;

		bool has_been_in_x_slider = false;
		bool has_been_in_y_slider = false;

		if (input_device.GetMultiTouchInfo(mt_info_list))
		{
			for (int i = 0; i < sizeof(mt_info_list) / sizeof(mt_info_list[0]); i++)
			{
				const avej_lite::TMultiTouchInfo& mt_info = mt_info_list[i];

				if (s_InRect(mt_info.x, mt_info.y, REGION_X_SLIDE))
				{
					has_been_in_x_slider = true;

					if (s_x_slider_pressed)
					{
						if (abs(mt_info.x - s_x_slider_pos) > SCROLL_SPEED*2)
							Move((mt_info.x - s_x_slider_pos > 0) ? SCROLL_SPEED : -SCROLL_SPEED*2, 0);
					}
					else
					{
						s_x_slider_pressed = true;
						s_x_slider_pos     = mt_info.x;
					}
				}

				if (s_InRect(mt_info.x, mt_info.y, REGION_Y_SLIDE))
				{
					has_been_in_y_slider = true;

					if (s_y_slider_pressed)
					{
						if (abs(mt_info.y - s_y_slider_pos) > SCROLL_SPEED)
							Move(0, (mt_info.y - s_y_slider_pos > 0) ? SCROLL_SPEED : -SCROLL_SPEED);
					}
					else
					{
						s_y_slider_pressed = true;
						s_y_slider_pos     = mt_info.y;
					}
				}
			}
		}

		if (!has_been_in_x_slider)
			s_x_slider_pressed = false;

		if (!has_been_in_y_slider)
			s_y_slider_pressed = false;
	}
#else
	// 슬라이드 식
	{
		avej_lite::TMultiTouchInfoArray mt_info_list;

		if (input_device.GetMultiTouchInfo(mt_info_list))
		{
			for (int i = 0; i < sizeof(mt_info_list) / sizeof(mt_info_list[0]); i++)
			{
				const avej_lite::TMultiTouchInfo& mt_info = mt_info_list[i];

				if (s_InRect(mt_info.x, mt_info.y, REGION_X_SLIDE))
				{
					int current_x = this->GetPosX();
					current_x += (m_x_len / 2);

					if (abs(mt_info.x - current_x) > SCROLL_SPEED*2)
						Move((mt_info.x - current_x > 0) ? SCROLL_SPEED : -SCROLL_SPEED*2, 0);
				}

				if (s_InRect(mt_info.x, mt_info.y, REGION_Y_SLIDE))
				{
					int current_y = this->GetPosY();
					current_y += (m_y_len * 2 / 3);

					if (abs(mt_info.y - current_y) > SCROLL_SPEED)
						Move(0, (mt_info.y - current_y > 0) ? SCROLL_SPEED : -SCROLL_SPEED);
				}
			}
		}
	}
#endif
*/
	if (IS_PRESSING(m_absolute_number, KEYTYPE_LEFT_KEY))
		Move(-SCROLL_SPEED*2,0);
	
	if (IS_PRESSING(m_absolute_number, KEYTYPE_RIGHT_KEY))
		Move(SCROLL_SPEED,0);

	if (IS_PRESSING(m_absolute_number, KEYTYPE_UP_KEY))
	{
		if (m_is_flyable)
		{
			Move(0,-SCROLL_SPEED);
		}
		else
		{
			if (m_is_jumping == ON_GROUND)
			{
				m_is_jumping = JUMP_UP;
				m_jump = 10;
			}
		}
	}

	if (IS_PRESSING(m_absolute_number, KEYTYPE_DOWN_KEY))
	{
		if (m_is_flyable)
		{
			Move(0,SCROLL_SPEED);
		}
		else
		{
			if (m_is_jumping == JUMP_UP)
			{
				m_is_jumping = FALL_DOWN;
			}
			else
			{
				Move(-SCROLL_SPEED,0);
			}
		}
	}

    if (m_auto_shoot > 0)
		ShootSpecial();

    if (GetSysDesc().IsAutoShot() || IS_PRESSING(m_absolute_number, KEYTYPE_WEAPON_KEY))
		Shoot(0,0);

	if (IS_PRESSED(m_absolute_number, KEYTYPE_SPECIAL_KEY))
	{
		if (m_special_weapon > 0)
		{
			m_auto_shoot += 50;
			--m_special_weapon;
		}
//??		ascii_code = ascii_code - [key_data[m_absolute_number][SPECIAL_KEY]];
	}
/*??
	if (_PLUS_KEY in ascii_code)
	{
		dec(m_hit_point);
		ascii_code = ascii_code - [_PLUS_KEY];
	}
*/
	return true;
}

void TFriend::Shoot(int vx, int vy)
{
    if (--m_weapon_delay > 0)
		return;

	this->_Shoot(vx, vy);
}

void TFriend::ShootSpecial(void)
{
	if (--m_auto_shoot < 0)
	{
		m_auto_shoot = 0;
		return;
	}

	this->_ShootSpecial();
}

void TFriend::SetLevel(int level)
{
	if (level >= 1 && level <= MAX_LEVEL)
		m_weapon.level[m_weapon.weapon] = level;
}

int TFriend::GetLevel(void)
{
	return m_weapon.level[m_weapon.weapon];
}

////////////////////////////////////////////////////////////////////////////////
// class TFriendSMgal

void TFriendSMgal::_Shoot(int vx, int vy)
{
	switch (m_weapon.weapon)
	{
		case WEAPONTYPE_MAIN:
			{
				m_weapon_y = (m_weapon_y != 0) ? 0 : 12;

				CreateObject(TILENAME_SMGAL_WEAPON1,GetPosX()+16, GetPosY()+2+m_weapon_y, SCROLL_SPEED*2, 0, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);

				m_weapon_delay = (2 + (2 - m_weapon.level[m_weapon.weapon])) * 8 / SCROLL_SPEED;
			}
			break;

		case WEAPONTYPE_SUB:
			{
				switch (GetLevel())
				{
				case 1:
					CreateObject(TILENAME_SMGAL_WEAPON2, GetPosX()+16, GetPosY()+10, SCROLL_SPEED*2, 0, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
					break;
				case 2:
					CreateObject(TILENAME_SMGAL_WEAPON2, GetPosX()+16, GetPosY()+10, SCROLL_SPEED*2, -SCROLL_SPEED*0, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
					CreateObject(TILENAME_SMGAL_WEAPON2, GetPosX()+16, GetPosY()+10, SCROLL_SPEED*2, -SCROLL_SPEED*1, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
					break;
				case 3:
					CreateObject(TILENAME_SMGAL_WEAPON2, GetPosX()+16, GetPosY()+10, SCROLL_SPEED*2, -SCROLL_SPEED*0/3, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
					CreateObject(TILENAME_SMGAL_WEAPON2, GetPosX()+16, GetPosY()+10, SCROLL_SPEED*2, -SCROLL_SPEED*2/3, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
					CreateObject(TILENAME_SMGAL_WEAPON2, GetPosX()+16, GetPosY()+10, SCROLL_SPEED*2, -SCROLL_SPEED*4/3, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
					break;
				}
/*
				for (j = 0; j < GetLevel(); j++)
				{
					k = -SCROLL_SPEED * j;
					CreateObject(TILENAME_SMGAL_WEAPON2,GetPosX()+16,GetPosY()+10,SCROLL_SPEED*2,k,0,0,PROJECTILEATTR_ENEMY_ONLY,1,true,EVENTBIT_NO_EVENT);
				}
*/
				m_weapon_delay = 3 * 8 / SCROLL_SPEED;
			}
			break;

		case WEAPONTYPE_SPECIAL :
			{
				CreateObject(TILENAME_SMGAL_WEAPON3, GetPosX()+16, GetPosY()+10, SCROLL_SPEED_CALC(12), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_ENEMY_PIERCING, GetLevel(), true, EVENTBIT_NO_EVENT);
				m_weapon_delay = 6 * 8 / SCROLL_SPEED;
			}
			break;
		default:
			break;
	}
}

void TFriendSMgal::_ShootSpecial(void)
{
	const int data_table[5][3] = // array[1..4,1..2] of shortint
	{
		{0 ,0, 0},
		{0, -SCROLL_SPEED,-SCROLL_SPEED}, {0, 0,-SCROLL_SPEED}, {0, SCROLL_SPEED,-SCROLL_SPEED}, {0, SCROLL_SPEED,0}
	};

	if (m_auto_shoot % 4 == 0)
	{
		for (int i = 1; i <= 4; i++)
		{
			int obj_index = CreateObject(TILENAME_SMGAL_SP,GetPosX()+16,GetPosY()+5,data_table[i][1],data_table[i][2],0,0,PROJECTILEATTR_ALL_ENEMY_SIDE,1,true,EVENTBIT_HOMMING);
			if (obj_index > 0)
			{
				TMovable* p_temp_object = (TMovable*)objects[obj_index];
				p_temp_object->SetDestination(MIN_ENEMY,MAX_ENEMY);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// class TFriendNeto

void TFriendNeto::_Shoot(int vx, int vy)
{
#if (SCROLL_SPEED == 4)
	const static float S_TABLE[5][4] =
	{
		{ 16.0f,  0.0f,  0.0f,  0.0f},
		{ 12.0f,-16.0f,  0.0f,  0.5f}, { 12.0f, 16.0f,  0.0f, -0.5f},
		{  0.0f,-16.0f,  0.5f,  0.5f}, {  0.0f, 16.0f,  0.5f, -0.5f}
	};
#elif (SCROLL_SPEED == 2)
	const static float S_TABLE[5][4] =
	{
		{ 16.0f,  0.0f,  0.0f,  0.0f},
		{ 12.0f,-16.0f,  0.0f,  0.275f}, { 12.0f, 16.0f,  0.0f, -0.225f},
		{  0.0f,-16.0f,  0.25f,  0.275f}, {  0.0f, 16.0f,  0.25f, -0.225f}
	};
#else
	const static int S_TABLE[5][4] =
	{
		{16,0,0,0}, {12,-16,0,1}, {12,16,0,-1}, {0,-16,1,1}, {0,16,1,-1}
	};
#endif

	switch (m_weapon.weapon)
	{
		case WEAPONTYPE_MAIN:
			{
				for (int j = 0; j <= GetLevel()*2-2; j++)
				{
					CreateObject(TILENAME_NETO_WEAPON2, GetPosX()+40, GetPosY()+32, SCROLL_SPEED_CALC(S_TABLE[j][0]), SCROLL_SPEED_CALC(S_TABLE[j][1]), SCROLL_SPEED_CALC(S_TABLE[j][2]), SCROLL_SPEED_CALC(S_TABLE[j][3]), PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
				}
				m_weapon_delay = (GetLevel() + 1) * 8 / SCROLL_SPEED;
			}
			break;

		case WEAPONTYPE_SUB:
			{
				int num_enemy = 0;
				for (int obj_index = MIN_ENEMY; obj_index <= MAX_ENEMY; obj_index++)
				{
					if (assigned_(objects[obj_index]))
					++num_enemy;
				}

				if (num_enemy > 0)
				{
					int target   = (random(num_enemy)+1);
					int count    = 0;
					int selected = 0;

					for (int obj_index = MIN_ENEMY; obj_index <= MAX_ENEMY; obj_index++)
					{
						if (assigned_(objects[obj_index]))
							++count;

						if (count == target)
						{
							selected = obj_index;
							break;
						}
					}

					if (assigned_(objects[selected]))
					{
						int obj_x = objects[selected]->GetPosCenterX() - GetPosCenterX();
						int obj_y = objects[selected]->GetPosCenterY() - GetPosCenterY();

						double divide = SF_SQRT(double(double(obj_x)*obj_x+long(obj_y)*obj_y)) / (16.0 * SCROLL_SPEED);

						obj_x = round_(obj_x / divide);
						obj_y = round_(obj_y / divide);

						if ((abs_(obj_x) > abs_(obj_y)) && (obj_x > 0))
						{
							CreateObject(TILENAME_NETO_WEAPON1, GetPosX()+40, GetPosY()+32, SCROLL_SPEED_CALC(obj_x), SCROLL_SPEED_CALC(obj_y), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
						}
						else
						{
							CreateObject(TILENAME_NETO_WEAPON1, GetPosX()+40, GetPosY()+32, SCROLL_SPEED*4, 0, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
						}
						m_weapon_delay = (2 + (2 - GetLevel())) * 8 / SCROLL_SPEED;
					}
				}
				else
				{
					CreateObject(TILENAME_NETO_WEAPON1, GetPosX()+40, GetPosY()+32, SCROLL_SPEED*4, 0, 0, 0, PROJECTILEATTR_ENEMY_ONLY, 1, true, EVENTBIT_NO_EVENT);
					m_weapon_delay = 2 * 8 / SCROLL_SPEED;
				}
			}
			break;

		case WEAPONTYPE_SPECIAL :
			{
				avej_lite::CInputDevice& input_device = avej_lite::singleton<avej_lite::CInputDevice>::get();

				if (IS_PRESSING(m_absolute_number, KEYTYPE_UP_KEY))
				{
					if (m_weapon_y < 16)
						m_weapon_y += 2;
				}
				else if (IS_PRESSING(m_absolute_number, KEYTYPE_DOWN_KEY))
				{
					if (m_weapon_y > -16)
						m_weapon_y -= 2;
				}
				else
				{
					if (m_weapon_y > 0)
						m_weapon_y -= 2;
					if (m_weapon_y < 0)
						m_weapon_y += 2;
				}

				if (GetLevel() <= 3)
				{
					CreateObject(TILENAME_NETO_WEAPON3,GetPosX()+40,GetPosY()+32,SCROLL_SPEED_CALC(16),SCROLL_SPEED_CALC(m_weapon_y),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_ENEMY_PIERCING,1,true,EVENTBIT_NO_EVENT);
				}
				else
				{
					CreateObject(TILENAME_NETO_WEAPON3,GetPosX()+40,GetPosY()+32,SCROLL_SPEED_CALC(16),SCROLL_SPEED_CALC(m_weapon_y+random(5)-2),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_ENEMY_PIERCING,1,true,EVENTBIT_NO_EVENT);
				}

				m_weapon_delay = (3 + (2 - GetLevel())) * 8 / SCROLL_SPEED;
			}
			break;
		default:
			break;
	}
}

void TFriendNeto::_ShootSpecial(void)
{
	if (m_auto_shoot < 50)
	{
		CreateObject(TILENAME_NETO_SP,GetPosX()+16,GetPosY()+5,SCROLL_SPEED_CALC((random(3)+1)*8),SCROLL_SPEED_CALC(random(33)-16),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_ENEMY_ONLY,1,true,EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_NETO_SP,GetPosX()+16,GetPosY()+5,SCROLL_SPEED_CALC((random(3)+1)*8),SCROLL_SPEED_CALC(random(33)-16),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_ENEMY_ONLY,1,true,EVENTBIT_NO_EVENT);
	}
	else
	{
		CreateObject(TILENAME_NETO_WEAPON3,GetPosX()+40,GetPosY()+32,SCROLL_SPEED_CALC((random(2)+1)*16),SCROLL_SPEED_CALC(random(11)-5),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_ENEMY_PIERCING,1,true,EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_NETO_WEAPON3,GetPosX()+40,GetPosY()+32,SCROLL_SPEED_CALC((random(2)+1)*16),SCROLL_SPEED_CALC(random(11)-5),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_ENEMY_PIERCING,1,true,EVENTBIT_NO_EVENT);
	}
}

} // namespace miku

