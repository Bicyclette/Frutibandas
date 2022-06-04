#include "mouse.hpp"
#include "stb_image.h"

Mouse::Mouse(std::string img_target) :
	m_normal(nullptr),
	m_hover(nullptr),
	m_target(nullptr)
{
	m_normal = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_hover = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	create_target_cursor(img_target);
	use_normal();
}

Mouse::~Mouse()
{
	if (m_normal)
	{
		SDL_FreeCursor(m_normal);
	}
	if (m_hover)
	{
		SDL_FreeCursor(m_hover);
	}
	if (m_target)
	{
		SDL_FreeCursor(m_target);
	}
	if (m_target_surface)
	{
		SDL_FreeSurface(m_target_surface);
	}
}

void Mouse::create_target_cursor(std::string img_target)
{
	int req_format = STBI_rgb_alpha;
	int width, height, orig_format;
	unsigned char* data = stbi_load(img_target.c_str(), &width, &height, &orig_format, req_format);
	if (data == NULL) {
		SDL_Log("Loading image failed: %s", stbi_failure_reason());
		exit(1);
	}

	// Set up the pixel format color masks for RGB(A) byte arrays.
	// Only STBI_rgb (3) and STBI_rgb_alpha (4) are supported here!
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int shift = (req_format == STBI_rgb) ? 8 : 0;
	rmask = 0xff000000 >> shift;
	gmask = 0x00ff0000 >> shift;
	bmask = 0x0000ff00 >> shift;
	amask = 0x000000ff >> shift;
#else // little endian, like x86
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = (req_format == STBI_rgb) ? 0 : 0xff000000;
#endif

	int depth, pitch;
	if (req_format == STBI_rgb) {
		depth = 24;
		pitch = 3 * width; // 3 bytes per pixel * pixels per row
	}
	else { // STBI_rgb_alpha (RGBA)
		depth = 32;
		pitch = 4 * width;
	}

	m_target_surface = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch, rmask, gmask, bmask, amask);

	if (m_target_surface == NULL) {
		SDL_Log("Creating surface failed: %s", SDL_GetError());
		stbi_image_free(data);
		exit(1);
	}

	m_target = SDL_CreateColorCursor(m_target_surface, 25, 25);

	stbi_image_free(data);
}