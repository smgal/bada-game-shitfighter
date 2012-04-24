
#ifndef __SFWV_OBJ_H__
#define __SFWV_OBJ_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_res.h"

namespace miku
{
	////////////////////////////////////////////////////////////////////////////
	// enum

	enum TProjectileAttr
	{
		PROJECTILEATTR_NO_DAMAGE, //?? 아이템인 것 같지만 아닌 것도 같다.
		PROJECTILEATTR_FRIEND_ONLY,
		PROJECTILEATTR_ENEMY_ONLY,
		PROJECTILEATTR_FRIEND_ENEMY,
		PROJECTILEATTR_ALL_ENEMY_SIDE,
		PROJECTILEATTR_ENEMY_PIERCING,
		PROJECTILEATTR_NO_EFFECT,
		PROJECTILEATTR_PARTICLE
	};

	enum TEventBit
	{
		EVENTBIT_NO_EVENT    = 0,
		EVENTBIT_HP_UP       = 1,
		EVENTBIT_LEVEL_UP    = 4,
		EVENTBIT_LEVEL_DOWN  = 8,
		EVENTBIT_SPECIAL     = 16,
		EVENTBIT_HOMMING     = 128
	};

	////////////////////////////////////////////////////////////////////////////
	// external

	extern struct TPersistent* objects[MAX_ENEMY+1]; // array[1..MAX_ENEMY] of 

	extern int CreateObject(TTileName number, int x, int y, int vx, int vy, int ax, int ay, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit);
	extern int CreateObject(TTileName number, int x, int y, float vx_real, float vy_real, float ax_real, float ay_real, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit);

	////////////////////////////////////////////////////////////////////////////
	// macro

	#define typeof(class_name) \
		(unsigned long)((((#class_name)[sizeof(#class_name)*8/9] << 24) | ((#class_name)[sizeof(#class_name)*7/9] << 16) | ((#class_name)[sizeof(#class_name)*6/9] <<  8) | ((#class_name)[sizeof(#class_name)*5/9] <<  0)) ^ \
						(((#class_name)[sizeof(#class_name)*4/9] << 24) | ((#class_name)[sizeof(#class_name)*3/9] << 16) | ((#class_name)[sizeof(#class_name)*2/9] <<  8) | ((#class_name)[sizeof(#class_name)*1/9] <<  0)))

	#define REGISTER_TYPE_ID(class_name) \
		virtual unsigned long GetTypeId(void) { return typeof(class_name); }

	////////////////////////////////////////////////////////////////////////////
	// class TPersistent

	struct TPersistent: public TTile
	{
		REGISTER_TYPE_ID(TPersistent);

		TTileName m_number;
		int       m_sx_f16, m_sy_f16;
		int       m_hit_ticks;
		int       m_hit_point;
		bool      m_is_flyable;
		int       m_jump;
		int       m_max_jump;
		enum { ON_GROUND, JUMP_UP, FALL_DOWN } m_is_jumping;

		TPersistent(TTileName number, int hit_point, bool is_flyable);
		virtual ~TPersistent();

		virtual bool IsValid(void);

		inline int   GetPosX(void) const { return (m_sx_f16 >> 16); }
		inline int   GetPosY(void) const { return (m_sy_f16 >> 16); }
		int          GetPosCenterX(void);
		int          GetPosCenterY(void);
		void         Move(int x1, int y1);
		void         Warp(int x, int y);

		bool         CheckIfCrash(int x1, int y1, int x2, int y2, int power);

		virtual void ProcessHit(const TPersistent& actee, TProjectileAttr attribute, unsigned short event_bit) { }
		virtual bool ProcessByAI(void);
		void         ProcessByGravity(void);

		virtual void Display(void);

	protected:
		inline void  SetPosX(int x) { m_sx_f16 = (x << 16); }
		inline void  SetPosY(int y) { m_sy_f16 = (y << 16); }

	private:
		inline void  MoveX(int dx) { m_sx_f16 += (dx << 16); }
		inline void  MoveY(int dy) { m_sy_f16 += (dy << 16); }
	};

	////////////////////////////////////////////////////////////////////////////
	// class TMovable

	struct TMovable: public TPersistent
	{
		REGISTER_TYPE_ID(TMovable);

		int             m_vx_f16, m_vy_f16;
		int             m_ax_f16, m_ay_f16;
		TProjectileAttr m_attribute;
		unsigned short  m_event_bit;
		int             m_destination;

		TMovable(TTileName number, int x, int y, int vx, int vy, int ax, int ay, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit);
		TMovable(TTileName number, int x, int y, float vx_real, float vy_real, float ax_real, float ay_real, TProjectileAttr attribute, int power, bool is_flyable, unsigned short event_bit);
		virtual bool IsValid(void);
		virtual bool ProcessByAI(void);
		void         SetDestination(int minimum, int maximum);

	private:
		void         MoveSub(void);
	};

} // namespace miku

#endif // #ifndef __SFWV_OBJ_H__

