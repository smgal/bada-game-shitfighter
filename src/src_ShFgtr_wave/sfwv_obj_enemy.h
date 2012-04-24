
#ifndef __SFWV_OBJ_ENEMY_H__
#define __SFWV_OBJ_ENEMY_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_res.h"
#include "sfwv_obj.h"

namespace miku
{
	////////////////////////////////////////////////////////////////////////////
	// constant

	#define MAX_REGISTERED_FRAME  2

	////////////////////////////////////////////////////////////////////////////
	// external

	extern int CreateEnemy(TTileName tile_number, int max_power, int x, int y, bool is_flyable);

	////////////////////////////////////////////////////////////////////////////
	// class TEnemy

	struct TEnemy: public TPersistent
	{
		REGISTER_TYPE_ID(TEnemy);

		byte         m_max_frame, m_current_frame, m_delay_frame, m_frame_count;
		int          m_auto_shoot;
		TImage*      m_temp_sprite[MAX_REGISTERED_FRAME+1];

		TEnemy(TTileName number, int hit_point, bool is_flyable, byte max_frame, byte delay_frame);
		virtual ~TEnemy();

		virtual void Display(void);
		virtual bool ProcessByAI(void);
		virtual void Shoot(int vx, int vy);

	protected:
		void         m_MoveRandom(void);
		virtual bool _ProcessByAI(void) = 0;
		virtual void _Shoot(int vx, int vy) { };
	};

	////////////////////////////////////////////////////////////////////////////
	// class TEnemy_Xxxx

	struct TEnemy_ZacoBigShit: public TEnemy
	{
		TEnemy_ZacoBigShit(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_ZACO_BIG_SHIT, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_ZacoTeeth: public TEnemy
	{
		TEnemy_ZacoTeeth(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_ZACO_TEETH, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_ZacoRoach: public TEnemy
	{
		TEnemy_ZacoRoach(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_ZACO_ROACH, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_ZacoFly: public TEnemy
	{
		TEnemy_ZacoFly(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_ZACO_FLY, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_BossGasMan: public TEnemy
	{
		TEnemy_BossGasMan(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_BOSS_GAS_MAN, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_BossNoseWaxMan: public TEnemy
	{
		TEnemy_BossNoseWaxMan(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_BOSS_NOSE_WAX_MAN, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_BossOvereatMan: public TEnemy
	{
		TEnemy_BossOvereatMan(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_BOSS_OVEREAT_MAN, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_BossHairWaxMan: public TEnemy
	{
		TEnemy_BossHairWaxMan(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_BOSS_HAIR_WAX_MAN2, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

	struct TEnemy_BossShitFighter: public TEnemy
	{
		TEnemy_BossShitFighter(int hit_point, bool is_flyable)
			: TEnemy(TILENAME_BOSS_SHIT_FIGHTER, hit_point, is_flyable, 1, 0) { }
	protected:
		virtual bool _ProcessByAI(void);
		virtual void _Shoot(int vx, int vy);
	};

} // namespace miku

#endif // #ifndef __SFWV_OBJ_ENEMY_H__

