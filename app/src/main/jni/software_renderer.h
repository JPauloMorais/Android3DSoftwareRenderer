///////////
//// J. Paulo G.M.A.
//// 3D Software Renderer [2016]
//////////

#ifndef INC_3D_SOFTWARE_RENDERER_SOFTWARE_RENDERER_H
#define INC_3D_SOFTWARE_RENDERER_SOFTWARE_RENDERER_H

#include <android/bitmap.h>
#include <stdint.h>
#include <jni.h>

typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;
typedef float  r32;
typedef double r64;

typedef struct
{
	u8 open;

	u32 width;
    u32 height;
	u32 stride;
	void *pixels;
} RenderBuffer;

typedef struct
{
	r32 speed;
	r32 spread;
	r32 star_positions[4096 * 3];
} StarFieldData;

inline void open_render_buffer (RenderBuffer *rb, JNIEnv *env, jobject bitmap);
inline void close_render_buffer (RenderBuffer *rb, JNIEnv *env, jobject bitmap);

inline void set_pixel(RenderBuffer *rb, u32 x, u32 y, u8 r, u8 g, u8 b, u8 a);
inline void clear (RenderBuffer *rb, u8 r, u8 g, u8 b, u8 a);

void render_star_field(RenderBuffer *rb, StarFieldData sfdata, r32 delta);

#endif //INC_3D_SOFTWARE_RENDERER_SOFTWARE_RENDERER_H
