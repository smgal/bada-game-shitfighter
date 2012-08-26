
#pragma warning(disable: 4786)

#include <vector>

#include "sfwv_config.h"
#include "sfwv_util.h"

namespace miku
{

void util::ClearKeyBuffer(void)
{
	avej_lite::CInputDevice& input_device = avej_lite::singleton<avej_lite::CInputDevice>::get();
	input_device.UpdateInputState();

	// remove key-buffer
	for (avej_lite::TInputKey key = avej_lite::INPUT_KEY_BEGIN; key < avej_lite::INPUT_KEY_END; inc_(key))
		input_device.WasKeyPressed(key);
}

////////////////////////////////////////////////////////////////////////////////
// class CTextFileFromRes

struct util::CTextFileFromRes::TImpl
{
public:
	TImpl(const unsigned short* sz_text_stream, int size)
	{
		if (sizeof(unsigned short) != sizeof(tchar))
		{
			m_text_stream.bind(new tchar[size]);

			const unsigned short* p_src = sz_text_stream;
			tchar* p_dst = m_text_stream.get();
			tchar* p_dst_end = p_dst + size;

			while (p_dst < p_dst_end)
				*p_dst++ = tchar(*p_src++);
		}

		const tchar* p_stream_beg = (m_text_stream.get()) ? m_text_stream.get() : reinterpret_cast<const tchar*>(sz_text_stream);
		const tchar* p_stream_end = p_stream_beg + size;

		const tchar* p_stream = p_stream_beg;

		while (p_stream < p_stream_end)
		{
			if ((*p_stream != 0x0D) && (*p_stream != 0x0A))
			{
				++p_stream;
				continue;
			}

			m_paragraph_list.push_back(std::make_pair(p_stream_beg, p_stream));

			++p_stream;

			// end of file with <CR>
			if (p_stream >= p_stream_end)
				break;

			if (*p_stream == 0x0A)
				++p_stream;

			p_stream_beg = p_stream;
		}

		m_current_paragraph = m_paragraph_list.begin();
	}

	bool IsValid(void)
	{
		if (m_paragraph_list.size() == 0)
			return false;

		return (m_current_paragraph != m_paragraph_list.end());
	}

	bool GetString(tchar* out_str, int str_len)
	{
		if (m_current_paragraph == m_paragraph_list.end())
			return false;

		int sour_len = m_current_paragraph->second - m_current_paragraph->first;
		int copy_len = min(str_len, sour_len);

		STRNCPY(out_str, m_current_paragraph->first, copy_len);

		if (copy_len < str_len)
			out_str[copy_len] = 0;

		++m_current_paragraph;

		return true;
	}

private:
	avej_lite::util::auto_array<tchar> m_text_stream;

	typedef std::pair<const tchar*, const tchar*> TParagraph;

	std::vector<TParagraph>           m_paragraph_list;
	std::vector<TParagraph>::iterator m_current_paragraph;

};

util::CTextFileFromRes::CTextFileFromRes(const unsigned short* sz_text_stream, int size)
: m_p_impl(new TImpl(sz_text_stream, size))
{
}

util::CTextFileFromRes::~CTextFileFromRes()
{
	delete m_p_impl;
}

bool util::CTextFileFromRes::IsValid(void)
{
	return m_p_impl->IsValid();
}

bool util::CTextFileFromRes::GetString(tchar* out_str, int str_len)
{
	return m_p_impl->GetString(out_str, str_len);
}

////////////////////////////////////////////////////////////////////////////////
// ComposeString()

static void s_Int2Str(tchar **ppBuffer, int value)
{
	if (value == 0)
	{
		*((*ppBuffer)++) = TCHAR('0');
	}
	else
	{
		if (value < 0)
		{
			*((*ppBuffer)++) = TCHAR('-');
			value = -value;
		}

		int nNum = 0;
		int temp = value;
		while (temp)
		{
			temp = temp / 10;
			++nNum;
		}

		int nLoop = nNum;
		while (nLoop-- > 0)
		{
			(*ppBuffer)[nLoop] = (value % 10) + TCHAR('0');
			value = value / 10;
		}
		*ppBuffer += nNum;
	}
}

void util::ComposeString(tchar pBuffer[], const tchar* szFormat, const int data ...)
{
	if (pBuffer == 0)
		return;

	const int* pData = &data;

	while (*szFormat)
	{
		if (*szFormat != TCHAR('@'))
		{
			*pBuffer++ = *szFormat++;
			continue;
		}
		++szFormat;
		s_Int2Str(&pBuffer, *pData++);
	}

	*pBuffer = 0;
}

int util::tstrlen(const tchar* str1)
{
	const tchar* str2 = str1;
	while (*str2++)
		;
	return str2-str1-1;
}

tchar* util::tstrcpy(tchar* dest, const tchar* src)
{
	return util::tstrncpy(dest, src, util::tstrlen(src)+1);
}

tchar* util::tstrncpy(tchar* dest, const tchar* src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i]; i++)
		dest[i] = src[i];

	for (; i < n; i++)
		dest[i] = TCHAR('\0');

	return dest;
}

tchar* util::tstrcat(tchar* dest, const tchar* src)
{
	return util::tstrncat(dest, src, util::tstrlen(src)+1);
}

tchar* util::tstrncat(tchar* dest, const tchar* src, size_t n)
{
   size_t dest_len = util::tstrlen(dest);
   size_t i;

   for (i = 0 ; i < n && src[i]; i++)
	   dest[dest_len + i] = src[i];

   dest[dest_len + i] = TCHAR('\0');

   return dest;
}

int util::tstrncmp(const tchar* s1, const tchar* s2, size_t n)
{
	if (n == 0)
		return 0;

	while (*s1 == *s2 && n-- > 0)
	{
		if (*s1 == 0 || n == 0)
			return 0;
		s1++;
		s2++;
	}

	if (n <= 0)
		return 0;

	return (*s1 > *s2) ? 1 : -1;
}

int util::tnumchar(const tchar* str1)
{
	if (sizeof(tchar) == 1)
	{
		return STRLEN(str1);
	}
	else
	{
		int count = 0;

		while (*str1)
		{
			count += (*str1++ < 256) ? 1 : 2;
		}

		return count;
	}
}

} // namespace miku
