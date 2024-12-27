#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <filesystem>
#include <unordered_map>

class Game {
	public:
		Game() {
			if (!init()) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize game :3. %s", SDL_GetError());
			}

			running = true;
		}

		~Game() {
			clean();
		}

		void loop() {
			while (running) {
				handleEvents();
				update();
				render();

				frameIndex++;
				SDL_Delay(80);
			}
		}

	private:
		bool init() {
			bool success = true;
			success &= initSDL(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
			success &= initWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
			success &= initRenderer(0, SDL_RENDERER_ACCELERATED);

			SDL_Texture* puma_texture = createTexture(std::filesystem::path("spritelib_gpl")/"platform"/"char9.png");
			textures.emplace("puma", puma_texture);

			return success;
		}

		void handleEvents() {
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						running = false;
						break;
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym) {
							case SDLK_q:
								running = false;
								break;
							case SDLK_n:
								frameIndex++;
								break;
							case SDLK_p:
								frameIndex--;
								break;
						}
				}
			}
		}

		void update() {
		}

		void render() {
			SDL_RenderClear(renderer);

			SDL_Rect frame;
			frame.x = 0;
			frame.y = 35;
			frame.w = 128;
			frame.h = 96;

			drawSpriteSheet(textures.at("puma"), &frame, 6, frameIndex);

			SDL_RenderPresent(renderer);
		}

		void clean() {
			SDL_Log("Entered clean function");

			for (auto& tex : textures) {
				SDL_DestroyTexture(tex.second);
			}

			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		bool initSDL(uint32_t flags) {
			if (SDL_Init(flags) < 0) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL :3. %s", SDL_GetError());
				return false;
			}
			SDL_Log("Initialized SDL");

			return true;
		}

		bool initWindow(std::string title, int x, int y, int w, int h, SDL_WindowFlags flags) {
			window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
			if (!window) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window :3. %s", SDL_GetError());
				return false;
			}
			SDL_Log("Created window");

			return true;
		}

		bool initRenderer(int index, SDL_RendererFlags flags) {
			renderer = SDL_CreateRenderer(window, index, flags);
			if (!renderer) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer :3. %s", SDL_GetError());
				return false;
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_Log("Created renderer");

			return true;
		}

		SDL_Texture* createTexture(const std::filesystem::path& path) {
			SDL_Surface* tempSurface = IMG_Load(path.c_str());

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

			SDL_FreeSurface(tempSurface);

			return texture;
		}

		SDL_Rect createTextureRect(SDL_Texture* texture) {
			SDL_Rect rect{};
			SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

			return rect;
		}

		void drawTexture(SDL_Texture* texture, SDL_Rect* src = nullptr, SDL_Rect* dest = nullptr) {
			SDL_RenderCopy(renderer, texture, src, dest);
		}

		void drawSpriteSheet(SDL_Texture* texture, SDL_Rect* frame, uint32_t frames, uint32_t frameIndex) {
			SDL_Rect textureRect = createTextureRect(texture);
			SDL_Rect src = *frame;

			frameIndex %= frames;
			uint32_t horizontalIndex = frameIndex % (textureRect.w / src.w - 1);
			uint32_t verticalIndex = (frameIndex / (textureRect.w / src.w - 1)) % (textureRect.h / src.h);

			src.x += src.w * horizontalIndex;
			src.y += (src.h + src.y) * verticalIndex;

			drawTexture(texture, &src, nullptr);
		}

		void LogRect(SDL_Rect& rect) {
			SDL_Log("%u, %u, %u, %u", rect.x, rect.y, rect.w, rect.h);
		}


		// Core
		SDL_Window* window;
		SDL_Renderer* renderer;

		// Handle events
		SDL_Event event;

		// Game loop
		bool running = false;
		uint64_t frameIndex = 0;

		// Draw
		std::unordered_map<std::string, SDL_Texture*> textures;
};
