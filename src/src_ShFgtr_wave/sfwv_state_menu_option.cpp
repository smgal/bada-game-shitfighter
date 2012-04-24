
////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_config.h"
#include "sfwv_util.h"
#include "sfwv_sys_desc.h"
#include "sfwv_gfx.h"
#include "sfwv_obj.h"

using namespace avej_lite;

#define for if (0); else for

namespace miku
{

////////////////////////////////////////////////////////////////////////////////
// resource

static const int    G_MAX_MENU = 4;
static const tchar* G_MENU_NAME[G_MAX_MENU] =
{
   TCHAR("1P: SMgal 2P: x"),
   TCHAR("1P: NeTo  2P: x"),
   TCHAR("1P: SMgal 2P: Neto"),
   TCHAR("1P: Neto  2P: SMgal")
};

#if defined(USE_WCHAR)
	static const tchar szText000[] = { 0xC774, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0xB3D9, 0x003A, 0x0020, 0x0034, 0xAC1C, 0xC758, 0x0020, 0xBC29, 0xD5A5, 0x0020, 0xBC84, 0xD2BC, 0x0000 };
	static const tchar szText001[] = { 0xC77C, 0xBC18, 0x0020, 0xACF5, 0xACA9, 0x003A, 0x0020, 0x0061, 0x0075, 0x0074, 0x006F, 0x0000 };
	static const tchar szText002[] = { 0xD2B9, 0xC218, 0x0020, 0xACF5, 0xACA9, 0x003A, 0x0020, 0x0021, 0x0021, 0x0021, 0xD0A4, 0x0000 };
	static const tchar szText003[] = { 0xAC8C, 0xC784, 0x0020, 0xC885, 0xB8CC, 0x003A, 0x0020, 0x0045, 0x0073, 0x0063, 0xD0A4, 0x0000 };
	
	static const tchar szText010[] = { 0x0031, 0x0050, 0x0020, 0xC774, 0xB3D9, 0x003A, 0x0020, 0x0034, 0xAC1C, 0xC758, 0x0020, 0xBC29, 0xD5A5, 0x0020, 0xBC84, 0xD2BC, 0x0020, 0x0020, 0x0020, 0x0032, 0x0050, 0x0020, 0xC774, 0xB3D9, 0x003A, 0x0020, 0xC88C, 0xB3D9, 0x0000 };
	static const tchar szText011[] = { 0x0031, 0x0050, 0x0020, 0xC77C, 0xBC18, 0x003A, 0x0020, 0x0061, 0x0075, 0x0074, 0x006F, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0032, 0x0050, 0x0020, 0xC77C, 0xBC18, 0x003A, 0x0020, 0x0061, 0x0075, 0x0074, 0x006F, 0x0000 };
	static const tchar szText012[] = { 0x0031, 0x0050, 0x0020, 0xD2B9, 0xC218, 0x003A, 0x0020, 0xC5C6, 0xC74C, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0032, 0x0050, 0x0020, 0xD2B9, 0xC218, 0x003A, 0x0020, 0x0021, 0x0021, 0x0021, 0xD0A4, 0x0000 };
	static const tchar szText013[] = { 0xAC8C, 0xC784, 0x0020, 0xC885, 0xB8CC, 0x003A, 0x0020, 0x0045, 0x0073, 0x0063, 0xD0A4, 0x0000 };

#else
	static const tchar* szText000 = "이     동: 4개의 방향 버튼";
	static const tchar* szText001 = "일반 공격: auto";
	static const tchar* szText002 = "특수 공격: !!!키";
	static const tchar* szText003 = "게임 종료: Esc키";
	
	static const tchar* szText010 = "1P 이동: 4개의 방향 버튼   2P 이동: 좌동";
	static const tchar* szText011 = "1P 일반: auto              2P 일반: auto";
	static const tchar* szText012 = "1P 특수: 없음              2P 특수: !!!키";
	static const tchar* szText013 = "게임 종료: Esc키";

#endif

static const tchar* RES_STR_MENU1[] =
{
	szText000,
	szText001,
	szText002,
	szText003,
};

static const tchar* RES_STR_MENU2[] =
{
	szText010,
	szText011,
	szText012,
	szText013,
};

////////////////////////////////////////////////////////////////////////////////
// callback

namespace menu_option
{
	static int s_selected_menu;

	bool OnCreate(void)
	{
		s_selected_menu = ord_(GetSysDesc().GetPlayerMode());

		return true;
	}

	bool OnDestory(void)
	{
		SetSysDesc().SetPlayerMode(TPlayerMode(s_selected_menu));

		return true;
	}

	bool OnProcess(void)
	{
		{
			CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
			input_device.UpdateInputState();

			if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1) ||
				input_device.WasKeyPressed(avej_lite::INPUT_KEY_A) ||
				input_device.WasKeyPressed(avej_lite::INPUT_KEY_B))
			{
				g_ChangeState(STATE_TITLE);
				return false;
			}

			if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_UP))
			{
				if (--s_selected_menu < 0)
					s_selected_menu = G_MAX_MENU - 1;
			}
			if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_DOWN))
			{
				++s_selected_menu;
				s_selected_menu %= G_MAX_MENU;
			}
		}


		{
			static int s_count_step = 0;
			static int s_count = 0;

			if (++s_count_step == 5)
			{
				s_count_step = 0;
				s_count = (s_count+1) % 2;
			}
	
			g_p_gfx_device->BeginDraw();

			const TRect* p_rect[3] = { 0, 0, 0 };

			switch (s_selected_menu)
			{
			case 0: // player1
				p_rect[0] = &g_sprite_rect[TILENAME_SMGAL1+s_count];
				break;
			case 1: // player2
				p_rect[0] = &g_sprite_rect[TILENAME_NETO1+s_count];
				break;
			case 2: // dual_mode1
				p_rect[1] = &g_sprite_rect[TILENAME_SMGAL1+s_count];
				p_rect[2] = &g_sprite_rect[TILENAME_NETO1+s_count];
				break;
			case 3: // dual_mode2
				p_rect[1] = &g_sprite_rect[TILENAME_NETO1+s_count];
				p_rect[2] = &g_sprite_rect[TILENAME_SMGAL1+s_count];
				break;
			}

			const int x_center = 320;
			const int y_center = 70;
			const int x_gap    = 120;
			const int y_gap    = 0;

			const int POS_TABLE[3][2] =
			{
				{ x_center,         y_center        },
				{ x_center - x_gap, y_center - y_gap},
				{ x_center + x_gap, y_center + y_gap},
			};

			for (int i = 0; i < 3; i++)
			{
				if (p_rect[i])
				{
					int x_revised = POS_TABLE[i][0] - p_rect[i]->w / 2;
					gfx::BitBlt(x_revised, POS_TABLE[i][1], g_p_res_sprite, p_rect[i]->x, p_rect[i]->y, p_rect[i]->w, p_rect[i]->h);
				}
			}

			for (int i = 0; i < G_MAX_MENU; i++)
			{
				int x = 210;
				int color_index = (i == ord_(s_selected_menu)) ? 10 : 2;
				gfx_ix::DrawText(x+2,301+i*24,G_MENU_NAME[i],TIndexColor(color_index-1));
				gfx_ix::DrawText(x+0,300+i*24,G_MENU_NAME[i],TIndexColor(color_index));
			}

			{
				int x = 210;
				int y = 170;

				switch (s_selected_menu)
				{
				case 0: // player1
				case 1: // player2
					x = 210;
					gfx_ix::DrawText(x, y+GLYPH_H*0, RES_STR_MENU1[0], TIndexColor(15));
					gfx_ix::DrawText(x, y+GLYPH_H*1, RES_STR_MENU1[1], TIndexColor(15));
					gfx_ix::DrawText(x, y+GLYPH_H*2, RES_STR_MENU1[2], TIndexColor(15));
					gfx_ix::DrawText(x, y+GLYPH_H*3, RES_STR_MENU1[3], TIndexColor(15));
					break;
				case 2: // dual_mode1
				case 3: // dual_mode2
					x = 40;
					gfx_ix::DrawText(x, y+GLYPH_H*0, RES_STR_MENU2[0], TIndexColor(15));
					gfx_ix::DrawText(x, y+GLYPH_H*1, RES_STR_MENU2[1], TIndexColor(15));
					gfx_ix::DrawText(x, y+GLYPH_H*2, RES_STR_MENU2[2], TIndexColor(15));
					gfx_ix::DrawText(x, y+GLYPH_H*3, RES_STR_MENU2[3], TIndexColor(15));
					break;
				}
			}

			gui::CButtonGroup::Show();

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

