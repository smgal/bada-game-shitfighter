
#include "../avej_config.h"
#include "../avej_util.h"

////////////////////////////////////////////////////////////////////////////////
// avej_lite::util::GetTicks

#if (TARGET_DEVICE == TARGET_WIN32)

// 삭제

#elif (TARGET_DEVICE == TARGET_GP2XWIZ)

// 삭제

#elif (TARGET_DEVICE == TARGET_BADA)

#include <FSystem.h>

namespace avej_lite
{
	namespace util
	{
		unsigned long GetTicks(void)
		{
			static long long s_startTick = 0;

			if (s_startTick == 0)
				Osp::System::SystemTime::GetTicks(s_startTick);

			static long long currentTick;
			Osp::System::SystemTime::GetTicks(currentTick);

			double diff = currentTick - s_startTick;

			return (unsigned long)diff;
		}
	}
}

#else

#error ...

#endif // #if defined(_WIN32)

////////////////////////////////////////////////////////////////////////////////
// avej_lite::util::Random

namespace avej_lite
{
	namespace util
	{
		static unsigned long rand_seed = 0L;

		int Random(int range)
		{
			if (range <= 0)
				return 0;

			rand_seed = rand_seed * 214013L + 2531011L;
			return ((rand_seed >> 16) & 0x7fff) % range;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// avej_lite::util::CReadStream

namespace avej_lite
{
	namespace util
	{
		////////////////////////////////////////////////////////////////////////
		// CReadStream

		CReadStream::CReadStream(void)
			: m_is_available(false)
		{
		}

		CReadStream::~CReadStream(void)
		{
		}

		bool CReadStream::IsValid(void) const
		{
			return m_is_available;
		}
	}
}

#if ( ( TARGET_DEVICE == TARGET_WIN32 ) || ( TARGET_DEVICE == TARGET_GP2XWIZ ) )

// 삭제

#elif ( TARGET_DEVICE == TARGET_BADA )

#include <FIo.h>

namespace avej_lite
{
	namespace util
	{
		////////////////////////////////////////////////////////////////////////
		// CFileReadStream

		struct CFileReadStream::TImpl
		{
			Osp::Io::File file;
			int           file_length;
		};

		CFileReadStream::CFileReadStream(const char* sz_file_name)
			: m_p_impl(new TImpl)
		{
			Osp::Base::String file_name(sz_file_name);

			file_name = "/Res/" + file_name;

			result r = m_p_impl->file.Construct(file_name, L"r");

			if (r != E_SUCCESS)
				return;

			m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_END, 0);
			m_p_impl->file_length = m_p_impl->file.Tell();
			m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_BEGIN, 0);

			this->m_is_available = true;
		}

		CFileReadStream::~CFileReadStream(void)
		{
			delete m_p_impl;
		}

		long  CFileReadStream::Read(void* p_buffer, long count) const
		{
			if (!this->m_is_available)
				return 0;

			m_p_impl->file.Read(p_buffer, count);
		}

		long  CFileReadStream::Seek(long offset, TOrigin origin) const
		{
			if (!this->m_is_available)
				return -1;

			switch (origin)
			{
			case ORIGN_SET:
				m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_BEGIN, offset);
				break;
			case ORIGN_CUR:
				m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_CURRENT, offset);
				break;
			case ORIGN_END:
				m_p_impl->file.Seek(Osp::Io::FILESEEKPOSITION_END, offset);
				break;
			default:
				return -1;
			}

			return m_p_impl->file.Tell();
		}

		long  CFileReadStream::GetSize(void) const
		{
			if (!this->m_is_available)
				return -1;

			return m_p_impl->file_length;
		}

		void* CFileReadStream::GetPointer(void) const
		{
			return 0;
		}

		bool  CFileReadStream::IsValidPos(void) const
		{
			if (!this->m_is_available)
				return false;

			int read_pos = m_p_impl->file.Tell();

			return (read_pos >= 0) && (read_pos < m_p_impl->file_length);
		}
	}
}

#else

#error ...

#endif

////////////////////////////////////////////////////////////////////////////////
// CMemoryReadStream

namespace avej_lite
{
	namespace util
	{
		struct CMemoryReadStream::TImpl
		{
			void* p_memory;
			long  size;
			long  position;
		};

		CMemoryReadStream::CMemoryReadStream(void* p_memory, long size)
			: m_p_impl(new TImpl)
		{
			m_p_impl->p_memory = p_memory;
			m_p_impl->size     = size;
			m_p_impl->position = 0;

			m_is_available = (m_p_impl->p_memory != 0);
		}

		CMemoryReadStream::~CMemoryReadStream()
		{
			delete m_p_impl;
		}

		long CMemoryReadStream::Read(void* p_buffer, long count) const
		{
			if (!this->m_is_available)
				return -1;

			long result;

			if ((m_p_impl->position >= 0) && (count >= 0 ))
			{
				result = m_p_impl->size - m_p_impl->position;
				if (result > 0)
				{
					if (result > count)
						result = count;

					memcpy(p_buffer, (char*)m_p_impl->p_memory + m_p_impl->position, result);

					m_p_impl->position += result;

					return result;
				}
			}

			return 0;
		}

		long CMemoryReadStream::Seek(long offset, TOrigin origin) const
		{
			if (!this->m_is_available)
				return -1;

			switch (origin)
			{
			case ORIGN_SET:
				m_p_impl->position = offset;
				break;
			case ORIGN_CUR:
				m_p_impl->position += offset;
				break;
			case ORIGN_END:
				m_p_impl->position = m_p_impl->size + offset;
				break;
			default:
				return -1;
			}

			// 특별한 범위 체크 안함
			return m_p_impl->position;
		}

		long CMemoryReadStream::GetSize(void) const
		{
			if (!this->m_is_available)
				return -1;

			return m_p_impl->size;
		}

		void *CMemoryReadStream::GetPointer(void) const
		{
			if (!this->m_is_available)
				return 0;

			return m_p_impl->p_memory;
		}

		bool CMemoryReadStream::IsValidPos(void) const
		{
			if (!this->m_is_available)
				return false;

			return ((m_p_impl->position >= 0) && (m_p_impl->position < m_p_impl->size));
		}
	}
}
