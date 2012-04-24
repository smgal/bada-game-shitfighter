
#ifndef __SFWV_SYS_DESC_H__
#define __SFWV_SYS_DESC_H__

#include "sfwv_config.h"

namespace miku
{
	enum TPlayerMode
	{
		PLAYERMODE_PLAYER1_ONLY,
		PLAYERMODE_PLAYER2_ONLY,
		PLAYERMODE_DOUBLE_MODE1,
		PLAYERMODE_DOUBLE_MODE2
	};

	enum TLanguage
	{
		LANGUAGE_KOREAN,
		LANGUAGE_ENGLISH
	};

	struct TSysDesc
	{
		TLanguage language;
		bool      use_sound;

		struct TStage
		{
			struct TMesaage
			{
				int remained_ticks;

				TMesaage();

				void         Set(const tchar* sz_message);
				const tchar* Get(void) const;

			private:
				tchar stage_message[32];

			} message;

			unsigned int current_stage;
			bool         has_cleared;

			TStage()
				: current_stage(0), has_cleared(false)
			{
			}
		} stage;

		TPlayerMode GetPlayerMode(void) const;
		void        SetPlayerMode(TPlayerMode player_mode);

		bool        IsAutoShot(void) const;

	private:
		THIS_CLASS_IS_A_SINGLETON(TSysDesc);
	};

	const TSysDesc& GetSysDesc(void);
		  TSysDesc& SetSysDesc(void);

} // namespace miku

#endif // #ifndef __SFWV_SYS_DESC_H__

