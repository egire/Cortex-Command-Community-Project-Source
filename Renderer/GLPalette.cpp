#include "GLPalette.h"
#include "GL/glew.h"
#include <SDL2/SDL_pixels.h>
namespace RTE {
	Palette::Palette(const std::array<glm::u8vec4, PALETTESIZE> &data) :
	    m_Palette(data) {
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_1D, m_Texture);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, PALETTESIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Palette.data());
		glBindTexture(GL_TEXTURE_1D, 0);

		m_SDLPalette = std::unique_ptr<SDL_Palette, sdl_palette_deleter>(SDL_AllocPalette(PALETTESIZE));
		SDL_SetPaletteColors(m_SDLPalette.get(), static_cast<SDL_Color *>(static_cast<void *>(m_Palette.data())), 0, PALETTESIZE);
	}

	Palette::~Palette() {}

	SDL_Palette *Palette::GetAsPalette() {
		return m_SDLPalette.get();
	}

	void Palette::sdl_palette_deleter::operator()(SDL_Palette *p) {
		SDL_FreePalette(p);
	}

} // namespace RTE