
////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_config.h"
#include "sfwv_util.h"
#include "sfwv_res.h"
#include "sfwv_gfx.h"

#ifdef _WIN32
#	pragma warning(disable: 4786)
#endif

#include <vector>
#include <string>
#include <algorithm>

using namespace avej_lite;

namespace miku
{

#if defined(USE_WCHAR)
	// " OK키: 게임 시작"
	static const tchar RES_STR_LEFT_TEXT[] = { 0x0020, 0x004F, 0x004B, 0xD0A4, 0x003A, 0x0020, 0xAC8C, 0xC784, 0x0020, 0xC2DC, 0xC791, 0x0000 };
	// "Esc키: 타이틀로 "
	static const tchar RES_STR_RIGHT_TEXT[] = { 0x0045, 0x0073, 0x0063, 0xD0A4, 0x003A, 0x0020, 0xD0C0, 0xC774, 0xD2C0, 0xB85C, 0x0020, 0x0000 };

#else
	static const tchar* RES_STR_LEFT_TEXT = " OK키: 게임 시작";
	static const tchar* RES_STR_RIGHT_TEXT = "Esc키: 타이틀로 ";
#endif

////////////////////////////////////////////////////////////////////////////////
// callback

#define START_MAKER  TCHAR("#START STORY")
#define END_MAKER    TCHAR("#END")

namespace menu_story
{
	const  int  H_KEY_GUIDE     = (16*2);
	const  int  PIXELS_PER_PAGE = 240*2;

	static bool s_auto_scroll = false;
	static int  s_scroll_time_stamp = 0;
	static int  s_scroll_time_stamp_delay = 2;
	static int  s_min_scroll_time_stamp = -PIXELS_PER_PAGE * s_scroll_time_stamp_delay;
	static int  s_max_scroll_time_stamp = 0;
	static std::vector< std::pair<unsigned long, STD_STRING> > s_string_list;

	static gui::CKeyGuide s_key_guide;

	static int  s_x = 0;
	static int  s_y = 0;
	
	static void s_PrintString(const std::pair<unsigned long, STD_STRING>& string_pair)
	{
		if ((s_y + GLYPH_H > 0) && (s_y < PIXELS_PER_PAGE))
		{
			unsigned long real_color    = string_pair.first;
			unsigned long shading_color = ((real_color >> 1) & 0xFF000000) | (real_color & 0x00FFFFFF);

			gfx::DrawText(s_x+1, s_y+0, string_pair.second.c_str(), shading_color, true);
			gfx::DrawText(s_x+0, s_y+1, string_pair.second.c_str(), shading_color, true);
			gfx::DrawText(s_x+0, s_y+0, string_pair.second.c_str(), real_color,    true);
		}

		s_y += GLYPH_H;
	}

	bool OnCreate(void)
	{
		util::CTextFileFromRes text_res((const tchar*)p_res_story, sizeof_p_res_story());

		if (text_res.IsValid())
		{
			const unsigned long COLOR_TABLE[4] =
			{
				0xFFFFFFFF, 0xFF80FFBF, 0xFFC080FF, 0xFFFF80BF
			};

			tchar s[256];

			while (text_res.GetString(s, 256))
			{
				if (STRNCMP(s, START_MAKER, NUMCHAR(START_MAKER)) == 0)
					break;
			}

			while (text_res.GetString(s, 256))
			{
				if (STRNCMP(s, END_MAKER, NUMCHAR(END_MAKER)) == 0)
					break;

				int len = STRLEN(s);
				while (len > 0 && s[len] < 27)
					s[len--] = 0;

				if (len > 0)
				{
					unsigned long color_index = s[len] - TCHAR('1');
					s[len--] = 0;

					color_index = clamp(color_index, 0UL, (unsigned long)(sizeof(COLOR_TABLE) / sizeof(COLOR_TABLE[0])));
					s_string_list.push_back(std::make_pair<unsigned long, STD_STRING>(COLOR_TABLE[color_index], s));
				}
				else
				{
					s_string_list.push_back(std::make_pair<unsigned long, STD_STRING>(0, TCHAR("")));
				}
			}
		}

		s_scroll_time_stamp      = s_min_scroll_time_stamp;

		s_max_scroll_time_stamp  = (s_string_list.size()+1) * GLYPH_H - PIXELS_PER_PAGE + H_KEY_GUIDE;
		s_max_scroll_time_stamp *= s_scroll_time_stamp_delay;
		s_max_scroll_time_stamp  = max(s_max_scroll_time_stamp, 0);

		s_auto_scroll = true;

		new (&s_key_guide) gui::CKeyGuide(0, 480-H_KEY_GUIDE, 640, H_KEY_GUIDE);
		s_key_guide.SetLeftText(RES_STR_LEFT_TEXT);
		s_key_guide.SetRightText(RES_STR_RIGHT_TEXT);

		return true;
	}

	bool OnDestory(void)
	{
		s_string_list.clear();

		return true;
	}

	bool OnProcess(void)
	{
		{
			CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
			input_device.UpdateInputState();

			if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_A))
			{
				g_ChangeState(STATE_GAME_PLAY);
				return false;
			}

			if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1) ||
				input_device.WasKeyPressed(avej_lite::INPUT_KEY_B))
			{
				g_ChangeState(STATE_TITLE);
				return false;
			}

			int saved_scroll_time_stamp = s_scroll_time_stamp;

			if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_UP))
			{
				if (s_scroll_time_stamp > 0)
				{
					s_scroll_time_stamp -= s_scroll_time_stamp_delay;
				}
			}
			if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_DOWN))
				s_scroll_time_stamp += s_scroll_time_stamp_delay;

			if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_L1))
			{
				if (s_scroll_time_stamp > 0)
				{
					s_scroll_time_stamp -= (PIXELS_PER_PAGE - GLYPH_H) * s_scroll_time_stamp_delay;
					s_scroll_time_stamp  = max(s_scroll_time_stamp, 0);
				}
			}
			if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_R1))
				s_scroll_time_stamp += (PIXELS_PER_PAGE - GLYPH_H) * s_scroll_time_stamp_delay;

			if (s_auto_scroll)
				s_auto_scroll = (saved_scroll_time_stamp == s_scroll_time_stamp);

			s_scroll_time_stamp = clamp(s_scroll_time_stamp, s_min_scroll_time_stamp, s_max_scroll_time_stamp);
		}

		g_p_gfx_device->BeginDraw();

		{
			s_x = 6;
			s_y = 4 - s_scroll_time_stamp / s_scroll_time_stamp_delay;

			std::for_each(s_string_list.begin(), s_string_list.end(), s_PrintString);

			if (s_auto_scroll)
				++s_scroll_time_stamp;

			s_key_guide.Show();
		}

		gui::CButtonGroup::Show();

		g_p_gfx_device->EndDraw();
		g_p_gfx_device->Flip();

		return true;
	}

	AppCallback callback =
	{
		OnCreate,
		OnDestory,
		OnProcess
	};
}

} // namespace miku

