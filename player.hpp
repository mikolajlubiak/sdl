#pragma once

#include "game_object.hpp"

class Player : public GameObject {
	public:
		Player(std::string textureID, SDL_Rect textureSrc, uint32_t frames) : GameObject(textureID, textureSrc, frames) {}

		void draw(SDL_Renderer* renderer, uint32_t frameIndex) override {
			GameObject::draw(renderer, frameIndex);

		}

		void update() override {
			GameObject::update();
		}
};
