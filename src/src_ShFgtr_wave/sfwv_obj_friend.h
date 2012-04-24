
#ifndef __SFWV_OBJ_FRIEND_H__
#define __SFWV_OBJ_FRIEND_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_res.h"
#include "sfwv_obj.h"
#include "sfwv_obj_enemy.h"

namespace miku
{
	////////////////////////////////////////////////////////////////////////////
	// enum

	enum TWeaponType
	{
		WEAPONTYPE_MIN,
		WEAPONTYPE_MAIN,
		WEAPONTYPE_SUB,
		WEAPONTYPE_SPECIAL,
		WEAPONTYPE_MAX
	};

	enum TPlayerKeyType
	{
		PLAYERKEYTYPE_SINGLE,
		PLAYERKEYTYPE_DOUBLE
	};

	////////////////////////////////////////////////////////////////////////////
	// external

	extern void SetPlayerKeyType(TPlayerKeyType player_key_type);

	////////////////////////////////////////////////////////////////////////////
	// class TFriend

	struct TFriend: public TEnemy
	{
		REGISTER_TYPE_ID(TFriend);

		int m_absolute_number, m_weapon_delay, m_weapon_y, m_special_weapon;
		struct
		{
		   TWeaponType weapon;
		   int         level[WEAPONTYPE_MAX]; // array[succ(START_OF_TWEAPON)..pred(WEAPONTYPE_MAX)] of byte;
		} m_weapon;

		TFriend(TTileName number, int hit_point, bool is_flyable, byte max_frame, byte delay_frame, int absolute_number);
		virtual ~TFriend(void);

		virtual void ProcessHit(const TPersistent& actee, TProjectileAttr attribute, unsigned short event_bit);
		virtual bool ProcessByAI(void);
		virtual void Shoot(int vx, int vy);
		void         ShootSpecial(void);
		void         SetLevel(int level);
		int          GetLevel(void);

	protected:
		virtual bool _ProcessByAI(void) { return false; };
		virtual void _ShootSpecial(void) = 0;
	};

	////////////////////////////////////////////////////////////////////////////
	// class TFriendSMgal

	struct TFriendSMgal: public TFriend
	{
		REGISTER_TYPE_ID(TFriendSMgal);

		TFriendSMgal(int hit_point, int absolute_number)
			: TFriend(TILENAME_SMGAL1, hit_point, false, 2, 5, absolute_number) { }

	protected:
		virtual void _Shoot(int vx, int vy);
		virtual void _ShootSpecial(void);
	};

	////////////////////////////////////////////////////////////////////////////
	// class TFriendNeto

	struct TFriendNeto: public TFriend
	{
		REGISTER_TYPE_ID(TFriendNeto);

		TFriendNeto(int hit_point, int absolute_number)
			: TFriend(TILENAME_NETO1, hit_point, true, 2, 1, absolute_number)	{ }

	protected:
		virtual void _Shoot(int vx, int vy);
		virtual void _ShootSpecial(void);
	};

} // namespace miku

#endif // #ifndef __SFWV_OBJ_FRIEND_H__

