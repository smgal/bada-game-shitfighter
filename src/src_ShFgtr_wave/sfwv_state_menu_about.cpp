
////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_config.h"
#include "sfwv_util.h"
#include "sfwv_res.h"
#include "sfwv_gfx.h"

using namespace avej_lite;

namespace miku
{

////////////////////////////////////////////////////////////////////////////////
// callback

#define START_MAKER  TCHAR("#START ABOUT")
#define END_MAKER    TCHAR("#END")

namespace menu_about
{
	static gui::CMessageBox*       p_box_data = 0;
	static util::CTextFileFromRes* p_text_res = 0;
	static bool is_not_ready = true;
	static bool is_end_of_file = false;

	bool OnCreate(void)
	{
		p_text_res = new util::CTextFileFromRes((const tchar*)p_res_story, sizeof_p_res_story());

		tchar s[256];

		while (p_text_res->GetString(s, 256))
		{
			if (STRNCMP(s, START_MAKER, NUMCHAR(START_MAKER)) == 0)
				break;
		}

		if (!p_text_res->IsValid())
		{
			delete p_text_res;
			p_text_res = 0;

			return false;
		}

		int box_x      = 5;
		int box_y      = 10;
		int box_width  = 47-5;
		int box_height = 12-8;

		p_box_data = new gui::CMessageBox(box_x, box_y, box_x + box_width, box_y + box_height, TIndexColor(15), TIndexColor(1), TIndexColor(7), box_height-1);

		is_not_ready = true;
		is_end_of_file = false;

		return true;
	}

	bool OnDestory(void)
	{
		g_ChangeState(STATE_TITLE);

		delete p_box_data;
		p_box_data = 0;

		delete p_text_res;
		p_text_res = 0;

		return true;
	}

	bool OnProcess(void)
	{
		if (is_not_ready)
		{
			tchar s[256];

			int i = 0;
			do
			{
				if (!p_text_res->GetString(s, 256))
				{
					is_end_of_file = true;
					break;
				}
				if (STRNCMP(s, END_MAKER, NUMCHAR(END_MAKER)) == 0)
				{
					is_end_of_file = true;
					break;
				}

				p_box_data->SetMessage(i++, s);

			} while (s[0] && s[0] != 0x0A && s[0] != 0x0D);

			is_not_ready = false;
		}

		g_p_gfx_device->BeginDraw(false);
		{
			p_box_data->Show();
			gui::CButtonGroup::Show();
		}
		g_p_gfx_device->EndDraw();
		g_p_gfx_device->Flip();

		CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
		input_device.UpdateInputState();

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_A))
		{
			is_not_ready = true;
		}

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1))
		{
			return false;
		}

		return !(is_end_of_file && is_not_ready);
	}

	AppCallback callback =
	{
		OnCreate,
		OnDestory,
		OnProcess
	};
}

} // namespace miku

