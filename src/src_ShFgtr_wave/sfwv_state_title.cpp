
////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_config.h"
#include "sfwv_util.h"
#include "sfwv_gfx.h"

using namespace avej_lite;

namespace miku
{

////////////////////////////////////////////////////////////////////////////////
// resource

static const tchar* G_TITLE_NAME = TCHAR("");

static const int    G_MAX_MENU = 4;
static const tchar* G_MENU_NAME[G_MAX_MENU] = { TCHAR(""), TCHAR(""), TCHAR(""), TCHAR("") };

////////////////////////////////////////////////////////////////////////////////
// callback

static bool s_InRect(int x, int y, int x1, int y1, int x2, int y2)
{
	return (x >= x1 && x < x2 && y >= y1 && y < y2);
}

namespace title
{
	int s_selected_menu = 0;
	avej::sound::CMixer* p_mixer;

	bool OnCreate(void)
	{
		G_TITLE_NAME   = GetResString(RESSTRING_TITLE);
		G_MENU_NAME[0] = GetResString(RESSTRING_TITLE_MENU1);
		G_MENU_NAME[1] = GetResString(RESSTRING_TITLE_MENU2);
		G_MENU_NAME[2] = GetResString(RESSTRING_TITLE_MENU3);
		G_MENU_NAME[3] = GetResString(RESSTRING_TITLE_MENU4);

		avej::sound::CMixer& mixer = avej_lite::singleton<avej::sound::CMixer>::get();
		mixer.Load(1, "s_bang.wav");

		s_selected_menu = 0;
		return true;
	}
	bool OnDestory(void)
	{
		avej_lite::singleton<avej::sound::CMixer>::release();
		return true;
	}

	bool OnProcess(void)
	{
		CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
		input_device.UpdateInputState();

		if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_C) && input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_SYS3))
		{
			g_ChangeState(STATE_ENDING_HAPPY);
			return false;
		}
		if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_C) && input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_SYS4))
		{
			g_ChangeState(STATE_ENDING_BAD);
			return false;
		}

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1))
		{
			g_ChangeState(STATE_EXIT);
			return false;
		}
		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_A))
		{
			switch (s_selected_menu)
			{
			case 0:
				g_ChangeState(STATE_MENU_ABOUT);
				break;
			case 1:
				g_ChangeState(STATE_MENU_STORY);
				break;
			case 2:
				g_ChangeState(STATE_GAME_PLAY);
				break;
			case 3:
				g_ChangeState(STATE_MENU_OPTION);
				break;
			default:
				SF_ASSERT(false);
			}
			return false;
		}

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_UP))
		{
			avej::sound::CMixer& mixer = avej_lite::singleton<avej::sound::CMixer>::get();
			mixer.Play(1, 1, 255, 100);

			s_selected_menu = (s_selected_menu > 0) ? (s_selected_menu-1) : (G_MAX_MENU-1);
		}
		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_DOWN))
		{
			avej::sound::CMixer& mixer = avej_lite::singleton<avej::sound::CMixer>::get();
			mixer.Play(1, 1, 100, 255);

			s_selected_menu = (s_selected_menu < (G_MAX_MENU-1)) ? (s_selected_menu+1) : 0;
		}

		{
			TMultiTouchInfoArray multi_touch_info_array;

			if (input_device.GetMultiTouchInfo(multi_touch_info_array))
			{
				TMultiTouchInfo& ti = multi_touch_info_array[0];

				if (s_InRect(ti.x, ti.y, 100, 100, 200, 200))
				{
					g_ChangeState(STATE_EXIT);
					return false;
				}
			}
		}

		{
			IGfxDevice::TDeviceDesc screenDesc;
			g_p_gfx_device->GetDesc(screenDesc);

			g_p_gfx_device->BeginDraw();

			{
				const TRect& rect = g_sprite_rect[TILENAME_TILE_MAIN_TITLE];

				int x = 64;
				int y = 80;
				gfx::BitBlt(x, y, g_p_res_sprite, rect.x, rect.y, rect.w, rect.h);
				gfx::FillRect(0xFF000000, x, y, 72*2, 55*2);
			}
			{
				const TRect rect = g_sprite_rect[TILENAME_SMGAL_AND_NETO];
				gfx::BitBlt(234, 430, g_p_res_sprite, rect.x, rect.y, rect.w, rect.h);
			}
			{
				const TRect rect = g_sprite_rect[TILENAME_TILE_SHIT_FLY];

				for (int i = 0; i < 3; i++)
				{
					int x = 450 + random(61)-30;
					int y = 230 + random(11)-5;
					gfx::BitBlt(x, y, g_p_res_sprite, rect.x, rect.y, rect.w, rect.h);
				}
			}

			{
				int title_table[6][3] =
				{
					{13,179,79}, {13,179,81}, {13,181,79},
					{13,181,81}, { 7,182,82}, {15,180,80}
				};
				for (int i = 0; i < 6; i++)
					gfx_ix::DrawText(title_table[i][1]-60, title_table[i][2]-30, G_TITLE_NAME, TIndexColor(title_table[i][0]));
			}

			for (int i = 0; i < G_MAX_MENU; i++)
			{
				int x = 210;
				int color_index = (i == s_selected_menu) ? 10 : 2;
				gfx_ix::DrawText(x+2,301+i*24,G_MENU_NAME[i],TIndexColor(color_index-1));
				gfx_ix::DrawText(x+0,300+i*24,G_MENU_NAME[i],TIndexColor(color_index));
			}

			gui::CButtonGroup::Show();

			// flip the screen
			g_p_gfx_device->EndDraw();
			g_p_gfx_device->Flip();
		}

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

