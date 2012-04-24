
#ifndef __AVEJ_UTIL_H__
#define __AVEJ_UTIL_H__

#include <string.h>
#include "avej_base_type.h"

////////////////////////////////////////////////////////////////////////////////
// definition

#if 1
#	define CT_ASSERT(x, msg) typedef int __CT_ASSERT ## msg ## __ [(x) ? 1 : -1];
#else
#	define CT_ASSERT(x, msg)
#endif

////////////////////////////////////////////////////////////////////////////////
// System utility

namespace avej_lite
{
	namespace util
	{
		unsigned long GetTicks(void);
		int           Random(int range);
		void          Delay(unsigned long msec);

		bool DecodeSMgal(avej_lite::TPixelFormat pixel_format, const unsigned char* p_stream, unsigned int stream_size, avej_lite::TSurfaceDesc& out_surface_desc);
		bool DecodeSMgal(avej_lite::TPixelFormat pixel_format, const char* sz_file_name, avej_lite::TSurfaceDesc& out_surface_desc);
	}
}

////////////////////////////////////////////////////////////////////////////////
// string

namespace avej_lite
{
	namespace util
	{
		#define MAX_STRLEN 255

		class string
		{
		private:
			char m_string[MAX_STRLEN+1];

		public:
			string(void)
			{
				m_string[0] = 0;
			}
			string(const char* lpsz)
			{      
				strncpy(m_string, lpsz, MAX_STRLEN);
			}

			operator const char*() const
			{
				return m_string;
			};

			const string& operator=(const char* lpsz)
			{      
				strncpy(m_string, lpsz, MAX_STRLEN);
				return *this;
			}

			const string& operator+=(const char* lpsz)
			{      
				strncat(m_string, lpsz, MAX_STRLEN);
				return *this;
			}

			void copyToFront(const string& lpsz)
			{      
				char m_temp[MAX_STRLEN+1];
				strncpy(m_temp, m_string, MAX_STRLEN);
				strncpy(m_string, lpsz.m_string, MAX_STRLEN);
				strncat(m_string, m_temp, MAX_STRLEN);
			}

			bool isEmpty(void) const
			{
				return (m_string[0] == 0);
			}
		};
	}
}

////////////////////////////////////////////////////////////////////////////////
// stream

namespace avej_lite
{
	namespace util
	{
		enum TOrigin
		{
			ORIGN_SET     = 0,
			ORIGN_BEGIN   = ORIGN_SET,
			ORIGN_CUR     = 1,
			ORIGN_CURRENT = ORIGN_CUR,
			ORIGN_END     = 2
		};

		class CReadStream
		{ 
		public:
			CReadStream(void);
			virtual ~CReadStream(void);

			bool          IsValid(void) const;
			virtual long  Read(void* p_buffer, long count) const = 0;
			virtual long  Seek(long offset, TOrigin origin) const = 0;
			virtual long  GetSize(void) const = 0;
			virtual void* GetPointer(void) const = 0;
			virtual bool  IsValidPos(void) const = 0;

		protected:
			bool m_is_available;
		};

		class CFileReadStream: public CReadStream
		{
		public:
			CFileReadStream(const char* sz_file_name);
			~CFileReadStream(void);

			long  Read(void* p_buffer, long count) const;
			long  Seek(long offset, TOrigin origin) const;
			long  GetSize(void) const;
			void* GetPointer(void) const;
			bool  IsValidPos(void) const;

		private:
			struct TImpl;
			TImpl* m_p_impl;
		};

		class CMemoryReadStream: public CReadStream
		{
		public:
			CMemoryReadStream(void* p_memory, long size);
			~CMemoryReadStream(void);

			long  Read(void* p_buffer, long count) const;
			long  Seek(long offset, TOrigin origin) const;
			long  GetSize(void) const;
			void* GetPointer(void) const;
			bool  IsValidPos(void) const;

		private:
			struct TImpl;
			TImpl* m_p_impl;
		};
	}
}

#endif // #ifndef __AVEJ_UTIL_H__
