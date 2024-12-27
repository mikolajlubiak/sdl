#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <filesystem>

class TextureManager {
	public:
		static TextureManager* Instance(SDL_Renderer* renderer) {
			static TextureManager* textureManager = new TextureManager(renderer);
			return textureManager;
		}

		bool createTexture(const std::filesystem::path& path, const std::string& id) {
			SDL_Surface* tempSurface = IMG_Load(path.c_str());
			if (!tempSurface) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: %s", path.c_str());
				return false;
			}

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
			SDL_FreeSurface(tempSurface);
			if (!texture) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", id.c_str());
				return false;
			}

			textures.emplace(id, texture);

			return true;
		}

		SDL_Texture* getTexture(const std::string& id) const {
			return textures.at(id);
		}

		SDL_Rect createTextureRect(const std::string& id) {
			SDL_Rect rect{};
			SDL_QueryTexture(getTexture(id), nullptr, nullptr, &rect.w, &rect.h);

			return rect;
		}

		void drawTexture(const std::string& id, SDL_Rect* src = nullptr, SDL_Rect* dest = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
			SDL_RenderCopyEx(renderer, getTexture(id), src, dest, 0, 0, flip);
		}

		void drawSpriteSheet(const std::string& id, SDL_Rect* frame, SDL_Rect* dest, uint32_t frames, uint32_t frameIndex, SDL_RendererFlip flip = SDL_FLIP_NONE) {
			SDL_Rect textureRect = createTextureRect(id);
			SDL_Rect src = *frame;

			frameIndex %= frames;
			uint32_t horizontalIndex = frameIndex % (textureRect.w / src.w - 1);
			uint32_t verticalIndex = (frameIndex / (textureRect.w / src.w - 1)) % (textureRect.h / src.h);

			src.x += src.w * horizontalIndex;
			src.y += (src.h + src.y) * verticalIndex;

			drawTexture(id, &src, dest, flip);
		}

	private:
		TextureManager(SDL_Renderer* renderer) : renderer(renderer) {}
		~TextureManager() {
			for (auto& tex : textures) {
				SDL_DestroyTexture(tex.second);
			}
		}

		SDL_Renderer* renderer;
		std::unordered_map<std::string, SDL_Texture*> textures;
};
