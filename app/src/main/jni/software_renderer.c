#include "software_renderer.h"

#include <android/bitmap.h>
#include <android/log.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#define  LOG_TAG    "SoftRenderer"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static u32 frame_count = 0;
static r64 last_second_time = 0;

static r64 time_ms ()
{
	struct timespec res;
	clock_gettime(CLOCK_REALTIME, &res);
	r64 ret = 1000.0 * res.tv_sec + (r64)res.tv_nsec / 1e6;
	return ret;
}

JNIEXPORT void JNICALL
Java_com_orderinfinity_a3dsoftwarerenderer_SoftwareRendererView_init (JNIEnv *env, jclass type, jobject bitmap)
{
	last_second_time = time_ms();

	RenderBuffer render_buffer = {};
	open_render_buffer(&render_buffer, env, bitmap);
	clear(&render_buffer, 0,0,255,255);
	close_render_buffer(&render_buffer, env, bitmap);
}

JNIEXPORT void JNICALL
Java_com_orderinfinity_a3dsoftwarerenderer_SoftwareRendererView_render (JNIEnv *env, jclass type, jobject bitmap)
{
	//Contando Frames
	r64 frame_start = time_ms();
	if(frame_start - last_second_time >= 1000.0)
	{
		LOGV("FPS: %d", frame_count);
		frame_count = 0;
		last_second_time = frame_start;
	}
	frame_count++;

//	open_render_buffer(render_buffer, env, bitmap);

//	u32 i,j;
//	for (i = 100; i < 500; ++i)
//	{
//		for (j = 100; j < 500; ++j)
//		{
//			set_pixel(render_buffer, i,j, 255,0,0,255);
//		}
//	}

//	close_render_buffer(render_buffer, env, bitmap);
}



inline void
set_pixel (RenderBuffer *rb, u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
{
	if(!rb->open) return;

	u32 i = (x + y*rb->width) * sizeof(u32);
	((u32*)((u8*)rb->pixels + i))[0] = (((u32)a) << 24) | (((u32)b) << 16) | (((u32)g) << 8) | r;
}

inline void
clear (RenderBuffer *rb, u8 r, u8 g, u8 b, u8 a)
{
//	if(!rb->open) return;

	void *pixels = rb->pixels;

	u32 pixel = (((u32)a) << 24) | (((u32)b) << 16) | (((u32)g) << 8) | r;
	u32 y;
	for (y = 0; y < rb->height; y++)
	{
		u32 *line = pixels;
		int x;
		for (x = 0; x < rb->width; x+=2)
		{
			((u64*)line)[0] = (((u64)pixel) << 32) | ((u64)pixel);
			line += 2;
		}
		pixels = (u8*)pixels + rb->stride;
	}

}

void open_render_buffer (RenderBuffer *rb, JNIEnv *env, jobject bitmap)
{
	if(rb->open) return;

	AndroidBitmapInfo bmp_info;

	if (AndroidBitmap_getInfo(env, bitmap, &bmp_info) < 0)
	{
		LOGE("AndroidBitmap_getInfo() failed!");
		return;
	}

	if (bmp_info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
		LOGE("Bitmap format is not RGBA_8888!");
		return;
	}

	if (AndroidBitmap_lockPixels(env, bitmap, rb->pixels) < 0)
		LOGE("AndroidBitmap_lockPixels() failed!");

    rb->width = bmp_info.width;
	rb->height = bmp_info.height;
	rb->stride = bmp_info.stride;
	LOGV("W: %d, H: %d, S: %d", rb->width,rb->height,bmp_info.stride);

	rb->open = 1;
}

void close_render_buffer (RenderBuffer *rb, JNIEnv *env, jobject bitmap)
{
	if(!rb->open) return;
	AndroidBitmap_unlockPixels(env, bitmap);
	rb->open = 0;
}
