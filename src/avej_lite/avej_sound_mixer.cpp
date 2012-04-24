
#include "avej_sound_mixer.h"

struct avej::sound::CMixer::TImpl
{
	int dummy;
};

avej::sound::CMixer::CMixer()
: m_p_impl(0)
{
}

avej::sound::CMixer::~CMixer()
{
	delete m_p_impl;
}

bool avej::sound::CMixer::Load(int sound, const char* sz_file_name)
{
	return false;
}

bool avej::sound::CMixer::Load(int sound, const void* p_buffer, int buffer_size)
{
	return false;
}

void avej::sound::CMixer::Play(unsigned voice, unsigned sound, int left_vol, int right_vol)
{
}
