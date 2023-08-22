#pragma once

#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"
#include "tetris_controller.hpp"
#include "simple_render_system.hpp"
#include "lve_camera.hpp"

#include <memory>
#include <vector>
#include <map>
#include <string>

namespace lve{
	class FirstApp {
		public:
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

			FirstApp();
			~FirstApp();

			FirstApp(const FirstApp &) = delete;
			FirstApp &operator=(const FirstApp &) = delete;

			void run();

		private:
			LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
			LveDevice lveDevice{ lveWindow };
			LveRenderer lveRenderer{ lveWindow, lveDevice };

			std::map<std::pair<int, int>, LveGameObject> tetrisPieces;
			std::vector<LveGameObject> tetrisBoard;
			std::vector<LveGameObject> currentFallingPieces;

			void spawnTetrisPiece(TetrisController &tetrisController);
			void createTetrisBoard(TetrisController &tetrisController);
			void nextPiece(std::vector<std::pair<int, int>> endPos, TetrisController &tetrisController);
			void checkForLine(TetrisController &tetrisController);
			void removeLines();
			void draw(SimpleRenderSystem &simpleRenderSystem, LveCamera &camera);
			
			std::vector<std::pair<int, int>> generateClaimedSpots();
			std::vector<int> lines;
			bool removeAnimation;
			int animationTick = 1;
			int animtationSpeed = 1;

	};
}