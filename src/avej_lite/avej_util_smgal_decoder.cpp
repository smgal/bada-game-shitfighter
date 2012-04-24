
#include "avej_pixel_format.h"
#include "avej_base_type.h"
#include "avej_util.h"

#include <string.h>
#include <memory>
#include <cassert>

typedef unsigned long TPixel;

namespace smgal_decoder
{
	struct TImage32Bits
	{
		long            width;
		long            height;
		long            bytes_per_line;
		unsigned long   depth;
		unsigned char*  p_bitmap;

		TImage32Bits()
		:	width(0), height(0), bytes_per_line(0), depth(0), p_bitmap(NULL) {};
	};

	#define IMAGETYPE_MODE_MASK  0x07
	#define IMAGETYPE_RLE_MASK   0x08

	#define IMAGETYPE_COLORMAP   0x01
	#define IMAGETYPE_TRUECOLOR  0x02
	#define IMAGETYPE_BLACKWHITE 0x03

	struct TSMgalHeader
	{
		unsigned char  id_length;
		unsigned char  color_map_type;
		unsigned char  image_type;
		unsigned short color_map_start;
		unsigned short color_map_length;
		unsigned char  color_map_depth;
		unsigned short x_offset;
		unsigned short y_offset;
		unsigned short width;
		unsigned short height;
		unsigned char  pixel_depth;
		unsigned char  image_descriptor;

	};

} // smgal_decoder

using namespace avej_lite;

////////////////////

template <typename T>
void s_ReadFieldLE(const unsigned char*& p_stream, T& read)
{
	read = *p_stream++;
}

template <>
void s_ReadFieldLE<unsigned short>(const unsigned char*& p_stream, unsigned short& read)
{
	read  = (unsigned short)(*p_stream++);
	read |= (unsigned short)(*p_stream++) << 8;
}

template <>
void s_ReadFieldLE<unsigned long>(const unsigned char*& p_stream, unsigned long& read)
{
	read  = (unsigned long)(*p_stream++);
	read |= (unsigned long)(*p_stream++) <<  8;
	read |= (unsigned long)(*p_stream++) << 16;
	read |= (unsigned long)(*p_stream++) << 24;
}

#define CHECK_FILE_BROKEN(_begin, _end, required_bytes) if (int(_end - _begin) < required_bytes) return false;

bool DecodeSMgal_Native(const unsigned char* p_stream, unsigned int stream_size, smgal_decoder::TImage32Bits& image)
{
	if (p_stream == 0)
		return false;

	if (stream_size == 0)
		return false;

	const unsigned char* p_stream_begin = p_stream;
	const unsigned char* p_stream_end   = p_stream + stream_size;

	smgal_decoder::TSMgalHeader smgal_header;
	unsigned long               palette[256];

	CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, 18);

	s_ReadFieldLE(p_stream_begin, smgal_header.id_length);
	s_ReadFieldLE(p_stream_begin, smgal_header.color_map_type);
	s_ReadFieldLE(p_stream_begin, smgal_header.image_type);
	s_ReadFieldLE(p_stream_begin, smgal_header.color_map_start);
	s_ReadFieldLE(p_stream_begin, smgal_header.color_map_length);
	s_ReadFieldLE(p_stream_begin, smgal_header.color_map_depth);
	s_ReadFieldLE(p_stream_begin, smgal_header.x_offset);
	s_ReadFieldLE(p_stream_begin, smgal_header.y_offset);
	s_ReadFieldLE(p_stream_begin, smgal_header.width);
	s_ReadFieldLE(p_stream_begin, smgal_header.height);
	s_ReadFieldLE(p_stream_begin, smgal_header.pixel_depth);
	s_ReadFieldLE(p_stream_begin, smgal_header.image_descriptor);

	// Image Identification Field 지원 안함
	if (smgal_header.id_length != 0)
		return false;

	bool is_color_mapped = (smgal_header.image_type & IMAGETYPE_MODE_MASK) == IMAGETYPE_COLORMAP;
	bool is_RGB_color    = (smgal_header.image_type & IMAGETYPE_MODE_MASK) == IMAGETYPE_TRUECOLOR;
	bool is_compressed   = (smgal_header.image_type & IMAGETYPE_RLE_MASK) > 0;
	bool is_flipped      = (smgal_header.image_descriptor & 0x20) > 0;

	bool is_decodable    = (smgal_header.pixel_depth == 32 && is_RGB_color)
	                    || (smgal_header.pixel_depth == 8 && is_color_mapped && !is_compressed);

	if (!is_decodable)
		return false;

	smgal_header.image_descriptor &= 0x0F;

	image.width          = smgal_header.width;
	image.height         = smgal_header.height;
	image.bytes_per_line = smgal_header.width * 4;
	image.depth          = 32;
	image.p_bitmap       = new unsigned char[image.bytes_per_line * image.height];

	if (image.p_bitmap == NULL)
		return false;

	std::auto_ptr<unsigned char> mem_holder(image.p_bitmap);

	switch (smgal_header.pixel_depth)
	{
	case  8:
		{
			// palette 검증
			bool is_valid_palette = (smgal_header.color_map_start == 0) && (smgal_header.color_map_length <= 256) && ((smgal_header.color_map_depth == 24) || (smgal_header.color_map_depth == 32));

			if (!is_valid_palette)
				return false;

			CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, smgal_header.color_map_length * smgal_header.color_map_depth / 8);

			if (smgal_header.color_map_depth == 32)
			{
				//  b, g, r, attribute info
				assert(false);
			}
			else if (smgal_header.color_map_depth == 24)
			{
				//  b, g, r
				for (int i = 0; i < smgal_header.color_map_length; i++)
				{
					unsigned long b = *p_stream_begin++;
					unsigned long g = *p_stream_begin++;
					unsigned long r = *p_stream_begin++;
					unsigned long a = 0xFF;

					palette[smgal_header.color_map_start+i] = a << 24 | r << 16 | g << 8 | b;
				}
			}

			//?? 임시. 자동 컬러키 설정
			{
//				palette[*(p_stream_begin+(smgal_header.height-1)*smgal_header.width)] &= 0x00FFFFFF;
			}

			{
				int            w = smgal_header.width;
				int            h = smgal_header.height;
				unsigned long* p_dest_32;
				unsigned char* p_dest = image.p_bitmap;
				signed char    code;
				unsigned long  color;

				//?? 크기 비교를 한 번 해야 함

				if (is_compressed)
				{
					while (--h >= 0)
					{
						p_dest_32 = (is_flipped) ? (unsigned long*)(p_dest + image.bytes_per_line * (smgal_header.height-h-1)) : (unsigned long*)(p_dest + image.bytes_per_line * h);

						int count = w;
						while (count > 0)
						{
							CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, 1);
							s_ReadFieldLE(p_stream_begin, code);

							if (code & 0x80)
							{
								int repeat = ((int)code & 0x7F);
								count -= (++repeat);

								CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, 1);
								color = palette[*p_stream_begin++];

								while (--repeat >= 0)
								{
									*p_dest_32++ = color;
								}
							}
							else
							{
								int repeat = (int)code;
								count -= (++repeat);

								CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, 1 * repeat);

								while (--repeat >= 0)
								{
									*p_dest_32++ = palette[*p_stream_begin++];
								}
							}
						}

					}
				}
				else
				{
					while (--h >= 0)
					{
						p_dest_32 = (is_flipped) ? ((unsigned long*)p_dest + w * (smgal_header.height-h-1)) : ((unsigned long*)p_dest + h * w);

						for (int i = 0; i < w; i++)
						{
							*p_dest_32++ = palette[*p_stream_begin++];
						}
					}
				}
			}
		}
		break;
	case 32:
		{
			int            w = smgal_header.width;
			int            h = smgal_header.height;
			signed char    code;
			unsigned long  color;
			unsigned long* p_dest_32;
			unsigned char* p_dest = image.p_bitmap;

			if (is_compressed)
			{
				while (--h >= 0)
				{
					p_dest_32 = (is_flipped) ? (unsigned long*)(p_dest + image.bytes_per_line * (smgal_header.height-h-1)) : (unsigned long*)(p_dest + image.bytes_per_line * h);

					int count = w;
					while (count > 0)
					{
						CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, 1);
						s_ReadFieldLE(p_stream_begin, code);

						if (code & 0x80)
						{
							int repeat = ((int)code & 0x7F);
							count -= (++repeat);

							CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, 4);
							s_ReadFieldLE(p_stream_begin, color);

							while (--repeat >= 0)
							{
								*p_dest_32++ = color;
							}
						}
						else
						{
							int repeat = (int)code;
							count -= (++repeat);

							CHECK_FILE_BROKEN(p_stream_begin, p_stream_end, 4 * repeat);

							while (--repeat >= 0)
							{
								s_ReadFieldLE(p_stream_begin, color);
								*p_dest_32++ = color;
							}
						}
					}
				}
			}
			else
			{
				while (--h >= 0)
				{
					p_dest_32 = (unsigned long*)p_dest + h * w;

					//?? endian 고려 안되어 있음
					memcpy(&p_dest_32, p_stream_begin, w * 4);
					p_stream_begin += w * 4;
				}
			}
		}
		break;
	default:
		assert(false);
		return false;
	}

	mem_holder.release();

	return true;
}

template <avej_lite::TPixelFormat pixel_format>
bool _DecodeSMgal(const unsigned char* p_stream, unsigned int stream_size, avej_lite::TSurfaceDesc& out_surface_desc)
{

	smgal_decoder::TImage32Bits image_32bits;

	if (!DecodeSMgal_Native(p_stream, stream_size, image_32bits))
		return false;

	typedef typename TPixelInfo<pixel_format>::TPixel TPixel;

	TPixel* p_dest = (TPixel*)new unsigned char[image_32bits.width * image_32bits.height * sizeof(TPixel)];

	if (p_dest == NULL)
	{
		delete[] image_32bits.p_bitmap;
		return false;
	}

	ConvertColorSpace<TStreamPixelFormat(TToStreamFormat<pixel_format>::stream_format), SOURCE_ARGB8888>(p_dest, (unsigned long*)image_32bits.p_bitmap, image_32bits.width * image_32bits.height);

	memset(&out_surface_desc, 0, sizeof(out_surface_desc));

	out_surface_desc.buffer_desc.pixel_format  = pixel_format;
	out_surface_desc.buffer_desc.width         = image_32bits.width;
	out_surface_desc.buffer_desc.height        = image_32bits.height;
	out_surface_desc.buffer_desc.depth         = sizeof(TPixel) * 8;
	out_surface_desc.buffer_desc.has_color_key = false;
	out_surface_desc.buffer_desc.color_key     = 0;
	out_surface_desc.lock_desc.bytes_per_line  = image_32bits.width * sizeof(TPixel);
	out_surface_desc.lock_desc.p_buffer        = (void*)p_dest;
	out_surface_desc.hw_handle                 = 0;
	out_surface_desc.has_ownership             = true;

	delete[] image_32bits.p_bitmap;

	return true;
}

namespace avej_lite
{
	namespace util
	{
		bool DecodeSMgal(avej_lite::TPixelFormat pixel_format, const unsigned char* p_stream, unsigned int stream_size, avej_lite::TSurfaceDesc& out_surface_desc)
		{
			switch (pixel_format)
			{
			case PIXELFORMAT_RGBA4444:
				return _DecodeSMgal<PIXELFORMAT_RGBA4444>(p_stream, stream_size, out_surface_desc);
			default:
				assert(false);
				return false;
			};
		}

		bool DecodeSMgal(avej_lite::TPixelFormat pixel_format, const char* sz_file_name, avej_lite::TSurfaceDesc& out_surface_desc)
		{
			avej_lite::util::CFileReadStream file(sz_file_name);

			if (!file.IsValid())
				return false;

			unsigned int file_length = file.GetSize();
			
			std::auto_ptr<unsigned char> buffer(new unsigned char[file_length]);
			file.Read((char*)buffer.get(), file_length);

			return DecodeSMgal(pixel_format, buffer.get(), file_length, out_surface_desc);
		}
	}
}
