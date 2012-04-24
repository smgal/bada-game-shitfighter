
#include "gfx3d_config.h"

////////////////////////////////////////////////////////////////////////////////
// OSAL definition

namespace avej_lite { namespace gfx3d
{
	namespace osal
	{
		EGLNativeWindowType  GetNativeWindow(int screen, unsigned int width, unsigned int height, unsigned int depth);
		EGLNativeDisplayType GetNativeDC(void);
		void                 ReleaseNativeWindow(EGLNativeWindowType h_wnd);
		void                 ReleaseNativeDC(EGLNativeDisplayType h_dc);
		void                 ProcessMessage(unsigned long delay_time);
		bool                 PrintEGLError(const char* sz_message, int error);
		void                 ShowMessage(const char* sz_message, const char* sz_title = 0);
	}
}}

////////////////////////////////////////////////////////////////////////////////
// OSAL implementation

#if (TARGET_DEVICE == TARGET_WIN32)

// 삭제

#elif (TARGET_DEVICE == TARGET_GP2XWIZ)

// 삭제

#elif (TARGET_DEVICE == TARGET_BADA)

#include <stdlib.h>
#include <FBase.h>

extern void* g_hNativeWindow;


EGLNativeWindowType avej_lite::gfx3d::osal::GetNativeWindow(int screen, unsigned int width, unsigned int height, unsigned int depth)
{
	return (EGLNativeWindowType)g_hNativeWindow;
}

EGLNativeDisplayType avej_lite::gfx3d::osal::GetNativeDC(void)
{
	return (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
}

void avej_lite::gfx3d::osal::ReleaseNativeWindow(EGLNativeWindowType h_wnd)
{
}

void avej_lite::gfx3d::osal::ReleaseNativeDC(EGLNativeDisplayType h_dc)
{
}

void avej_lite::gfx3d::osal::ProcessMessage(unsigned long delay_time)
{
}

bool avej_lite::gfx3d::osal::PrintEGLError(const char* sz_message, int error)
{
	AppLog("[SMGAL] EGL error(%d): %s", error, sz_message);
	return true;
}

void avej_lite::gfx3d::osal::ShowMessage(const char* sz_message, const char* sz_title)
{
	AppLog("[SMGAL] [%s]: %s", sz_title, sz_message);
}

#endif
