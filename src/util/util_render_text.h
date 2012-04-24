
#ifndef __UTIL_RENDER_TEXT_H__
#define __UTIL_RENDER_TEXT_H__

typedef wchar_t widechar;

class IorzCodeParser
{
public:
	struct TGlyphRect
	{
		int nRect;
		struct { int x1, y1, x2, y2; } rect[3];
	};

	virtual ~IorzCodeParser() {}
	virtual bool GetGlyphRect(unsigned long index, TGlyphRect& glyphRect) const = 0;
};

class IorzRenderText
{
public:
	struct TBltParam
	{
		bool          is_available;
		long          x_dest, y_dest;
		long          x1, y1, x2, y2;
		unsigned long color;
		int           index_color;
	};

	typedef void  (*TFnBitBlt)(int x_dest, int y_dest, int width, int height, int x_sour, int y_sour, unsigned long color);

	virtual void RenderText(int x_dest, int y_dest, const widechar* sz_text, unsigned long color, TFnBitBlt fn_bit_blt) const = 0;
	virtual int  RenderText(int x_dest, int y_dest, const widechar* sz_text, unsigned long color, TBltParam blt_param[], int num_blt_param) const = 0;

protected:
	virtual ~IorzRenderText() {}
};

extern IorzRenderText* GetTextInstance(void);
extern void SetTextBufferDesc(unsigned int font_height, unsigned int buffer_width);

#endif
