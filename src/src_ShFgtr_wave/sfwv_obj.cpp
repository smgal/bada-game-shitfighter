
////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_util.h"
#include "sfwv_gfx.h"
#include "sfwv_map.h"
#include "sfwv_obj.h"

namespace miku
{

////////////////////////////////////////////////////////////////////////////////
// variables

TPersistent* objects[MAX_ENEMY + 1];

////////////////////////////////////////////////////////////////////////////////
// function

int CreateObject(TTileName number, int x, int y, int vx, int vy, int ax, int ay, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit)
{
	for (int i = MIN_OBJECTS; i <= MAX_OBJECTS; i++)
	{
		if (!assigned_(objects[i]))
		{
			objects[i] = new TMovable(number, x, y, vx, vy, ax, ay, attribute, power, is_flyable, event_bit);
			return i;
		}
	}

	return 0;
}

int CreateObject(TTileName number, int x, int y, float vx_real, float vy_real, float ax_real, float ay_real, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit)
{
	for (int i = MIN_OBJECTS; i <= MAX_OBJECTS; i++)
	{
		if (!assigned_(objects[i]))
		{
			objects[i] = new TMovable(number, x, y, vx_real, vy_real, ax_real, ay_real, attribute, power, is_flyable, event_bit);
			return i;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// class TPersistent

TPersistent::TPersistent(TTileName number, int hit_point, bool is_flyable)
	: TTile(number), m_number(number),
	  m_sx_f16(0), m_sy_f16(0),
	  m_hit_ticks(0), m_hit_point(hit_point),
	  m_is_flyable(is_flyable), m_max_jump(100), m_is_jumping(ON_GROUND)
{
}

TPersistent::~TPersistent()
{
}

bool TPersistent::IsValid(void)
{
	return (m_hit_point > 0);
}

int TPersistent::GetPosCenterX(void)
{
	return ((m_sx_f16 >> 16) + m_x_len / 2);
}

int TPersistent::GetPosCenterY(void)
{
	return ((m_sy_f16 >> 16) + m_y_len / 2);
}

void TPersistent::Move(int x1, int y1)
{
	int adder = (x1 > 0) ? m_x_len : 0;

	CStageMap& map = avej_lite::singleton<CStageMap>::get();

	while ((x1 != 0) && (!map.NotOnPlatform(GetPosX() + adder + x1, GetPosY() + y1 + m_y_len - (FOOT_STEP + 1))))
	{
		if (x1 > 0)
			--x1;
		if (x1 < 0)
			++x1;
	}

	while ((x1 != 0) && (!map.NotOnPlatform(GetPosX() + adder + x1, GetPosY() + y1)))
	{
		if (x1 > 0)
			--x1;
		if (x1 < 0)
			++x1;
	}

	while ((y1 != 0) && (!map.NotOnPlatform(GetPosX() + x1, GetPosY() + y1 + m_y_len - (FOOT_STEP + 1))
	                     || !map.NotOnPlatform(GetPosX() + m_x_len + x1, GetPosY() + y1 + m_y_len - (FOOT_STEP + 1))))
	{
		if (y1 > 0)
			--y1;
		if (y1 < 0)
			++y1;
	}

	while ((y1 != 0) && (!map.NotOnPlatform(GetPosX() + x1, GetPosY() + y1)
	                     || !map.NotOnPlatform(GetPosX() + m_x_len + x1, GetPosY() + y1)))
	{
		if (y1 > 0)
			--y1;
		if (y1 < 0)
			++y1;
	}

	MoveY(y1);

	if (GetPosY() < VIEWPORT_Y1 - Y_DOWN / 2)
		SetPosY(VIEWPORT_Y1 - Y_DOWN / 2);
	if (GetPosY() + m_y_len > VIEWPORT_Y2)
		SetPosY(VIEWPORT_Y2 - m_y_len);

	MoveX(x1);

	if (GetPosX() < VIEWPORT_X1)
	{
		SetPosX(VIEWPORT_X1);
		y1 = 0;

		while ((GetPosY() + y1 > VIEWPORT_Y1) && (! map.NotOnPlatform(GetPosX() + m_x_len, GetPosY() + y1 + m_y_len - (FOOT_STEP + 1))))
			--y1;

		while ((GetPosY() + y1 > VIEWPORT_Y1) && (! map.NotOnPlatform(GetPosX() + m_x_len, GetPosY() + y1)))
			++y1;

		MoveY(y1);
	}

	if (GetPosX() + m_x_len + 8 > VIEWPORT_X2)
		SetPosX(VIEWPORT_X2 - m_x_len - 8);
}

void TPersistent::Warp(int x, int y)
{
	if ((x >= VIEWPORT_X1) && (x + m_x_len <= VIEWPORT_X2))
		SetPosX(x);
	if ((y >= VIEWPORT_Y1) && (y + m_y_len <= VIEWPORT_Y2))
		SetPosY(y);
}

void TPersistent::Display(void)
{
	if (GetPosY() + m_y_len + Y_DOWN < SCREEN_Y_MAX)
		gfx::DrawSprite(GetPosX(), GetPosY() + Y_DOWN, m_p_sprite, (m_hit_ticks == 0));

	if (m_hit_ticks > 0)
		--m_hit_ticks;
}

bool TPersistent::CheckIfCrash(int x1, int y1, int x2, int y2, int power)
{
	bool ret = ((x1 >= GetPosX()) && (x1 < GetPosX() + m_x_len) &&
	            (y1 >= GetPosY()) && (y1 < GetPosY() + m_y_len)) ||
	           ((x2 >= GetPosX()) && (x2 < GetPosX() + m_x_len) &&
	            (y2 >= GetPosY()) && (y2 < GetPosY() + m_y_len));

	if (ret)
	{
		m_hit_ticks  = 1;
		m_hit_point -= power;
	}

	return ret;
}

bool TPersistent::ProcessByAI(void)
{
	return true;
}

void TPersistent::ProcessByGravity(void)
{
	if (m_is_flyable)
		return;

	const int TEMP_FOOT_STEP = (this->GetTypeId() == typeof(TFriendSMgal)) ? FOOT_STEP : 0;

	int test1, test2;

	CStageMap& map = avej_lite::singleton<CStageMap>::get();

	if (m_is_jumping != ON_GROUND)
	{
		switch (m_is_jumping)
		{
			case JUMP_UP   :
			{
				MoveY(-m_jump * 2);

				while ((!map.NotOnPlatform(GetPosX(), GetPosY()) || !map.NotOnPlatform(GetPosX() + m_x_len, GetPosY())))
				{
					MoveY(1);
					m_is_jumping = FALL_DOWN;
				}

				if (--m_jump <= 0)
					m_is_jumping = FALL_DOWN;
			}
			break;
			case FALL_DOWN :
			{
				MoveY(m_jump * 2);
				++m_jump;

				//?? 원래 20임. 현재 상수로 정의했음
				if (m_jump > 5)
					m_jump = 5;

				test1 = map.GetGravity(GetPosX(), GetPosY() + m_y_len - TEMP_FOOT_STEP);
				test2 = map.GetGravity(GetPosX() + m_x_len, GetPosY() + m_y_len - TEMP_FOOT_STEP);

				if ((test1 <= 0) || (test2 <= 0))
				{
					m_is_jumping = ON_GROUND;

					if (test1 <= 0)
						MoveY(test1);
					else
						MoveY(test2);
				}

				if (GetPosY() + m_y_len >= VIEWPORT_Y2 - FOOT_STEP - 3)
				{
					if (this->GetTypeId() == typeof(TFriendSMgal))
					{
						SetPosY(VIEWPORT_Y1);
					}
				}
			}
			break;
			default:
				break;
		}
	}
	else
	{
		test1 = map.GetGravity(GetPosX(), GetPosY() + m_y_len - TEMP_FOOT_STEP);
		test2 = map.GetGravity(GetPosX() + m_x_len, GetPosY() + m_y_len - TEMP_FOOT_STEP);

		if ((test1 > 0) && (test2 > 0))
		{
			m_is_jumping = FALL_DOWN;
			m_jump = 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// class TMovable

TMovable::TMovable(TTileName number, int x, int y, int vx, int vy, int ax, int ay, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit)
	: TPersistent(number, power, is_flyable)
{
	if ((vx != 0) || (vy != 0) || (ax != 0) || (ay != 0))
	{
		SetPosX(x);
		SetPosY(y);

		m_sx_f16      = x  << 16;
		m_sy_f16      = y  << 16;
		m_vx_f16      = vx << 16;
		m_vy_f16      = vy << 16;
		m_ax_f16      = ax << 16;
		m_ay_f16      = ay << 16;
		m_attribute   = attribute;
		m_event_bit   = event_bit;
		m_destination = 0;
	}
	else
	{
		SetPosX(-1);
	}
}

#define FLOAT_TO_FIXED16(f)  int(f * 65536)

TMovable::TMovable(TTileName number, int x, int y, float vx_real, float vy_real, float ax_real, float ay_real, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit)
	: TPersistent(number, power, is_flyable)
{
	if ((vx_real != 0.0f) || (vy_real != 0.0f) || (ax_real != 0.0f) || (ay_real != 0.0f))
	{
		SetPosX(x);
		SetPosY(y);

		m_sx_f16      = x  << 16;
		m_sy_f16      = y  << 16;
		m_vx_f16      = FLOAT_TO_FIXED16(vx_real);
		m_vy_f16      = FLOAT_TO_FIXED16(vy_real);
		m_ax_f16      = FLOAT_TO_FIXED16(ax_real);
		m_ay_f16      = FLOAT_TO_FIXED16(ay_real);
		m_attribute   = attribute;
		m_event_bit   = event_bit;
		m_destination = 0;
	}
	else
	{
		SetPosX(-1);
	}
}

bool TMovable::IsValid(void)
{
	if (m_attribute != PROJECTILEATTR_PARTICLE)
	{
		//?? 상수가 들어감
		if ((GetPosX() < VIEWPORT_X1) || (GetPosX() + m_x_len > VIEWPORT_X2) ||
		                (GetPosY() < VIEWPORT_Y1 - 150) || (GetPosY() + m_y_len > VIEWPORT_Y2 + 150))
			return false;
		else
			return true;
	}
	else
	{
		if (m_hit_point > 0)
		{
			--m_hit_point;
			return true;
		}
		else
		{
			return false;
		}
	}
}

void TMovable::MoveSub(void)
{
	m_sx_f16 += m_vx_f16;
	m_sy_f16 += m_vy_f16;

	m_vx_f16 += m_ax_f16;
	m_vy_f16 += m_ay_f16;

	SetPosX(m_sx_f16 >> 16);
	SetPosY(m_sy_f16 >> 16);
}

bool TMovable::ProcessByAI(void)
{
	bool result = true;

	if (m_attribute == PROJECTILEATTR_NO_DAMAGE || m_attribute == PROJECTILEATTR_FRIEND_ONLY || m_attribute == PROJECTILEATTR_FRIEND_ENEMY)
	{
		for (int obj_index = MIN_FRIEND; obj_index <= MAX_FRIEND; obj_index++)
		{
			if (!assigned_(objects[obj_index]))
				continue;

//			TFriend* p_temp_friend = (TFriend*)objects[obj_index];
			TPersistent* p_temp_friend = (TPersistent*)objects[obj_index];

			if (p_temp_friend->CheckIfCrash(GetPosX(), GetPosY(), (GetPosX() + m_x_len - 1), (GetPosY() + m_y_len - 1), m_hit_point))
			{
				p_temp_friend->ProcessHit(*this, m_attribute, m_event_bit);

				SetPosX(-1);
				return result;
			}
		}
	}

	if ((m_attribute == PROJECTILEATTR_ENEMY_ONLY) || (m_attribute == PROJECTILEATTR_FRIEND_ENEMY) || (m_attribute == PROJECTILEATTR_ALL_ENEMY_SIDE) || (m_attribute == PROJECTILEATTR_ENEMY_PIERCING))
	{
		for (int obj_index = MIN_ENEMY; obj_index <= MAX_ENEMY; obj_index++)
		{
			if (!assigned_(objects[obj_index]))
				continue;

			if (objects[obj_index]->CheckIfCrash(GetPosX(), GetPosY(), (GetPosX() + m_x_len - 1), (GetPosY() + m_y_len - 1), m_hit_point))
			{
				if (m_attribute == PROJECTILEATTR_ENEMY_PIERCING)
				{
					MoveSub();
				}
				else
				{
					CreateObject(TILENAME_SMGAL_SP, GetPosX(), GetPosY(), SCROLL_SPEED_CALC(-(random(2) + 1) * 8), SCROLL_SPEED_CALC(random(7) - 2), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_PARTICLE, 10, true, EVENTBIT_NO_EVENT);
					SetPosX(-1);
				}

				return result;
			}
		}
	}

	if ((m_attribute == PROJECTILEATTR_ALL_ENEMY_SIDE) || (m_attribute == PROJECTILEATTR_ENEMY_PIERCING))
	{
		for (int obj_index = MIN_OBJECTS; obj_index <= MAX_OBJECTS; obj_index++)
		{
			if (assigned_(objects[obj_index]))
			{
				TMovable* p_temp_object = (TMovable*)objects[obj_index];
				if ((p_temp_object->m_attribute == PROJECTILEATTR_FRIEND_ONLY) || (p_temp_object->m_attribute == PROJECTILEATTR_FRIEND_ENEMY))
				{
					if (p_temp_object->CheckIfCrash(GetPosX(), GetPosY(), (GetPosX() + m_x_len - 1), (GetPosY() + m_y_len - 1), m_hit_point))
					{
						p_temp_object->SetPosX(-1);
						return result;
					}
				}
			}
		}
	}

	if ((m_event_bit & EVENTBIT_HOMMING) > 0)
	{
		if ((m_destination > 0) && (m_destination <= MAX_ENEMY))
		{
			if (assigned_(objects[m_destination]))
			{
				int obj_x = objects[m_destination]->GetPosCenterX() - GetPosX();
				int obj_y = objects[m_destination]->GetPosY() - GetPosY();

				double divide = 0.0;

				if (SCROLL_SPEED > 1)
					divide = SF_SQRT(double(double(obj_x) * obj_x + double(obj_y) * obj_y)) / (SCROLL_SPEED / 2);
				else
					divide = SF_SQRT(double(double(obj_x) * obj_x + double(obj_y) * obj_y));

				if (divide > 0.00001)
				{
					m_ax_f16 = round_((obj_x << 16) / divide);
					m_ay_f16 = round_((obj_y << 16) / divide);

					if (((m_vx_f16 + m_ax_f16) >> 16) >  16)
						m_ax_f16 =  16 - m_vx_f16;
					if (((m_vy_f16 + m_ay_f16) >> 16) >  16)
						m_ay_f16 =  16 - m_vy_f16;
					if (((m_vx_f16 + m_ax_f16) >> 16) < -16)
						m_ax_f16 = -16 - m_vx_f16;
					if (((m_vy_f16 + m_ay_f16) >> 16) < -16)
						m_ay_f16 = -16 - m_vy_f16;
				}
			}
			else
			{
				m_ax_f16      = 0;
				m_ay_f16      = 0;
				m_destination = 0;
				m_event_bit  &= ~EVENTBIT_HOMMING;
			}
		}
		else
		{
			m_ax_f16      = 0;
			m_ay_f16      = 0;
			m_destination = 0;
			m_event_bit  &= ~EVENTBIT_HOMMING;
		}
	}

	MoveSub();

	return result;
}

void TMovable::SetDestination(int minimum, int maximum)
{
	int number = 0;

	for (int obj_index = minimum; obj_index <= maximum; obj_index++)
	{
		if (assigned_(objects[obj_index]))
			++number;
	}

	if (number > 0)
	{
		int magic = (random(number) + 1);

		number = 0;

		for (int obj_index = minimum; obj_index <= maximum; obj_index++)
		{
			if (assigned_(objects[obj_index]))
				++number;

			if (number == magic)
			{
				m_destination = obj_index;
				return;
			}
		}
	}
	else
	{
		m_destination = 0;
	}
}

} // namespace miku

