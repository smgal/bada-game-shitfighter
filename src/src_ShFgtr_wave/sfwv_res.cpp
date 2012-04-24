
#include "sfwv_sys_desc.h"
#include "sfwv_res.h"

namespace miku
{

TTile* tile_data[TILENAME_MAX];

TTile::TTile()
:	m_x_len(0), m_y_len(0), m_size(0), m_p_sprite(0)
{
}

TTile::TTile(int x1, int y1, int x2, int y2)
:	m_x_len(0), m_y_len(0), m_size(0), m_p_sprite(0)
{
}

TTile::TTile(TTileName number)
{
    m_x_len    = tile_data[number]->m_x_len;
    m_y_len    = tile_data[number]->m_y_len;
    m_size     = tile_data[number]->m_size;
    m_p_sprite = tile_data[number]->m_p_sprite;
}

TTile::~TTile()
{
}

////////////////////////////////////////////////////////////////////////////////
//

namespace
{

#if defined(USE_WCHAR)
	// 초 극상 / 궁극 변태 / 대변 파이터
	static const tchar RES_STR_KOR_TITLE[] = { 0xCD08, 0x0020, 0xADF9, 0xC0C1, 0x0020, 0x002F, 0x0020, 0xAD81, 0xADF9, 0x0020, 0xBCC0, 0xD0DC, 0x0020, 0x002F, 0x0020, 0xB300, 0xBCC0, 0x0020, 0xD30C, 0xC774, 0xD130, 0x0000 };
	// 이 게임은 뭐지 ???
	static const tchar RES_STR_KOR_MENU1[] = { 0xC774, 0x0020, 0xAC8C, 0xC784, 0xC740, 0x0020, 0xBB50, 0xC9C0, 0x0020, 0x003F, 0x003F, 0x003F, 0x0000 };
	// 스토리를 알고 싶군
	static const tchar RES_STR_KOR_MENU2[] = { 0xC2A4, 0xD1A0, 0xB9AC, 0xB97C, 0x0020, 0xC54C, 0xACE0, 0x0020, 0xC2F6, 0xAD70, 0x0000 };
	// 게임을 시작해 볼까
	static const tchar RES_STR_KOR_MENU3[] = { 0xAC8C, 0xC784, 0xC744, 0x0020, 0xC2DC, 0xC791, 0xD574, 0x0020, 0xBCFC, 0xAE4C, 0x0000 };
	// 옵션도 조정해 보자
	static const tchar RES_STR_KOR_MENU4[] = { 0xC635, 0xC158, 0xB3C4, 0x0020, 0xC870, 0xC815, 0xD574, 0x0020, 0xBCF4, 0xC790, 0x0000 };

#else
	static const tchar* RES_STR_KOR_TITLE = "초 극상 / 궁극 변태 / 대변 파이터";
	static const tchar* RES_STR_KOR_MENU1 = "이 게임은 뭐지 ???";
	static const tchar* RES_STR_KOR_MENU2 = "스토리를 알고 싶군";
	static const tchar* RES_STR_KOR_MENU3 = "게임을 시작해 볼까";
	static const tchar* RES_STR_KOR_MENU4 = "옵션도 조정해 보자";

#endif

}

static const tchar* s_current_res_string[RESSTRING_MAX] = { TCHAR("@"), };

static bool s_UpdateResString(void)
{
	switch (GetSysDesc().language)
	{
	case LANGUAGE_KOREAN:
		s_current_res_string[RESSTRING_TITLE      ] = RES_STR_KOR_TITLE;
		s_current_res_string[RESSTRING_TITLE_MENU1] = RES_STR_KOR_MENU1;
		s_current_res_string[RESSTRING_TITLE_MENU2] = RES_STR_KOR_MENU2;
		s_current_res_string[RESSTRING_TITLE_MENU3] = RES_STR_KOR_MENU3;
		s_current_res_string[RESSTRING_TITLE_MENU4] = RES_STR_KOR_MENU4;
		break;
	case LANGUAGE_ENGLISH:
		s_current_res_string[RESSTRING_TITLE      ] = TCHAR("Superb / Ultra Pervert / Shit Fighter");
		s_current_res_string[RESSTRING_TITLE_MENU1] = TCHAR("What is this???");
		s_current_res_string[RESSTRING_TITLE_MENU2] = TCHAR("I wanna know the story of this game");
		s_current_res_string[RESSTRING_TITLE_MENU3] = TCHAR("Let's go into the game");
		s_current_res_string[RESSTRING_TITLE_MENU4] = TCHAR("Is there options");
		break;
	default:
		return false;
	}

	return true;
}

void UpdateResString(void)
{
	s_UpdateResString();
}

const tchar* GetResString(TResString res_string)
{
	if (s_current_res_string[0][0] == TCHAR('@'))
	{
		if (!s_UpdateResString())
			return TCHAR("");
	}

	return s_current_res_string[res_string];
}

} // namespace miku

