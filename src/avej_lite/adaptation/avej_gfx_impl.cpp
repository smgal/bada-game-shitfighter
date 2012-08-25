
#include "../avej_config.h"
#include "../avej_gfx.h"

#include "../avej_pixel_format.h"
#include "../avej_base_type.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#if (TARGET_DEVICE == TARGET_BADA)
extern int g_nativeWindowWidth;
extern int g_nativeWindowHeight;
#endif

using namespace avej_lite;

////////////////////////////////////////////////////////////////////////////////
//

class CGfxSurfaceImplNull: public IGfxSurface
{
public:
	CGfxSurfaceImplNull() {}
	~CGfxSurfaceImplNull() {}

	void          SetActiveLayer(unsigned long layer) {};
	bool          FillRect(unsigned long color, TFixed16 x, TFixed16 y, TFixed16 w, TFixed16 h) { return false; }
	bool          FillRect(unsigned long color, TPoint<TFixed16> point[]) { return false; }
	bool          FillRect(unsigned long color, int x, int y, int w, int h) { return false; }
	bool          FillRect(unsigned long color, TPoint<int> point[]) { return false; }
	bool          DrawLine(unsigned long color, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2) { return false; }
	bool          DrawGradLine(unsigned long color1, unsigned long color2, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2) { return false; }
	bool          BitBlt(TFixed16 x, TFixed16 y, const IGfxSurface* ref_surface, TFixed16 x_sour, TFixed16 y_sour, TFixed16 w_sour, TFixed16 h_sour, const unsigned long color[] = 0) { return false; }
	bool          BitBlt(int x, int y, const IGfxSurface* srcSurface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) { return false; }
	bool          BitBlt(int x, int y, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0) { return false; }
	bool          StretchBlt(TFixed16 x_dest, TFixed16 y_dest, TFixed16 w_dest, TFixed16 h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) { return false; }
	bool          StretchBlt(int x, int y, int w_dest, int h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) { return false; }
	bool          StretchBlt(int x, int y, int w_dest, int h_dest, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0) { return false; }
	void*         Handle(void) const { return 0; }
	unsigned long Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) { return 0; }
	unsigned long Color(unsigned long absColor) { return 0; }
};

class CGfxSurfaceImplHW: public CGfxSurfaceImplNull
{
public:
	CGfxSurfaceImplHW(const TSurfaceDesc& surface_desc): m_surface_desc(surface_desc) {}
	~CGfxSurfaceImplHW() {}

	void*         Handle(void) const { return (void*)&m_surface_desc; }

private:
	TSurfaceDesc m_surface_desc;
};

class CGfxSurfaceImplBackBuffer: public IGfxSurface
{
	friend class IGfxDeviceImpl;

public:
	CGfxSurfaceImplBackBuffer();
	~CGfxSurfaceImplBackBuffer();

	void          SetActiveLayer(unsigned long layer);
	bool          FillRect(unsigned long color, TFixed16 x, TFixed16 y, TFixed16 w, TFixed16 h);
	bool          FillRect(unsigned long color, TPoint<TFixed16> point[]);
	bool          FillRect(unsigned long color, int x, int y, int w, int h);
	bool          FillRect(unsigned long color, TPoint<int> point[]);
	bool          DrawLine(unsigned long color, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2);
	bool          DrawGradLine(unsigned long color1, unsigned long color2, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2);
	bool          BitBlt(TFixed16 x, TFixed16 y, const IGfxSurface* ref_surface, TFixed16 x_sour, TFixed16 y_sour, TFixed16 w_sour, TFixed16 h_sour, const unsigned long color[] = 0);
	bool          BitBlt(int x, int y, const IGfxSurface* srcSurface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0);
	bool          BitBlt(int x, int y, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0);
	bool          StretchBlt(TFixed16 x_dest, TFixed16 y_dest, TFixed16 w_dest, TFixed16 h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0);
	bool          StretchBlt(int x, int y, int w_dest, int h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0);
	bool          StretchBlt(int x, int y, int w_dest, int h_dest, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0);
	void*         Handle(void) const;
	unsigned long Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b);
	unsigned long Color(unsigned long absColor);

private:
	unsigned long m_current_layer;

	void m_Flush(unsigned long layer);
};

class IGfxDeviceImpl: public IGfxDevice
{
protected:

public:
	IGfxDeviceImpl();
	~IGfxDeviceImpl();

	bool  BeginDraw(bool clear_flag);
	bool  EndDraw(void);
	bool  Flip(void);
	bool  GetSurface(IGfxSurface** pp_surface);
	bool  CreateSurface(const TSurfaceDesc& surface_desc, IGfxSurface** pp_surface);
    bool  CreateSurfaceFrom(const char* sz_file_name, IGfxSurface** pp_surface);
	bool  CreateSurfaceFrom(const unsigned char* sz_stream, unsigned long stream_size, IGfxSurface** pp_surface);
	bool  GetDesc(TDeviceDesc& desc);
    void  SetLayerDesc(TLayerDesc& layer_desc);
    void  GetLayerDesc(TLayerDesc& layer_desc);

	static IGfxDevice* p_gfx_impl;
	static int         ref_count;
	static IGfxDevice* GetInstance(void);
	static int         Release(void);

private:
	bool                       _m_is_valid;
	CGfxSurfaceImplBackBuffer* _m_p_back_buffer;

};

IGfxDevice* IGfxDeviceImpl::p_gfx_impl = 0;
int         IGfxDeviceImpl::ref_count  = 0;

IGfxDevice* IGfxDeviceImpl::GetInstance(void)
{
	if (p_gfx_impl == 0)
	{
    	p_gfx_impl = new IGfxDeviceImpl;
	}

	++ref_count;
	
	return p_gfx_impl;
}

int IGfxDeviceImpl::Release(void)
{
	if (--ref_count == 0)
		delete (IGfxDeviceImpl*)p_gfx_impl;

	return ref_count;
}

IGfxDevice* IGfxDevice::GetInstance(void)
{
	return IGfxDeviceImpl::GetInstance();
}

int IGfxDevice::Release(void)
{
	return IGfxDeviceImpl::Release();
}

////////////////////////////////////////////////////////////////////////////////
// common data

#include <vector>
#include "gfx3d_config.h"
#include "gfx3d_type.h"

struct TQuardCoord
{
	GLfixed data[4][2];

	TQuardCoord()
	{}
	TQuardCoord(int x1, int y1, int x2, int y2)
	{
		data[0][0] = x1;
		data[0][1] = y1;
		data[1][0] = x2;
		data[1][1] = y1;
		data[2][0] = x1;
		data[2][1] = y2;
		data[3][0] = x2;
		data[3][1] = y2;
	}
};

struct TQuardColor
{
	gfx3d::TFixedVec4 data[4];

	TQuardColor(unsigned long color)
	{
		gfx3d::TFixedVec4 color_component = gfx3d::_FXD_color2array(color);

		data[0] = color_component;
		data[1] = color_component;
		data[2] = color_component;
		data[3] = color_component;
	}
	TQuardColor(const unsigned long color[4])
	{
		data[0] = gfx3d::_FXD_color2array(color[0]);
		data[1] = gfx3d::_FXD_color2array(color[1]);
		data[2] = gfx3d::_FXD_color2array(color[2]);
		data[3] = gfx3d::_FXD_color2array(color[3]);
	}
};

static std::vector<TQuardCoord> s_vertex_list[MAX_LAYER_IN_SURFACE];
static std::vector<TQuardColor> s_color_list[MAX_LAYER_IN_SURFACE];
static std::vector<TQuardCoord> s_tex_coord_list[MAX_LAYER_IN_SURFACE];

static TLayerDesc s_layer_option =
{
	1,
	{
		TLayerDesc::LAYER_ATTRIB_OPAQUE,
	},
	TLayerDesc::OPTION_NONE
};

inline bool s_FillRect(unsigned int layer, unsigned long color, const TQuardCoord& vertices)
{
	static const TQuardCoord null_tex(0, 0, 1, 1);
	const TQuardColor colors(color);

	s_vertex_list[layer].push_back(vertices);
	s_color_list[layer].push_back(colors);
	s_tex_coord_list[layer].push_back(null_tex);

	return true;
}

inline bool s_BitBlt(unsigned int layer, const TQuardCoord& vertices, const IGfxSurface* src_surface, TFixed16 x_sour, TFixed16 y_sour, TFixed16 w_sour, TFixed16 h_sour, const unsigned long color[])
{
	static const TQuardColor white_color(0xFFFFFFFF);

	s_vertex_list[layer].push_back(vertices);

	if (color)
	{
		TQuardColor quard_color(color);
		s_color_list[layer].push_back(quard_color);
	}
	else
	{
		s_color_list[layer].push_back(white_color);
	}

	{
		const TSurfaceDesc& surface_desc = *((TSurfaceDesc*)src_surface->Handle());

		const int IMAGE_WIDTH  = surface_desc.buffer_desc.width;
		const int IMAGE_HEIGHT = surface_desc.buffer_desc.height;

		const TQuardCoord tex_coord(x_sour.m_data / IMAGE_WIDTH, y_sour.m_data / IMAGE_HEIGHT, (x_sour.m_data + w_sour.m_data) / IMAGE_WIDTH, (y_sour.m_data + h_sour.m_data) / IMAGE_HEIGHT);

		s_tex_coord_list[layer].push_back(tex_coord);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// imaplementation (CGfxSurfaceImplBackBuffer)

CGfxSurfaceImplBackBuffer::CGfxSurfaceImplBackBuffer()
: m_current_layer(0)
{
}

CGfxSurfaceImplBackBuffer::~CGfxSurfaceImplBackBuffer()
{
}

void  CGfxSurfaceImplBackBuffer::SetActiveLayer(unsigned long layer)
{
	if (m_current_layer != layer)
	{
		m_current_layer = (layer < s_layer_option.num_layer) ? layer : (s_layer_option.num_layer - 1);
	}
}

void  CGfxSurfaceImplBackBuffer::m_Flush(unsigned long layer)
{
	int num_of_index = s_vertex_list[layer].size();

	if (num_of_index <= 0)
		return;

	if (s_layer_option.layer_attrib[layer] & TLayerDesc::LAYER_ATTRIB_ALPHA_TEST)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
	}

	if (s_layer_option.layer_attrib[layer] & TLayerDesc::LAYER_ATTRIB_ALPHA_BLEND)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FIXED, 0, &*s_vertex_list[layer].begin());

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FIXED, 0, &*s_color_list[layer].begin());

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FIXED, 0, &*s_tex_coord_list[layer].begin());

	{
		// I am sure that this value is a constant.
		unsigned short* p_index_buffer = new unsigned short[num_of_index * 6];

		{
			unsigned short* p_index = p_index_buffer;
			unsigned short  index = 0;

			for (int i = 0; i < num_of_index; i++)
			{
				*p_index++ = index;
				*p_index++ = index+1;
				*p_index++ = index+2;
				*p_index++ = index+2;
				*p_index++ = index+1;
				*p_index++ = index+3;

				index += 4;
			}
		};

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glTranslatex(0, 0, -(Z_ORIGIN + int(layer)*2));

			if (0)
			{
				const  int FIXED_ONE = (1 << 16);
				static int angle = 90;

				glTranslatex(160*4, 120*4, 0);
				glRotatex((angle << 16), 0, 0, FIXED_ONE);
				glTranslatex(-160*4, -120*4, 0);
			}

			if (USE_PERSPECTIVE)
			{
				int scale = Z_SCALE << 16;
				glScalex(scale, scale, scale);
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 1);

			glDrawElements(GL_TRIANGLES, num_of_index*6, GL_UNSIGNED_SHORT, p_index_buffer);

			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		delete[] p_index_buffer;
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

bool  CGfxSurfaceImplBackBuffer::FillRect(unsigned long color, TFixed16 x, TFixed16 y, TFixed16 width, TFixed16 height)
{
	return s_FillRect(m_current_layer, color, TQuardCoord(x.m_data, y.m_data, (x.m_data + width.m_data), (y.m_data + height.m_data)));
}

bool  CGfxSurfaceImplBackBuffer::FillRect(unsigned long color, TPoint<TFixed16> point[])
{
	TQuardCoord vertices;

	vertices.data[0][0] = point[0].x.m_data;
	vertices.data[0][1] = point[0].y.m_data;
	vertices.data[1][0] = point[1].x.m_data;
	vertices.data[1][1] = point[1].y.m_data;
	vertices.data[2][0] = point[3].x.m_data;
	vertices.data[2][1] = point[3].y.m_data;
	vertices.data[3][0] = point[2].x.m_data;
	vertices.data[3][1] = point[2].y.m_data;

	return s_FillRect(m_current_layer, color, vertices);
}

bool  CGfxSurfaceImplBackBuffer::FillRect(unsigned long color, int x, int y, int width, int height)
{
	return s_FillRect(m_current_layer, color, TQuardCoord(x << 16, y << 16, (x+width) << 16, (y+height) << 16));
}

bool  CGfxSurfaceImplBackBuffer::FillRect(unsigned long color, TPoint<int> point[])
{
	TQuardCoord vertices;

	vertices.data[0][0] = point[0].x << 16;
	vertices.data[0][1] = point[0].y << 16;
	vertices.data[1][0] = point[1].x << 16;
	vertices.data[1][1] = point[1].y << 16;
	vertices.data[2][0] = point[3].x << 16;
	vertices.data[2][1] = point[3].y << 16;
	vertices.data[3][0] = point[2].x << 16;
	vertices.data[3][1] = point[2].y << 16;

	return s_FillRect(m_current_layer, color, vertices);
}

bool  CGfxSurfaceImplBackBuffer::DrawLine(unsigned long color, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2)
{
	TQuardCoord vertices;
	static const TQuardCoord null_tex(0, 0, 1, 1);

	unsigned long color_list[4] = { color, 0, color, 0 };
	const TQuardColor colors(color_list);

	double LINE_THICK = 1.3;
	double RADIAN_90  = 3.141592 / 2.0;
	double angle = atan2(double(y2.m_data - y1.m_data), double(x2.m_data - x1.m_data));
	
	int x[4] =
	{
		x1.m_data + int(LINE_THICK * cos(angle - RADIAN_90) * 65536.0),
		x2.m_data,// + int(LINE_THICK * cos(angle - RADIAN_90) * 65536.0),
		x1.m_data + int(LINE_THICK * cos(angle + RADIAN_90) * 65536.0),
		x2.m_data,// + int(LINE_THICK * cos(angle + RADIAN_90) * 65536.0),
	};
	int y[4] =
	{
		y1.m_data - int(LINE_THICK * sin(angle + RADIAN_90) * 65536.0),
		y2.m_data,// - int(LINE_THICK * sin(angle + RADIAN_90) * 65536.0),
		y1.m_data - int(LINE_THICK * sin(angle - RADIAN_90) * 65536.0),
		y2.m_data,// - int(LINE_THICK * sin(angle - RADIAN_90) * 65536.0),
	};

	vertices.data[0][0] = x[0];
	vertices.data[0][1] = y[0];
	vertices.data[1][0] = x[1];
	vertices.data[1][1] = y[1];
	vertices.data[2][0] = x[2];
	vertices.data[2][1] = y[2];
	vertices.data[3][0] = x[3];
	vertices.data[3][1] = y[3];


	s_vertex_list[m_current_layer].push_back(vertices);
	s_color_list[m_current_layer].push_back(colors);
	s_tex_coord_list[m_current_layer].push_back(null_tex);

	return true;
}

bool  CGfxSurfaceImplBackBuffer::DrawGradLine(unsigned long color1, unsigned long color2, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2)
{
	TQuardCoord vertices;
	static const TQuardCoord null_tex(0, 0, 1, 1);

	unsigned long color_list[4] = { color1, color2, color1, color2 };
	const TQuardColor colors(color_list);

	double LINE_THICK = 1.0;
	double RADIAN_90  = 3.141592 / 2.0;
	double angle = atan2(double(y2.m_data - y1.m_data), double(x2.m_data - x1.m_data));
	
	int x[4] =
	{
		x1.m_data + int(LINE_THICK * cos(angle - RADIAN_90) * 65536.0),
		x2.m_data + int(LINE_THICK * cos(angle - RADIAN_90) * 65536.0),
		x1.m_data + int(LINE_THICK * cos(angle + RADIAN_90) * 65536.0),
		x2.m_data + int(LINE_THICK * cos(angle + RADIAN_90) * 65536.0),
	};
	int y[4] =
	{
		y1.m_data - int(LINE_THICK * sin(angle + RADIAN_90) * 65536.0),
		y2.m_data - int(LINE_THICK * sin(angle + RADIAN_90) * 65536.0),
		y1.m_data - int(LINE_THICK * sin(angle - RADIAN_90) * 65536.0),
		y2.m_data - int(LINE_THICK * sin(angle - RADIAN_90) * 65536.0),
	};

	vertices.data[0][0] = x[0];
	vertices.data[0][1] = y[0];
	vertices.data[1][0] = x[1];
	vertices.data[1][1] = y[1];
	vertices.data[2][0] = x[2];
	vertices.data[2][1] = y[2];
	vertices.data[3][0] = x[3];
	vertices.data[3][1] = y[3];


	s_vertex_list[m_current_layer].push_back(vertices);
	s_color_list[m_current_layer].push_back(colors);
	s_tex_coord_list[m_current_layer].push_back(null_tex);

	return true;
}

bool  CGfxSurfaceImplBackBuffer::BitBlt(TFixed16 x, TFixed16 y, const IGfxSurface* src_surface, TFixed16 x_sour, TFixed16 y_sour, TFixed16 w_sour, TFixed16 h_sour, const unsigned long color[])
{
	const TQuardCoord vertices(x.m_data, y.m_data, (x.m_data + w_sour.m_data), (y.m_data + h_sour.m_data));
	return s_BitBlt(m_current_layer, vertices, src_surface, x_sour, y_sour, w_sour, h_sour, color);
}

bool  CGfxSurfaceImplBackBuffer::BitBlt(int x, int y, const IGfxSurface* src_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[])
{
	const TQuardCoord vertices(x << 16, y << 16, (x + w_sour) << 16, (y + h_sour) << 16);
	return s_BitBlt(m_current_layer, vertices, src_surface, TFixed16(x_sour), TFixed16(y_sour), TFixed16(w_sour), TFixed16(h_sour), color);
}

bool  CGfxSurfaceImplBackBuffer::BitBlt(int x, int y, const TSpriteDesc& sprite_desc, const unsigned long color[])
{
	const TQuardCoord vertices(x << 16, y << 16, (x + sprite_desc.w_sour) << 16, (y + sprite_desc.h_sour) << 16);
	return s_BitBlt(m_current_layer, vertices, sprite_desc.ref_surface, TFixed16(sprite_desc.x_sour), TFixed16(sprite_desc.y_sour), TFixed16(sprite_desc.w_sour), TFixed16(sprite_desc.h_sour), color);
}

bool  CGfxSurfaceImplBackBuffer::StretchBlt(TFixed16 x_dest, TFixed16 y_dest, TFixed16 w_dest, TFixed16 h_dest, const IGfxSurface* src_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[])
{
	const TQuardCoord vertices(x_dest.m_data, y_dest.m_data, x_dest.m_data + w_dest.m_data, y_dest.m_data + h_dest.m_data);
	return s_BitBlt(m_current_layer, vertices, src_surface, TFixed16(x_sour), TFixed16(y_sour), TFixed16(w_sour), TFixed16(h_sour), color);
}

bool  CGfxSurfaceImplBackBuffer::StretchBlt(int x_dest, int y_dest, int w_dest, int h_dest, const IGfxSurface* src_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[])
{
	const TQuardCoord vertices(x_dest << 16, y_dest << 16, (x_dest + w_dest) << 16, (y_dest + h_dest) << 16);
	return s_BitBlt(m_current_layer, vertices, src_surface, TFixed16(x_sour), TFixed16(y_sour), TFixed16(w_sour), TFixed16(h_sour), color);
}

bool  CGfxSurfaceImplBackBuffer::StretchBlt(int x_dest, int y_dest, int w_dest, int h_dest, const TSpriteDesc& sprite_desc, const unsigned long color[])
{
	const TQuardCoord vertices(x_dest << 16, y_dest << 16, (x_dest + w_dest) << 16, (y_dest + h_dest) << 16);
	return s_BitBlt(m_current_layer, vertices, sprite_desc.ref_surface, TFixed16(sprite_desc.x_sour), TFixed16(sprite_desc.y_sour), TFixed16(sprite_desc.w_sour), TFixed16(sprite_desc.h_sour), color);
}

void* CGfxSurfaceImplBackBuffer::Handle(void) const
{
	static bool         s_is_first = true;
	static TSurfaceDesc s_surface_desc;

	if (s_is_first)
	{
		memset(&s_surface_desc, 0, sizeof(s_surface_desc));
		//?? 현재는 이것만 선언
		s_surface_desc.buffer_desc.width  = SCREEN_WIDTH;
		s_surface_desc.buffer_desc.height = SCREEN_HEIGHT;
		s_is_first = false;
	}

	return (void*)&s_surface_desc;
}

unsigned long CGfxSurfaceImplBackBuffer::Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
	typedef unsigned long UC32;
	return this->Color(UC32(a) << 24 | UC32(r) << 16 | UC32(g) << 8 | UC32(b));
}

unsigned long CGfxSurfaceImplBackBuffer::Color(unsigned long absColor)
{
	return absColor;
}

////////////////////////////////////////////////////////////////////////////////
// imaplementation (IGfxDeviceImpl)

namespace avej_lite
{
	namespace gfx3d
	{
		bool InitOpenGL(unsigned int width, unsigned int height, unsigned int depth);
		void DoneOpenGL(void);
		void SwapBuffers(void);
		void ProcessMessage(unsigned long delay_time);
	}
}

#define CHECK_VALIDITY   if (!this->_m_is_valid) return false

IGfxDeviceImpl::IGfxDeviceImpl()
{
	this->_m_is_valid = gfx3d::InitOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

	if (this->_m_is_valid)
	{
		glClearColorx(0, 0, 0, 0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_FOG); 

		this->_m_p_back_buffer = new CGfxSurfaceImplBackBuffer;

		struct TViewPort
		{
			GLint x, y, width, height;
		} view_port;

		glGetIntegerv(GL_VIEWPORT, (GLint*)&view_port);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

#if (TARGET_DEVICE == TARGET_BADA)

		{
			int x      = 800 - g_nativeWindowWidth;
			int y      = 480 - g_nativeWindowHeight;
			int width  = g_nativeWindowWidth;
			int height = g_nativeWindowHeight;

			glViewport(x, y, width, height);

			view_port.width  = width;
			view_port.height = height;
		}

		if (USE_PERSPECTIVE)
		{
#if 1 // shit fighter
			int width  = (view_port.x+view_port.width)*240/view_port.height;
			int height = (view_port.y+view_port.height)*240/view_port.height;
			glFrustumx(view_port.x << 16, width << 16, height << 16, view_port.y << 16, Z_NEAR, Z_FAR);
#else
			int width  = view_port.width;
			int height = view_port.height;
			glFrustumx(view_port.x << 16, width << 16, height << 16, view_port.y << 16, Z_NEAR, Z_FAR);
#endif
		}
		else
		{
			glOrthox(view_port.x << 16, (view_port.x+view_port.width) << 16, (view_port.y+view_port.height) << 16, view_port.y << 16, Z_NEAR, Z_FAR);
		}

#else
		glViewport(view_port.x, view_port.y, view_port.width, view_port.height);

		if (USE_PERSPECTIVE)
			glFrustumx(view_port.x << 16, (view_port.x+view_port.width) << 16, (view_port.y+view_port.height) << 16, view_port.y << 16, Z_NEAR, Z_FAR);
		else
			glOrthox(view_port.x << 16, (view_port.x+view_port.width) << 16, (view_port.y+view_port.height) << 16, view_port.y << 16, Z_NEAR, Z_FAR);
#endif

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	};
}

IGfxDeviceImpl::~IGfxDeviceImpl()
{
	if (this->_m_is_valid)
	{
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		if (this->_m_p_back_buffer)
			delete this->_m_p_back_buffer;

		gfx3d::DoneOpenGL();
	}
}

bool  IGfxDeviceImpl::BeginDraw(bool clear_flag)
{
	CHECK_VALIDITY;

	for (unsigned long i = 0; i < s_layer_option.num_layer; i++)
	{
		s_vertex_list[i].clear();
		s_color_list[i].clear();
		s_tex_coord_list[i].clear();
	}

	this->_m_p_back_buffer->SetActiveLayer(0);

	if (clear_flag)
	{
		if (s_layer_option.option_flag & TLayerDesc::OPTION_DEPTH_TEST)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else
			glClear(GL_COLOR_BUFFER_BIT);
	}

	return true;
}

bool  IGfxDeviceImpl::EndDraw(void)
{
	for (unsigned long i = 0; i < s_layer_option.num_layer; i++)
	{
		this->_m_p_back_buffer->m_Flush(i);
	}

	return true;
}

bool  IGfxDeviceImpl::Flip(void)
{
	CHECK_VALIDITY;

	gfx3d::SwapBuffers();

	return true;
}

bool  IGfxDeviceImpl::GetSurface(IGfxSurface** pp_surface)
{
	CHECK_VALIDITY;

	if (pp_surface == 0)
		return false;

	*pp_surface = this->_m_p_back_buffer;

	return (*pp_surface != 0);
}

namespace avej_lite
{
	namespace util
	{
		bool DecodeSMgal(TPixelFormat pixel_format, const unsigned char* p_stream, unsigned int stream_size, TSurfaceDesc& out_surface_desc);
		bool DecodeSMgal(TPixelFormat pixel_format, const char* sz_file_name, TSurfaceDesc& out_surface_desc);
	}
}

static unsigned long s_CreateTexture(int width, int height, unsigned short* p_buffer, bool use_alpha_channel)
{
	GLint  format = GL_RGBA;
	GLenum pixel_type = (use_alpha_channel) ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_SHORT_5_5_5_1;

	GLuint handle;

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, pixel_type, p_buffer);

#if 0
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_REPEAT
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // GL_MODULATE, GL_DECAL, GL_BLEND, or GL_REPLACE

	return handle;
}

bool  IGfxDeviceImpl::CreateSurface(const TSurfaceDesc& ref_surface_desc, IGfxSurface** pp_surface)
{
	CHECK_VALIDITY;

	if (pp_surface == 0)
		return false;

	TSurfaceDesc surface_desc = ref_surface_desc;

	// the temporary solution for FillRect();
	*((unsigned short*)surface_desc.lock_desc.p_buffer) = 0xFFFF;

	surface_desc.hw_handle = s_CreateTexture(surface_desc.buffer_desc.width, surface_desc.buffer_desc.height, (unsigned short*)surface_desc.lock_desc.p_buffer, true);

	*pp_surface = new CGfxSurfaceImplHW(surface_desc);

	return true;
}

bool  IGfxDeviceImpl::CreateSurfaceFrom(const char* sz_file_name, IGfxSurface** pp_surface)
{
	CHECK_VALIDITY;

	if (pp_surface == 0)
		return false;

	TSurfaceDesc  surface_desc;

	if (!util::DecodeSMgal(PIXELFORMAT_RGBA4444, sz_file_name, surface_desc))
		return false;

	bool result = this->CreateSurface(surface_desc, pp_surface);

	delete[] (unsigned char*)surface_desc.lock_desc.p_buffer;

	return result;
}

bool  IGfxDeviceImpl::CreateSurfaceFrom(const unsigned char* sz_stream, unsigned long stream_size, IGfxSurface** pp_surface)
{
	CHECK_VALIDITY;

	if (pp_surface == 0)
		return false;

	TSurfaceDesc  surface_desc;

	if (!util::DecodeSMgal(PIXELFORMAT_RGBA4444, sz_stream, stream_size, surface_desc))
		return false;

	bool result = this->CreateSurface(surface_desc, pp_surface);

	delete[] (unsigned char*)surface_desc.lock_desc.p_buffer;

	return result;
}

bool  IGfxDeviceImpl::GetDesc(TDeviceDesc& desc)
{
	CHECK_VALIDITY;

	desc.width  = SCREEN_WIDTH;
	desc.height = SCREEN_HEIGHT;
	desc.pitch  = SCREEN_WIDTH * SCREEN_DEPTH / 8;
	desc.depth  = SCREEN_DEPTH;
	desc.format = 0;

	return false;
}

void  IGfxDeviceImpl::SetLayerDesc(TLayerDesc& layer_desc)
{
	s_layer_option = layer_desc;

	// option_flag에 대한 default value
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_FOG); 

	// option_flag에 해당하는 것은 모든 layer에 공통 적용되는 것이므로 여기서 초기화를 해야 한다.
	if (s_layer_option.option_flag & TLayerDesc::OPTION_DEPTH_TEST)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
}

void  IGfxDeviceImpl::GetLayerDesc(TLayerDesc& layer_desc)
{
	layer_desc = s_layer_option;
}
