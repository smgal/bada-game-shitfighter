
/*
 * Win32
 * config 1: rgba=5:6:5:0(16), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 2: rgba=5:6:5:0(16), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 3: rgba=5:6:5:0(16), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 4: rgba=8:8:8:8(32), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 5: rgba=8:8:8:8(32), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 6: rgba=8:8:8:8(32), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 *
 * WIZ
 * config 1: rgba=5:6:5:0(16), ds=16:0, caveat=3050, native=0, level=0, surface=0005, visual=3038, id=0
 * config 2: rgba=5:6:5:0(16), ds=16:0, caveat=3050, native=0, level=0, surface=0005, visual=3038, id=0
*/

#if defined(_WIN32)
#	pragma warning(disable: 4786)
#	pragma comment(lib, "libgles_cm.lib")
#endif

////////////////////////////////////////////////////////////////////////////////
// uses

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
// native OpenGL definition

namespace avej_lite { namespace gfx3d
{
	bool InitOpenGL(unsigned int width, unsigned int height, unsigned int depth);
	void DoneOpenGL(void);
	void SwapBuffers(void);
	void ProcessMessage(unsigned long delay_time);
}}

////////////////////////////////////////////////////////////////////////////////
// native OpenGL ES implementation

#define NULL  0

#define CHECK_EGL_ERROR(message)                 \
	{                                            \
		EGLint error = eglGetError();            \
		if (error != EGL_SUCCESS)                \
		{                                        \
			osal::PrintEGLError(message, error); \
			return false;                        \
		}                                        \
	}

namespace avej_lite { namespace gfx3d
{
	struct TEGLAttrib
	{
		bool                 is_valid;
		EGLNativeWindowType  egl_window;
		EGLNativeDisplayType egl_dc;
		EGLDisplay			 egl_display;
		EGLConfig			 egl_config[10];
		EGLSurface			 egl_surface_window;
		EGLContext			 egl_context;

		EGLSurface           egl_pbuffer;
		GLuint               tex_pbuffer;

		TEGLAttrib()
		: is_valid(false) {}
	};
	
	static TEGLAttrib s_egl_attrib;

	EGLDisplay GetEglDisplay(void)
	{
		return (s_egl_attrib.is_valid) ? s_egl_attrib.egl_display : 0;
	}

	bool InitOpenGL(unsigned int width, unsigned int height, unsigned int depth)
	{
		if (s_egl_attrib.is_valid)
			return false;

		s_egl_attrib.egl_window  = osal::GetNativeWindow(0, width, height, depth);
		s_egl_attrib.egl_dc      = osal::GetNativeDC(); // -> 0
		s_egl_attrib.egl_display = eglGetDisplay(s_egl_attrib.egl_dc);

		EGLint major_version;
		EGLint minor_version;

		if (!eglInitialize(s_egl_attrib.egl_display, &major_version, &minor_version))
		{
			osal::ShowMessage("eglInitialize() failed.");
			return false;
		}

#if defined(_WIN32) && defined(EGL_VERSION_1_3)
		eglBindAPI(EGL_OPENGL_ES_API);
		CHECK_EGL_ERROR("eglBindAPI");
#endif

		const EGLint config_attrib[] =
		{
#if (TARGET_DEVICE==TARGET_GP2XWIZ)
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_NONE
#elif (TARGET_DEVICE==TARGET_BADA)
			EGL_RED_SIZE,	5,
			EGL_GREEN_SIZE,	6,
			EGL_BLUE_SIZE,	5,
			EGL_ALPHA_SIZE,	0,
			EGL_DEPTH_SIZE, 8,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_NONE
#else
			EGL_RED_SIZE,	8,
			EGL_GREEN_SIZE,	8,
			EGL_BLUE_SIZE,	8,
			EGL_ALPHA_SIZE,	8,
			EGL_DEPTH_SIZE, 8,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_NONE
#endif
		};

		{
			int config;

			if (!eglChooseConfig(s_egl_attrib.egl_display, config_attrib, &s_egl_attrib.egl_config[0], 10, &config) || (config < 1))
			{
				osal::ShowMessage("eglChooseConfig() failed.");
				return false;
			}
		}

		EGLConfig& egl_config = s_egl_attrib.egl_config[0];

		s_egl_attrib.egl_surface_window = eglCreateWindowSurface(s_egl_attrib.egl_display, egl_config, s_egl_attrib.egl_window, NULL); // NULL -> config_attrib
		CHECK_EGL_ERROR("eglCreateWindowSurface");

#if defined(_WIN32) && defined(EGL_VERSION_1_3)
		{
			EGLint context_attrib[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
			s_egl_attrib.egl_context = eglCreateContext(s_egl_attrib.egl_display, egl_config, NULL, context_attrib);
		}
#else
		s_egl_attrib.egl_context = eglCreateContext(s_egl_attrib.egl_display, egl_config, EGL_NO_CONTEXT, NULL); // EGL_NO_CONTEXT -> 0, NULL -> config_attrib
#endif
		CHECK_EGL_ERROR("eglCreateContext");

		eglMakeCurrent(s_egl_attrib.egl_display, s_egl_attrib.egl_surface_window, s_egl_attrib.egl_surface_window, s_egl_attrib.egl_context);
		CHECK_EGL_ERROR("eglMakeCurrent");

		glClearColorx(0, 0, 0, 0);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

#if defined(_CAANOO_)
		// 카누의 액정에 대한 보정
		glViewport(0, -3, 320, 240);
#endif

		// HW capability probing
		{
			struct TCapability
			{
				int  max_tex_unit;
				int  max_tex_size;
				bool use_pbuffer;
			} capability;

			glGetIntegerv(GL_MAX_TEXTURE_UNITS, &capability.max_tex_unit);
			glGetIntegerv(GL_MAX_TEXTURE_SIZE,  &capability.max_tex_size);

			{
				GLint surface_type;

				eglGetConfigAttrib(s_egl_attrib.egl_display, egl_config, EGL_SURFACE_TYPE, &surface_type);
				capability.use_pbuffer = (surface_type & EGL_PBUFFER_BIT) > 0;
			}
		}

		s_egl_attrib.is_valid = true;

		return true;
	}

	void DoneOpenGL(void)
	{
		if (s_egl_attrib.is_valid)
		{
#if defined(_MSC_VER)
#else
			eglMakeCurrent(s_egl_attrib.egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
#endif

			if (s_egl_attrib.egl_context)
				eglDestroyContext(s_egl_attrib.egl_display, s_egl_attrib.egl_context);

			if (s_egl_attrib.egl_surface_window)
				eglDestroyContext(s_egl_attrib.egl_display, s_egl_attrib.egl_surface_window);

			osal::ReleaseNativeWindow(s_egl_attrib.egl_window);
			osal::ReleaseNativeDC(s_egl_attrib.egl_dc);

			eglTerminate(s_egl_attrib.egl_display);
		}

		s_egl_attrib.is_valid = false;
	}

	void s_DrawTexture2D(int x, int y, int w, int h, GLuint texture, int tex_w, int tex_h, bool can_display)
	{
		const GLfixed Z_DEPTH = 50;

		struct
		{
			GLint x, y, width, height;
		} view_port;

		// 현재 view port 정보 얻기
		glGetIntegerv(GL_VIEWPORT, (GLint*)&view_port);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrthox(view_port.x, view_port.width, view_port.y , view_port.height, -Z_DEPTH, Z_DEPTH);

		const GLfixed x1 = x;
		const GLfixed y1 = view_port.height - h - y;
		const GLfixed x2 = x1 + w;
		const GLfixed y2 = y1 + h;

		const GLfixed vertices[] =
		{
			x1, y1,
			x2, y1,
			x1, y2,
			x2, y2
		};

		const GLfixed tex_x1 = 0;
		const GLfixed tex_y1 = 0;
		const GLfixed tex_x2 = 65536 * view_port.width  / tex_w;
		const GLfixed tex_y2 = 65536 * view_port.height / tex_h;

		const GLfixed texture_coord[] =
		{
			tex_x1, tex_y1,
			tex_x2, tex_y1,
			tex_x1, tex_y2,
			tex_x2, tex_y2
		};

		static const unsigned short index_buffer[] =
		{
			0, 1, 2, 3
		};

		// vertex buffer 입력
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FIXED, 0, vertices);

		// texture coordinate 입력
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FIXED, 0, texture_coord);

		// texture를 사용 함
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatex(0, 0, -Z_DEPTH+1);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, &index_buffer[0]);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}


	void SwapBuffers(void)
	{
		eglSwapBuffers(s_egl_attrib.egl_display, s_egl_attrib.egl_surface_window);
	}
	
	void ProcessMessage(unsigned long delay_time)
	{
		osal::ProcessMessage(delay_time);
	}
}}
