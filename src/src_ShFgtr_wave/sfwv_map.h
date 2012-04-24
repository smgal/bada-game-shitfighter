
#ifndef __SFWV_MAP_H__
#define __SFWV_MAP_H__

#include "sfwv_config.h"

namespace miku
{
	class CStageMap
	{
		THIS_CLASS_IS_A_SINGLETON(CStageMap);

	public:
		unsigned char GetTile(int a_x, int a_y);
		void          Move(int x, int y);
		bool          NotOnPlatform(int x, int y);
		int           GetGravity(int x, int y);
		void          SetCurrentMap(int stage);
		int           GetXOffset(void);
		int           GetYOffset(void);
		bool          IsEndOfMap(void);
	};

} // namespace miku

#endif // #ifndef __SFWV_MAP_H__

