
#ifndef __AVEJ_SOUND_MIXER_H__
#define __AVEJ_SOUND_MIXER_H__

namespace avej
{
	namespace sound
	{
		class CMixer
		{
		public:
			CMixer();
			~CMixer();

			bool Load(int sound, const char* sz_file_name);
			bool Load(int sound, const void* p_buffer, int buffer_size);
			void Play(unsigned voice, unsigned sound, int left_vol, int right_vol);

		private:
			struct TImpl;
			TImpl* m_p_impl;

		};
	}
}

#endif // #ifndef __AVEJ_SOUND_MIXER_H__

