#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "texture_manager.hpp"

class GameObject {
	public:
		GameObject(std::string textureID, SDL_Rect textureSrc, uint32_t frames) : textureID(textureID), textureSrc(textureSrc), frames(frames) {
			position.w = textureSrc.w;
			position.h = textureSrc.h;
		}

		void draw(SDL_Renderer* renderer, uint32_t frameIndex) {
			TextureManager::Instance(renderer)->drawSpriteSheet(textureID, &textureSrc, &position, frames, frameIndex);
		}

		void update() {
			position.x++;
		};

	protected:
		std::string textureID;
		SDL_Rect textureSrc;
		uint32_t frames;

		SDL_Rect position{};
};
