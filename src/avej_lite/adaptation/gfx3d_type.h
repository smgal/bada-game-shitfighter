
#ifndef __GFX3D_TYPE_H__
#define __GFX3D_TYPE_H__

#include <vector>

namespace avej_lite { namespace gfx3d
{
	typedef int TFixed;

	////////////////////////////////////////////////////////////////////////////
	// structure

	struct TFixedVec2
	{
		TFixed x, y;

		TFixedVec2() {}
		TFixedVec2(TFixed _x, TFixed _y)
			: x(_x), y(_y) {}
	};

	struct TFixedVec3
	{
		TFixed x, y, z;

		TFixedVec3() {}
		TFixedVec3(TFixed _x, TFixed _y, TFixed _z)
			: x(_x), y(_y), z(_z) {}
	};

	struct TFixedVec4
	{
		union
		{
			TFixed fixed[4];
			struct
			{
				TFixed r, g, b, a;
			};
		};

		TFixedVec4() {}
		TFixedVec4(TFixed _r, TFixed _g, TFixed _b, TFixed _a)
			: r(_r), g(_g), b(_b), a(_a) {}
	};

	inline TFixed _FXD_from8bit(unsigned char data)
	{
		TFixed temp = TFixed(data);
		temp  += (temp >> 7);
		temp <<= 8;

		return temp;
	}

	inline TFixedVec4 _FXD_color2array(unsigned long color32bit)
	{
		TFixed a = _FXD_from8bit((color32bit >> 24) & 0xFF);
		TFixed r = _FXD_from8bit((color32bit >> 16) & 0xFF);
		TFixed g = _FXD_from8bit((color32bit >>  8) & 0xFF);
		TFixed b = _FXD_from8bit((color32bit >>  0) & 0xFF);

		return TFixedVec4(r, g, b, a);
	}
}} // namespace avej_lite { namespace gfx3d

#endif // #ifndef __GFX3D_TYPE_H__
