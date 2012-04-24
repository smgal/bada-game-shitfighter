
////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_util.h"
#include "sfwv_sys_desc.h"
#include "sfwv_obj_enemy.h"

namespace miku
{

////////////////////////////////////////////////////////////////////////////////
// function

int CreateEnemy(TTileName tile_number, int max_power, int x, int y, bool is_flyable)
{
	if (tile_number < MIN_ENEMY && tile_number > MAX_ENEMY)
		return 0;

	for (int obj_index = MIN_ENEMY; obj_index <= MAX_ENEMY; obj_index++)
	{
		if (assigned_(objects[obj_index]))
			continue;

		switch (tile_number)
		{
			case TILENAME_ZACO_BIG_SHIT:
				objects[obj_index] = new TEnemy_ZacoBigShit(max_power, is_flyable);
				break;
			case TILENAME_ZACO_TEETH:
				objects[obj_index] = new TEnemy_ZacoTeeth(max_power, is_flyable);
				break;
			case TILENAME_ZACO_ROACH:
				objects[obj_index] = new TEnemy_ZacoRoach(max_power, is_flyable);
				break;
			case TILENAME_ZACO_FLY:
				objects[obj_index] = new TEnemy_ZacoFly(max_power, is_flyable);
				break;
			case TILENAME_BOSS_NOSE_WAX_MAN:
				objects[obj_index] = new TEnemy_BossNoseWaxMan(max_power, is_flyable);
				break;
			case TILENAME_BOSS_HAIR_WAX_MAN1:
			case TILENAME_BOSS_HAIR_WAX_MAN2:
				objects[obj_index] = new TEnemy_BossHairWaxMan(max_power, is_flyable);
				break;
			case TILENAME_BOSS_OVEREAT_MAN:
				objects[obj_index] = new TEnemy_BossOvereatMan(max_power, is_flyable);
				break;
			case TILENAME_BOSS_GAS_MAN:
				objects[obj_index] = new TEnemy_BossGasMan(max_power, is_flyable);
				break;
			case TILENAME_BOSS_SHIT_FIGHTER:
				objects[obj_index] = new TEnemy_BossShitFighter(max_power, is_flyable);
				break;
			default:
				return 0;
		}

		objects[obj_index]->Warp(x-objects[obj_index]->m_x_len, y-objects[obj_index]->m_y_len);

		return obj_index;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// class TEnemy

TEnemy::TEnemy(TTileName number, int hit_point, bool is_flyable, byte max_frame, byte delay_frame)
: TPersistent(number, hit_point, is_flyable)
{
	for (int i = 0; i < max_frame; i++)
	{
		m_temp_sprite[i+1] = tile_data[TTileName(ord_(number)+i)]->m_p_sprite;
	}

	m_max_frame     = max_frame;
	m_current_frame = 1;
	m_delay_frame   = delay_frame * 8 / SCROLL_SPEED;
	m_frame_count   = 0;
	m_auto_shoot    = 0;
}

TEnemy::~TEnemy()
{
	int number;
	TTileName TN = TILENAME_MIN;

	if (GetPosX() <= VIEWPORT_X1)
		return;

	if (random(4) == 0)
	{
		// 적들은 죽어서 아이템을 남긴다.
		number = 0;
		switch (random(12))
		{
		case 0:
			TN = TILENAME_ITEM_SMGAL1;
			break;
		case 1:
			TN = TILENAME_ITEM_SMGAL2;
			break;
		case 2:
			TN = TILENAME_ITEM_SMGAL3;
			break;
		case 3:
			TN = TILENAME_ITEM_NETO1;
			break;
		case 4:
			TN = TILENAME_ITEM_NETO2;
			break;
		case 5:
			TN = TILENAME_ITEM_NETO3;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			TN = TILENAME_ITEM_HP;
			break;
		case 10:
		case 11:
			TN = TILENAME_ITEM_SP;
			break;
		}

		if (TN == TILENAME_ITEM_HP)
		{
			number = CreateObject(TN,GetPosX(),GetPosY(),-SCROLL_SPEED,0,0,0, PROJECTILEATTR_NO_DAMAGE, -10,false,EVENTBIT_HP_UP);
		}
		else if (TN == TILENAME_ITEM_SP)
		{
			if (random(2) == 0)
				number = CreateObject(TILENAME_ITEM_SP,GetPosX(),GetPosY(),-SCROLL_SPEED,0,0,0,PROJECTILEATTR_NO_DAMAGE,0,false,EVENTBIT_SPECIAL);
			else
				CreateObject(TILENAME_ITEM_SP,GetPosX(),GetPosY(),-SCROLL_SPEED,0,0,0,PROJECTILEATTR_NO_DAMAGE,0,true,EVENTBIT_SPECIAL);
		}
		else
		{
			if (TN >= TILENAME_ITEM_SMGAL1 && TN <= TILENAME_ITEM_SMGAL3)
				number = CreateObject(TN,GetPosX(),GetPosY(),-SCROLL_SPEED,0,0,0,PROJECTILEATTR_NO_DAMAGE,ord_(TN),false,EVENTBIT_LEVEL_UP);
			else
				CreateObject(TN,GetPosX(),GetPosY(),-SCROLL_SPEED,0,0,0,PROJECTILEATTR_NO_DAMAGE,ord_(TN),true,EVENTBIT_LEVEL_UP);
		}

		if (number > TILENAME_MIN)
		{
			objects[number]->m_is_jumping = objects[number]->JUMP_UP;
			objects[number]->m_jump       = 3;
			objects[number]->m_max_jump   = 12;
		}
	}
	else
	{
		CreateObject(TTileName(ord_(m_number)),GetPosX(),GetPosY(),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(-7),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(3.0f * SCROLL_SPEED / 8.0f),PROJECTILEATTR_NO_EFFECT,0,true,EVENTBIT_NO_EVENT);
	}
}

void TEnemy::Display(void)
{
	if (m_max_frame > 0)
	{
		if (m_delay_frame > 0)
		{
			if (++m_frame_count > m_delay_frame)
			{
				m_frame_count = 0;

				if (++m_current_frame > MAX_REGISTERED_FRAME)
					m_current_frame = 1;

				m_p_sprite = m_temp_sprite[m_current_frame];
			}
		}
	}

	TPersistent::Display();
}

void TEnemy::m_MoveRandom(void)
{
	if (m_is_flyable)
	{
		Move((random(3)-1)*SCROLL_SPEED, (random(3)-1)*SCROLL_SPEED);
	}
	else
	{
		Move((random(4)-1)*SCROLL_SPEED, 0);

		if (random(10) == 0)
		if (m_is_jumping == ON_GROUND)
		{
			m_is_jumping = JUMP_UP;
			m_jump       = random(5) + 6;
		}
	}
}

bool TEnemy::ProcessByAI(void)
{
	bool result_by_sub = this->_ProcessByAI();

	if (result_by_sub)
	{
		Shoot(0,0);
		return (GetPosX() > VIEWPORT_X1);
	}
	else
	{
		return false;
	}
}

void TEnemy::Shoot(int vx, int vy)
{
	this->_Shoot(vx, vy);
}

////////////////////////////////////////////////////////////////////////////////
// TEnemy_Xxxx::_ProcessByAI(void)

bool TEnemy_ZacoBigShit::_ProcessByAI(void)
{
	switch (GetSysDesc().stage.current_stage)
	{
	case 1:
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		{
			if (GetSysDesc().stage.current_stage > 3)
				Move((random(4)-1)*SCROLL_SPEED,0);

			if (m_is_jumping == ON_GROUND)
			{
				m_is_jumping = JUMP_UP;
				m_jump = random(GetSysDesc().stage.current_stage*2)+6;
			}
		}
		break;
	}

	return true;
}

bool TEnemy_ZacoTeeth::_ProcessByAI(void)
{
	switch (GetSysDesc().stage.current_stage)
	{
	case 1:
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		{
			if (GetSysDesc().stage.current_stage > 3)
				Move((random(4)-1)*SCROLL_SPEED,0);

			if (m_is_jumping == ON_GROUND)
			{
				m_is_jumping = JUMP_UP;
				m_jump = random(GetSysDesc().stage.current_stage*2)+6;
			}
		}
		break;
	}

	return true;
}

bool TEnemy_ZacoRoach::_ProcessByAI(void)
{
	Move(-SCROLL_SPEED,0);

	if (GetSysDesc().stage.current_stage > 1)
	{
		if (m_is_jumping == ON_GROUND)
		{
			m_is_jumping = JUMP_UP;
			m_jump       = random(GetSysDesc().stage.current_stage*2)+6;
		}
	}

	return true;
}

bool TEnemy_ZacoFly::_ProcessByAI(void)
{
	if (GetSysDesc().stage.current_stage == 1)
		Move(-SCROLL_SPEED*2,0);
	else if (GetSysDesc().stage.current_stage == 5)
		Move(-SCROLL_SPEED,0);
	else
		Move(-2,(random(2)*2-1)*SCROLL_SPEED);

	return true;
}

bool TEnemy_BossGasMan::_ProcessByAI(void)
{
	if (random(5) == 0)
		m_MoveRandom();

	return true;
}

bool TEnemy_BossNoseWaxMan::_ProcessByAI(void)
{
	if (random(5) == 0)
		m_MoveRandom();

	return true;
}

bool TEnemy_BossOvereatMan::_ProcessByAI(void)
{
	if (random(5) == 0)
		m_MoveRandom();

	return true;
}

bool TEnemy_BossHairWaxMan::_ProcessByAI(void)
{
	if (m_auto_shoot == 0)
		m_MoveRandom();

	return true;
}

bool TEnemy_BossShitFighter::_ProcessByAI(void)
{
	Move(SCROLL_SPEED,0);

	if (random(10) == 0)
	if (m_is_jumping == ON_GROUND)
	{
		m_is_jumping = JUMP_UP;
		m_jump       = random(5) + 6;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// TEnemy_Xxxx::_Shoot(int vx, int vy)

void TEnemy_ZacoBigShit::_Shoot(int vx, int vy)
{
	if (random(SCROLL_DELAY_CALC(10)) == 0)
	{
		int repeat = (GetSysDesc().stage.current_stage > 3) ? 2 : 1;

		for (; repeat > 0; --repeat)
		{
			int obj_x = -16;
			int obj_y = -16;

			switch (GetSysDesc().stage.current_stage)
			{
				case 2:
				case 4:
					obj_x = -16 + random(9) - 4;
					break;
				case 3:
				case 5:
					{
						obj_x = -16 + random(9) - 4;
						obj_y = -16 + random(9) - 4;
					}
					break;
			}

			CreateObject(TILENAME_SCREW_SHIT, GetPosX(), GetPosY(), SCROLL_SPEED_CALC(obj_x), SCROLL_SPEED_CALC(obj_y), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(2 * SCROLL_SPEED / 8.0f), PROJECTILEATTR_FRIEND_ONLY, 1, true, EVENTBIT_NO_EVENT);
		}
	}
}

void TEnemy_ZacoTeeth::_Shoot(int vx, int vy)
{
	if (random(SCROLL_DELAY_CALC(20)) == 0)
	{
		CreateObject(TILENAME_SPIT,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,0,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);

		if (GetSysDesc().stage.current_stage > 2)
		{
			CreateObject(TILENAME_SPIT,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-SCROLL_SPEED*2,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
		}

		if (GetSysDesc().stage.current_stage > 4)
		{
			CreateObject(TILENAME_SPIT,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-SCROLL_SPEED,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
		}
	}
}

void TEnemy_ZacoRoach::_Shoot(int vx, int vy)
{
	if (random(SCROLL_DELAY_CALC((6-GetSysDesc().stage.current_stage)*10)) == 0)
		CreateObject(TILENAME_GAS,GetPosX(),GetPosY() + 20,SCROLL_SPEED_CALC(-16),SCROLL_SPEED_CALC(-random(3)*8),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
}

void TEnemy_ZacoFly::_Shoot(int vx, int vy)
{
	if (random(SCROLL_DELAY_CALC(10)) != 0)
		return;

	switch (GetSysDesc().stage.current_stage)
	{
		case 1:
		case 2:
			CreateObject(TILENAME_WORM,GetPosX(),GetPosY()+20,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(8),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
			break;
		case 3:
		case 5:
			{
				if (random(SCROLL_DELAY_CALC(5)) == 0)
				{
					int obj_index = CreateObject(TILENAME_WORM,GetPosX(),GetPosY(),-SCROLL_SPEED,0,0,0,PROJECTILEATTR_NO_DAMAGE,2,false,EVENTBIT_HP_UP);

					if (obj_index > 0)
					{
						objects[obj_index]->m_is_jumping = objects[obj_index]->JUMP_UP;
						objects[obj_index]->m_jump       = 5;
						objects[obj_index]->m_max_jump   = 20;
					}
				}

				if (GetSysDesc().stage.current_stage == 5)
				{
					CreateObject(TILENAME_WORM,GetPosX(),GetPosY()+10,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(8),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
					CreateObject(TILENAME_WORM,GetPosX(),GetPosY()+10,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(random(2)*8),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
				}
			}
			break;
		case 4 :
			{
				CreateObject(TILENAME_WORM,GetPosX(),GetPosY()+10,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(8),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
				CreateObject(TILENAME_WORM,GetPosX(),GetPosY()+10,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(random(2)*8),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
			}
			break;
	}
}

void TEnemy_BossGasMan::_Shoot(int vx, int vy)
{
	if (random(SCROLL_DELAY_CALC(10)) != 0)
		return;

	for (int obj_index = MIN_FRIEND; obj_index <= MAX_FRIEND; obj_index++)
	{
		if (assigned_(objects[obj_index]))
		{
			int obj_x = objects[obj_index]->GetPosCenterX() - GetPosX();
			int obj_y = objects[obj_index]->GetPosY() - GetPosY();

			double divide = SF_SQRT(double(double(obj_x)*obj_x + double(obj_y)*obj_y)) / 16.0;

			CreateObject(TILENAME_GAS, GetPosX(), GetPosY()+20, SCROLL_SPEED_CALC(round_(obj_x/divide)), SCROLL_SPEED_CALC(round_(obj_y/divide)), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_FRIEND_ONLY, 1, true, EVENTBIT_NO_EVENT);

			return;
		}
	}
}

void TEnemy_BossNoseWaxMan::_Shoot(int vx, int vy)
{
	if (random(SCROLL_DELAY_CALC(10)) == 0)
	{
		int obj_x = -16 + random(9) - 4;
		int obj_y = -16 + random(9) - 4;
		int speed = random(3) + 1;

		CreateObject(TILENAME_NOSE_WAX,GetPosX(),GetPosY(),SCROLL_SPEED_CALC(obj_x),SCROLL_SPEED_CALC(obj_y),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(speed * SCROLL_SPEED / 8.0f),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
	}

	if (random(SCROLL_DELAY_CALC(20)) == 0)
	{
		CreateObject(TILENAME_NOSE_WAX,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-SCROLL_SPEED*2,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_NOSE_WAX,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-SCROLL_SPEED,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_NOSE_WAX,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-0,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
	}
}

void TEnemy_BossOvereatMan::_Shoot(int vx, int vy)
{
	if (m_auto_shoot > 0)
	{
		--m_auto_shoot;

		CreateObject(TILENAME_OVEREAT,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-SCROLL_SPEED*2,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_OVEREAT,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-SCROLL_SPEED,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_OVEREAT,GetPosX(),GetPosY()+5,-SCROLL_SPEED*2,-0,0,0,PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
	}
	else
	{
		if (random(SCROLL_DELAY_CALC(200)) == 0)
		{
			m_auto_shoot = 10;
		}

		if (random(SCROLL_DELAY_CALC(7)) == 0)
		{
			for (int obj_index = MIN_FRIEND; obj_index <= MAX_FRIEND; obj_index++)
			{
				if (assigned_(objects[obj_index]))
				{
					int obj_x = objects[obj_index]->GetPosCenterX() - GetPosX();
					int obj_y = objects[obj_index]->GetPosY() - GetPosY();

					double divide = SF_SQRT(double(double(obj_x)*obj_x + double(obj_y)*obj_y)) / 16.0;
					
					CreateObject(TILENAME_OVEREAT,GetPosX(),GetPosY()+20,SCROLL_SPEED_CALC(round_(obj_x/divide)),SCROLL_SPEED_CALC(round_(obj_y/divide)),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
					
					return;
				}
			}
		}

		if (random(SCROLL_DELAY_CALC(10)) == 0)
		{
			int obj_index = CreateObject(TILENAME_OVEREAT,GetPosX(),GetPosY(),-SCROLL_SPEED,0,0,0,PROJECTILEATTR_NO_DAMAGE,2,false,EVENTBIT_HP_UP);
			if (obj_index > 0)
			{
				objects[obj_index]->m_is_jumping = objects[obj_index]->JUMP_UP;
				objects[obj_index]->m_jump       = 5;
				objects[obj_index]->m_max_jump   = 20;
			}
		}
	}
}

void TEnemy_BossHairWaxMan::_Shoot(int vx, int vy)
{
	if (m_auto_shoot > 0)
	{
		--m_auto_shoot;

		if (random(SCROLL_DELAY_CALC(7)) == 0)
		{
			CreateObject(TILENAME_HAIR_WAX,GetPosX(),GetPosY()+5,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(random(33)-16),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
			CreateObject(TILENAME_HAIR_WAX,GetPosX(),GetPosY()+5,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(random(33)-16),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
			CreateObject(TILENAME_HAIR_WAX,GetPosX(),GetPosY()+5,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(random(33)-16),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
			CreateObject(TILENAME_HAIR_WAX,GetPosX(),GetPosY()+5,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(random(33)-16),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
			CreateObject(TILENAME_HAIR_WAX,GetPosX(),GetPosY()+5,SCROLL_SPEED_CALC(-(random(3)+1)*8),SCROLL_SPEED_CALC(random(33)-16),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);
		}

		if (m_auto_shoot == 0)
		{
			m_p_sprite = tile_data[m_number]->m_p_sprite;
		}
	}
	else
	{
		if (random(SCROLL_DELAY_CALC(150)) == 0)
		{
			m_auto_shoot = 70;
			m_p_sprite   = tile_data[m_number-1]->m_p_sprite;
		}
		else if (random(SCROLL_DELAY_CALC(10)) == 0)
		{
			for (int obj_index = MIN_FRIEND; obj_index <= MAX_FRIEND; obj_index++)
			{
				if (assigned_(objects[obj_index]))
				{
					int obj_x = objects[obj_index]->GetPosCenterX() - GetPosX();
					int obj_y = objects[obj_index]->GetPosY() - GetPosY();

					double divide = SF_SQRT(double(double(obj_x)*obj_x + double(obj_y)*obj_y)) / 16.0;

					CreateObject(TILENAME_HAIR_WAX,GetPosX(),GetPosY()+20,SCROLL_SPEED_CALC(round_(obj_x/divide)),SCROLL_SPEED_CALC(round_(obj_y/divide)),SCROLL_SPEED_CALC(0),SCROLL_SPEED_CALC(0),PROJECTILEATTR_FRIEND_ONLY,1,true,EVENTBIT_NO_EVENT);

					return;
				}
			}
		}
	}
}

void TEnemy_BossShitFighter::_Shoot(int vx, int vy)
{
	if (random(SCROLL_DELAY_CALC(10)) == 0)
	{
		if (random(2) == 0)
			CreateEnemy(TILENAME_ZACO_BIG_SHIT, 5, GetPosX()+50, GetPosY()+20, false);
		else
			CreateEnemy(TILENAME_ZACO_FLY, 5, 500, 50, true);
	}

	if (random(SCROLL_DELAY_CALC(10)) == 0)
	{
		CreateObject(TILENAME_SHIT, GetPosX(), GetPosY()+5, SCROLL_SPEED_CALC(-(random(3)+1)*8), SCROLL_SPEED_CALC(random(33)-16), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_FRIEND_ONLY, 1, true, EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_SHIT, GetPosX(), GetPosY()+5, SCROLL_SPEED_CALC(-(random(3)+1)*8), SCROLL_SPEED_CALC(random(33)-16), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_FRIEND_ONLY, 1, true, EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_SHIT, GetPosX(), GetPosY()+5, SCROLL_SPEED_CALC(-(random(3)+1)*8), SCROLL_SPEED_CALC(random(33)-16), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_FRIEND_ONLY, 1, true, EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_SHIT, GetPosX(), GetPosY()+5, SCROLL_SPEED_CALC(-(random(3)+1)*8), SCROLL_SPEED_CALC(random(33)-16), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_FRIEND_ONLY, 1, true, EVENTBIT_NO_EVENT);
		CreateObject(TILENAME_SHIT, GetPosX(), GetPosY()+5, SCROLL_SPEED_CALC(-(random(3)+1)*8), SCROLL_SPEED_CALC(random(33)-16), SCROLL_SPEED_CALC(0), SCROLL_SPEED_CALC(0), PROJECTILEATTR_FRIEND_ONLY, 1, true, EVENTBIT_NO_EVENT);
	}

	if (random(SCROLL_DELAY_CALC(30)) == 0)
	{
		int obj_index = CreateObject(TILENAME_WORM, GetPosX(), GetPosY(), -SCROLL_SPEED, 0, 0, 0, PROJECTILEATTR_NO_DAMAGE, 2, false, EVENTBIT_HP_UP);

		if (obj_index > 0)
		{
			objects[obj_index]->m_is_jumping = objects[obj_index]->JUMP_UP;
			objects[obj_index]->m_jump       = 5;
			objects[obj_index]->m_max_jump   = 20;
		}
	}
}

} // namespace miku

