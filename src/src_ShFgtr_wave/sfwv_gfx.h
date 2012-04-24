
#ifndef __SFWV_GFX_BOX_H__
#define __SFWV_GFX_BOX_H__

#include "sfwv_config.h"
#include "sfwv_util.h"
#include "sfwv_res.h"

namespace miku
{
	////////////////////////////////////////////////////////////////////////////
	// external

	extern avej_lite::IGfxDevice*  g_p_gfx_device;
	extern avej_lite::IGfxSurface* g_p_back_buffer;
	extern avej_lite::IGfxSurface* g_p_res_sprite;
	extern TRect                   g_sprite_rect[];

	////////////////////////////////////////////////////////////////////////////
	//

	struct TIndexColor
	{
		TIndexColor();
		explicit TIndexColor(unsigned char index);
		unsigned long GetARGB(void);

	private:
		unsigned char index;
		unsigned long argb;
	};

	namespace gfx
	{
		void Init(void);
		void DrawImage(int x, int y, TImage* image);
		void DrawSprite(int x, int y, TImage* p_buffer, bool flag);
		void DrawText(int x, int y, const tchar* s, unsigned long real_color, bool use_fading);
		void FillRect(unsigned long back_color, int x, int y, int width, int height);
		void BitBlt(int x_dest, int y_dest, avej_lite::IGfxSurface* p_surface, int x_sour, int y_sour, int w_sour, int h_sour);
		void BlendBlt(int x_dest, int y_dest, avej_lite::IGfxSurface* p_surface, int x_sour, int y_sour, int w_sour, int h_sour, float opacity);
	}

	namespace gfx_ix
	{
		void DrawText(int x, int y, const tchar* s, TIndexColor color);
		void DrawText_x2(int x, int y, const tchar* s, TIndexColor color);
		void FillRect(int x1, int y1, int x2, int y2, TIndexColor back_color);
		void DrawLineX(int x1, int x2, int y, TIndexColor color);
		void DrawLineY(int x, int y1, int y2, TIndexColor color);
	}

	////////////////////////////////////////////////////////////////////////////
	//

	namespace gui
	{
		class CVisible
		{
		public:
			virtual ~CVisible() {}
			virtual void Show(void) = 0;
		};

		class CMessageBox: public CVisible
		{
		public:
			CMessageBox(int x1, int y1, int x2, int y2, TIndexColor fore_color, TIndexColor back_color, TIndexColor hide_color, byte max_line);
			~CMessageBox();

			void Enable(unsigned int line);
			void Disable(unsigned int line);
			void SetMessage(unsigned int line, const tchar* s);
			int  SetSelectedLine(unsigned int line);

			virtual void Show(void);

		private:
			struct TImpl;
			TImpl* m_p_impl;
		};

		class CKeyGuide: public CVisible
		{
		public:
			CKeyGuide();
			CKeyGuide(long x, long y, unsigned long width, unsigned long height);
			~CKeyGuide();

			void SetLeftText(const tchar* sz_text);
			void SetRightText(const tchar* sz_text);

			virtual void Show(void);

		private:
			struct TImpl;
			TImpl* m_p_impl;
		};

		class CFloatMessage: public CVisible
		{
			THIS_CLASS_IS_A_SINGLETON(CFloatMessage);

		public:
			void Register(int x, int y, TIndexColor color, const tchar* sz_message);
			virtual void Show(void);
		};

		class CButtonGroup
		{
		public:
			static void Show(void);
		};

		class CButtonGroupFull
		{
		public:
			static void Show(void);
		};
	}

} // namespace miku
/*
//?? 나중에는 util로 가야 함
//?? 공용 window / event system
namespace megurine
{
	////////////////////////////////////////////////////////////////////////////
	// type definition

	struct TBounds
	{
		int x1, y1, x2, y2;
	};

	class CVisible
	{
	public:
		virtual ~CVisible() {}
		virtual void Show(void) = 0;
	};

	////////////////////////////////////////////////////////////////////////
	//

	class CRectangle: public CVisible
	{
	public:
		CRectangle(const TBounds& bounds)
			: m_bounds(bounds)
		{
		}
		virtual ~CRectangle()
		{
		}

	private:
		TBounds m_bounds;
	};
}
*/
#endif // #ifndef __SFWV_GFX_BOX_H__

