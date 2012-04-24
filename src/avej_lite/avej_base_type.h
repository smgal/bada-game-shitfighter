
#ifndef __AVEJ_BASE_TYPE_H__
#define __AVEJ_BASE_TYPE_H__

#include "avej_pixel_format.h"

namespace avej_lite
{
	//! 버퍼를 표현하기 위한 기본 구조
	/*!
	 * \ingroup gfx2d
	 */
	struct TBufferDesc
	{
		TPixelFormat  pixel_format;    //!< 픽셀 포맷
		int           width;           //!< 버퍼의 가로 영역의 크기
		int           height;          //!< 버퍼의 세로 영역의 크기
		int           depth;           //!< 픽셀 깊이. 하나의 픽셀을 구성하기 위한 비트 수
		bool          has_color_key;   //!< 버퍼 고유의 컬러키가 정의 되어 있는지의 여부
		unsigned long color_key;       //!< 버퍼 고유의 컬러키. \c has_color_key가 true일 때 이 값이 유효함
	};

	//! 버퍼에 접근 하기 위한 기본 구조
	/*!
	 * \ingroup gfx2d
	 */
	struct TLockDesc
	{
		void*         p_buffer;        //!< 접근이 유효한 버퍼의 시작 주소
		long          bytes_per_line;  //!< (x,y+1)와 (x,y)의 실제 주소 차이. 'addr(x,y+1) - addr(x, y)'의 값
	};

	//! 서피스를 구성하기 위한 기본 구조
	/*!
	 * \ingroup gfx2d
	 */
	struct TSurfaceDesc
	{
		TBufferDesc    buffer_desc;    //!< 서피스의 버퍼 정의
		TLockDesc      lock_desc;      //!< 서피스의 버퍼 접근 속성 정의
		unsigned long  hw_handle;      //!< 서피스와 관련된 H/W 속성. 이 값이 0이면 H/W 속성은 없음
		bool           has_ownership;  //!< 서피스 버퍼에 대해 소유권이 있는지의 여부. 소유권이 있다면 서피스 해제 시 버퍼 메모리도 같이 해제

		//! 서피스 해제 시 호출 되는 함수를 정의. 이것이 정의되지 않으면 미리 정의된 디폴트 동작을 수행
		void          (*Destroy)(TSurfaceDesc& native_surface);
		//! 서피스 접근 시 호출 되는 함수를 정의. 이것이 정의되지 않으면 \c lock_desc 의 값을 사용
		bool          (*Lock)(TSurfaceDesc& native_surface);
		//! 서피스 접근 완료 시 호출 되는 함수를 정의.
		bool          (*Unlock)(TSurfaceDesc& native_surface);
		//! ARGB8888 기준의 컬러를 서피스 버퍼에 맞는 픽셀 포맷으로 변환하는 함수를 정의.
		unsigned long (*ToTargetColor)(unsigned long color_32bits);
		//! 사각 영역을 특정 컬러로 채우기 위한 자체적인 함수를 정의
		bool          (*FillRect)(int x, int y, unsigned int width, unsigned int height);
		//! 서피스끼리 사각 영역을 복사하기 위한 자체적인 함수를 정의
		bool          (*CopyRect)(int x, int y, TSurfaceDesc& src_native_surface, int src_x, int src_y, unsigned int src_width, unsigned int src_height);
	};
}

#endif // #ifndef __AVEJ_BASE_TYPE_H__
