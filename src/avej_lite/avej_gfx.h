
#ifndef __AVEJ_GFX_H__
#define __AVEJ_GFX_H__

/*!
 * \file
 * \ingroup AVEJ library
 * \brief AVEJ library의 graphics interface
 * \author SMgal smgal@hitel.net
 * \date 2006-12-31
*/

#include "avej_base_type.h"

namespace avej_lite
{

struct TFixed16
{
	explicit TFixed16() {}
	explicit TFixed16(int data): m_data(data << 16) {}
	explicit TFixed16(float data): m_data(long(data * 65536.0f)) {}
	explicit TFixed16(double data): m_data(long(data * 65536.0)) {}

	TFixed16 operator+=(const TFixed16& ref)
	{
		this->m_data += ref.m_data;
		return *this;
	}

	TFixed16 operator-=(const TFixed16& ref)
	{
		this->m_data -= ref.m_data;
		return *this;
	}

	long m_data;
};

inline TFixed16 operator+(TFixed16 lhs, TFixed16 rhs)
{
	TFixed16 temp;
	temp.m_data = lhs.m_data + rhs.m_data;

	return temp;
}

inline TFixed16 operator-(TFixed16 lhs, TFixed16 rhs)
{
	TFixed16 temp;
	temp.m_data = lhs.m_data - rhs.m_data;

	return temp;
}

struct TLayerDesc
{
	enum
	{
		OPTION_NONE       = 0x00000000,
		OPTION_DEPTH_TEST = 0x00000001,
		OPTION_DWORD      = 0x7FFFFFFF,
	};
	enum
	{
		LAYER_ATTRIB_OPAQUE       = 0x00000000,
		LAYER_ATTRIB_ALPHA_BLEND  = 0x00000001,
		LAYER_ATTRIB_ALPHA_TEST   = 0x00000002,
		LAYER_ATTRIB_DWORD        = 0x7FFFFFFF,
	};
	unsigned long num_layer;
	unsigned long layer_attrib[MAX_LAYER_IN_SURFACE];
	unsigned long option_flag;
};

//! Graphics의 surface를 위한 interface
/*!
 * \ingroup AVEJ library graphics
*/
class IGfxSurface
{
public:
	//! Surface의 메모리 디스크립터
	struct TLockDesc
    {
    	void* pMem;          //!< surface 메모리의 첫 주소
        int   pitch;         //!< surface에서 y좌표로 1 픽실을 증가시키기 위한 픽셀 수
    };

	template <typename T>
	struct TPoint
    {
		T x, y;
    };

	struct TSpriteDesc
	{
		const IGfxSurface* ref_surface;
		int x_sour;
		int y_sour;
		int w_sour;
		int h_sour;
	};

	virtual ~IGfxSurface(void) {}

	void SetClipRect(int x, int y, int w, int h) {}

	//! 현재 출력할 레이어를 지정한다. 0이 가장 최상위이고 번호가 올라갈수록 아래쪽에 놓인다.
	virtual void  SetActiveLayer(unsigned long layer) = 0;

	//! Surface의 지정한 사각 영역을 지정한 색으로 채운다.
	virtual bool  FillRect(unsigned long color, TFixed16 x, TFixed16 y, TFixed16 w, TFixed16 h) = 0;
	virtual bool  FillRect(unsigned long color, int x, int y, int width, int height) = 0;
	virtual bool  FillRect(unsigned long color, TPoint<TFixed16> point[]) = 0;
	virtual bool  FillRect(unsigned long color, TPoint<int> point[]) = 0;

	//! Surface에 선을 긋는다.
	virtual bool  DrawLine(unsigned long color, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2) = 0;
	virtual bool  DrawGradLine(unsigned long color1, unsigned long color2, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2) = 0;

	//! Surface에 다른 surface의 지정한 사각 영역을 복사한다.
	virtual bool  BitBlt(TFixed16 x, TFixed16 y, const IGfxSurface* ref_surface, TFixed16 x_sour, TFixed16 y_sour, TFixed16 w_sour, TFixed16 h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  BitBlt(int x, int y, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  BitBlt(int x, int y, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0) = 0;

	//! 
	virtual bool  StretchBlt(TFixed16 x_dest, TFixed16 y_dest, TFixed16 w_dest, TFixed16 h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  StretchBlt(int x_dest, int y_dest, int w_dest, int h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  StretchBlt(int x_dest, int y_dest, int w_dest, int h_dest, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0) = 0;

	//! Surface의 핸들을 얻는다.
	virtual void* Handle(void) const = 0;
	//! ARGB8888 형태의 컬러에 대해 surface의 해당 컬러 값을 얻는다.
	virtual unsigned long Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) = 0;
	//! ARGB8888 형태의 컬러에 대해 surface의 해당 컬러 값을 얻는다.
	virtual unsigned long Color(unsigned long absColor) = 0;
};

//! Graphics의 device를 위한 interface
/*!
 * \ingroup AVEJ library graphics
*/
class IGfxDevice
{
public:
	//! Device의 속성을 나타내기 위한 디스크립터
	struct TDeviceDesc
    {
    	int  width;    //!< Device 버퍼의 가로 크기
    	int  height;   //!< Device 버퍼의 세로 크기
    	int  pitch;    //!< Device의 버퍼에서 y좌표로 1 픽셀을 증가시키기 위한 픽셀 수
    	int  depth;    //!< Device 버퍼의 한 픽셀을 구성하는 비트 수
    	int  format;   //!< Device 버퍼의 픽셀 구조
    };

	virtual ~IGfxDevice(void) {}

	//! Device가 그림을 그릴 수 있도록 준비한다.
    virtual bool  BeginDraw(bool clear_flag = true) = 0;
	//! Device가 그림을 다 그렸음을 알려 준다.
    virtual bool  EndDraw(void) = 0;
	//! Device에 그려진 그림을 화면으로 나타낸다.
    virtual bool  Flip(void) = 0;
	//! Device의 백버퍼를 가져 온다.
    virtual bool  GetSurface(IGfxSurface** ppSurface) = 0;

	//! Device에 사용 가능한 surface를 descriptor를 통해 생성한다.
	virtual bool  CreateSurface(const TSurfaceDesc& surface_desc, IGfxSurface** pp_surface) = 0;

	//! Device에 사용 가능한 surface를 이미지 파일로부터 생성한다.
    virtual bool  CreateSurfaceFrom(const char* szFileName, IGfxSurface** ppSurface) = 0;
	virtual bool  CreateSurfaceFrom(const unsigned char* sz_stream, unsigned long stream_size, IGfxSurface** pp_surface) = 0;
	//! Device의 속성을 얻는다.
    virtual bool  GetDesc(TDeviceDesc& desc) = 0;
	//! 
    virtual void  SetLayerDesc(TLayerDesc& layer_desc) = 0;
	//! 
    virtual void  GetLayerDesc(TLayerDesc& layer_desc) = 0;

	//! Device의 인스턴스를 얻는다.
    static IGfxDevice* GetInstance(void);
	//! Device의 인스턴스의 사용이 끝났음을 알려 준다.
    static int Release(void);
};

/*!
\fn bool IGfxSurface::AssignType(EType type, unsigned long color = 0);

\param type [in]
	Surface의 출력 형태
\param color [in]
	Surface의 컬러 키. \p type이 \p TYPE_COLORKEY일때만 유효하다.

\return 함수가 성공하면 \c true 를 돌려주고 실패하면 \c false 를 돌려준다.

\remark
\p TYPE_NONE이 지정되면 이 surface는 항상 블렌딩 없이 출력이 된다. \n
\p TYPE_COLORKEY이 지정되면 \c color에서 지정한 값을 컬러키로 적용해서 출력한다. \n
\p TYPE_COLORKEY_AUTO이 지정되면 (0,0)의 픽셀 값을 컬러키로 적용해서 출력한다. \n
\p TYPE_ALPHA이 지정되면 각 픽셀에 정의되어 있는 알파 성분으로 블렌딩 해서 출력한다.

\par 예제:
\code
IGfxDevice*  pGfxDevice;
IGfxSurface* pSprite;

pGfxDevice = IGfxDevice::GetInstance();

pGfxDevice->CreateSurfaceFrom("./tile.bmp", &pSprite);
pSprite->AssignType(IGfxSurface::TYPE_COLORKEY_AUTO);
\endcode

\image html exmaple.png "예제의 결과"

\see IGfxSurface::BitBlt()
*/

} // namespace avej_lite

#endif // #ifndef __AVEJ_GFX_H__
