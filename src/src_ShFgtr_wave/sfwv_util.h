
#ifndef __SFWV_UTIL_H__
#define __SFWV_UTIL_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include "sfwv_config.h"

////////////////////////////////////////////////////////////////////////////////
// definition

#if 1
	#include <cassert>
	#include <cmath>
	#define SF_ASSERT(cond) assert(cond)
	#define SF_SQRT(a)      sqrt(a)
	#define SF_SIN(a)       sin(a)
#else
	#define SF_ASSERT(cond)
	#define SF_SQRT(a)      0
	#define SF_SIN(a)       0
#endif

#define assigned_(cond) (cond)

#define round_(val)  int(val + 0.5)

#define STRLEN     util::tstrlen
#define STRCPY     util::tstrcpy
#define STRCAT     util::tstrcat
#define STRNCPY    util::tstrncpy
#define STRNCAT    util::tstrncat
#define STRNCMP    util::tstrncmp
#define STD_STRING util::tstring
#define NUMCHAR    util::tnumchar


namespace miku
{
	template <typename T>
	int ord_(T value)
	{
		return (int)value;
	}

	template <typename T>
	void inc_(T& value)
	{
		int temp = (int)value;
		value = (T)(temp+1);
	}

	template <typename T>
	void inc_(T& value, int amount)
	{
		value += amount ;
	}

	template <typename T>
	void dec_(T& value)
	{
		--value;
	}

	template <typename T>
	void dec_(T& value, int amount)
	{
		value -= amount ;
	}

	template <typename T>
	T succ_(T value)
	{
		return T(int(value)+1);
	}

	template <typename T>
	T pred_(T value)
	{
		return T(int(value)-1);
	}

	template <typename T>
	T abs_(T value)
	{
		return (value >= 0) ? value : -value;
	}

	#define random(max_val) avej_lite::util::Random(max_val)

	template <typename T>
	T max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template <typename T>
	T min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <typename T>
	T clamp(T a, T min, T max)
	{
		return (a < min) ? min : (a > max) ? max : a;
	}

	namespace util
	{
		void ClearKeyBuffer(void);
		void ComposeString(tchar pBuffer[], const tchar* szFormat, const int data ...);

		class CTextFileFromRes
		{
		public:
			CTextFileFromRes(const unsigned short* sz_text_stream, int size);
			~CTextFileFromRes();

			bool IsValid(void);
			bool GetString(tchar* out_str, int str_len);

		private:
			struct TImpl;
			TImpl* m_p_impl;
		};

		int    tstrlen(const tchar* str1);
		tchar* tstrcpy(tchar* dest, const tchar* src);
		tchar* tstrncpy(tchar* dest, const tchar* src, size_t n);
		tchar* tstrcat(tchar* dest, const tchar* src);
		tchar* tstrncat(tchar* dest, const tchar* src, size_t n);
		int    tstrncmp(const tchar* s1, const tchar* s2, size_t n);
		int    tnumchar(const tchar* str1);

		class tstring
		{
			enum { MAX_LEN = 256 };

		private:
			tchar m_string[MAX_LEN+1];

		public:
			tstring(void)
			{
				m_string[0] = 0;
			}
			tstring(const tchar* lpsz)
			{
				tstrncpy(m_string, lpsz, MAX_LEN);
			}

			operator const tchar*() const
			{
				return m_string;
			};

			const tchar* c_str() const
			{
				return m_string;
			};

			const tstring& operator=(const tchar* lpsz)
			{
				tstrncpy(m_string, lpsz, MAX_LEN);
				return *this;
			}

			const tstring& operator+=(const tchar* lpsz)
			{
				tstrncat(m_string, lpsz, MAX_LEN);
				return *this;
			}

			void copyToFront(const tstring& lpsz)
			{
				tchar m_temp[MAX_LEN+1];
				tstrncpy(m_temp, m_string, MAX_LEN);
				tstrncpy(m_string, lpsz.m_string, MAX_LEN);
				tstrncat(m_string, m_temp, MAX_LEN);
			}

			bool isEmpty(void) const
			{
				return (m_string[0] == 0);
			}
		};
	}

} // namespace miku

#endif // #ifndef __SFWV_UTIL_H__

