
#ifndef __GFX3D_CONFIG_H__
#define __GFX3D_CONFIG_H__

#include "../avej_config.h"

#if !defined(OPENGL_ES_VERSION)
#	define  OPENGL_ES_VERSION 110
#endif

#if (TARGET_DEVICE == TARGET_BADA)
#include <FGraphicsOpengl.h>
using namespace Osp::Graphics::Opengl;
#else

#if   (OPENGL_ES_VERSION == 110)
#	include <GLES/egl.h>
#	include <GLES/gl.h>
	typedef NativeWindowType  EGLNativeWindowType;
	typedef NativeDisplayType EGLNativeDisplayType;
#elif (OPENGL_ES_VERSION == 200)
#	include <EGL/egl.h>
#	include <GLES2/gl2.h>
#endif

#endif

#if !defined(TARGET_DEVICE)
#	if defined(_WIN32)
#		define TARGET_DEVICE  TARGET_WIN32
#	elif defined(ARM9)
#		define TARGET_DEVICE  TARGET_GP2XWIZ
#	else
#		error  "add -DTARGET_DEVICE=TARGET_??? in your Makefile."
#	endif
#endif

#define Z_SCALE   4
#define Z_NEAR    1000
#define Z_ORIGIN (Z_NEAR * Z_SCALE)
#define Z_FAR    (Z_NEAR * 100)

#define USE_PERSPECTIVE  1

#endif // #ifndef __GFX3D_CONFIG_H__
