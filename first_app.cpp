#include "first_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"
// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <chrono>
#include <cassert>

#include "tetris_controller.hpp"

namespace lve {

	FirstApp::FirstApp()
	{
	}

	FirstApp::~FirstApp()
	{
	}


	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };
		LveCamera camera{};
		auto viewerObject = LveGameObject::createGameObject();
		KeyboardMovementController playerController{};
		TetrisController tetrisController{ lveDevice };
		createTetrisBoard(tetrisController);
		spawnTetrisPiece(tetrisController);
		
		auto currentTime = std::chrono::high_resolution_clock::now();
		bool once = true;

		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			if (playerController.checkModeSwitch(lveWindow.getGlFWwindow()))
			{
				playerController.moveInPlaneXZ(lveWindow.getGlFWwindow(), frameTime, viewerObject);
				camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
				float aspect = lveRenderer.getAspectRatio();
				camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 30.f);
			}
			else
			{
				if (!removeAnimation)
				{
					std::string tetrisMove = playerController.getTetrisMove(lveWindow.getGlFWwindow());
					if (tetrisMove != "no")
					{

						std::vector<std::pair<int, int>> claimedSpots = generateClaimedSpots();
						std::vector<std::pair<int, int>> endPos = tetrisController.moveRotatePiece(tetrisMove, currentFallingPieces, claimedSpots);
						if (endPos.size() > 0)
						{
							nextPiece(endPos, tetrisController);
							checkForLine(tetrisController);
						}
					}
					else
					{
						std::vector<std::pair<int, int>> claimedSpots = generateClaimedSpots();
						std::vector<std::pair<int, int>> endPos = tetrisController.moveDownCheckNextPiece(frameTime, currentFallingPieces, claimedSpots);
						if (endPos.size() > 0)
						{
							nextPiece(endPos, tetrisController);
							checkForLine(tetrisController);
						}
					}
				}
			}
			if (removeAnimation)
			{
				for (int y : lines)
				{
					for (int x = 0; x < 10; x++)
					{
						auto it = tetrisPieces.find(std::pair{ x, y });
						if (it != tetrisPieces.end())
						{
							it->second.transform.rotation.y = glm::mod(it->second.transform.rotation.y + 0.001f * animtationSpeed, glm::two_pi<float>());
						}
					}
				}
				animationTick += 1;
				if (animationTick % 1000 == 0)
				{
					animtationSpeed = animtationSpeed * 2;
				}
				if (animationTick == 6000)
				{
					animtationSpeed = 1;
					animationTick = 1;
					removeAnimation = false;
					removeLines();
				}
			}
			draw(simpleRenderSystem, camera);
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::draw(SimpleRenderSystem &simpleRenderSystem, LveCamera &camera)
	{
		if (auto commandBuffer = lveRenderer.beginFrame())
		{
			lveRenderer.beginSwapChainRenderPass(commandBuffer);

			simpleRenderSystem.renderGameObjects(commandBuffer, currentFallingPieces, camera);

			std::vector<LveGameObject> tetrisPiecesTextures;
			if (tetrisPieces.size() > 0)
			{
				for (auto const& x : tetrisPieces)
				{
					tetrisPiecesTextures.push_back(x.second);
				}
			}

			simpleRenderSystem.renderGameObjects(commandBuffer, tetrisPiecesTextures, camera);
			simpleRenderSystem.renderGameObjects(commandBuffer, tetrisBoard, camera);
			lveRenderer.endSwapChainRenderPass(commandBuffer);
			lveRenderer.endFrame();
		}
	}

	void FirstApp::createTetrisBoard(TetrisController &tetrisController)
	{
		std::vector <LveGameObject> insertVector = tetrisController.createTetrisBoard();
		for (std::size_t i = 0; i < insertVector.size(); ++i)
		{
			tetrisBoard.push_back(insertVector[i]);
		}
	}

	void FirstApp::spawnTetrisPiece(TetrisController &tetrisController)
	{
		std::vector <LveGameObject> insertVector = tetrisController.SpawnRandomPiece(lveDevice);
		for (std::size_t i = 0; i < insertVector.size(); ++i)
		{
			currentFallingPieces.push_back(insertVector[i]);
		}
	}

	void FirstApp::nextPiece(std::vector<std::pair<int, int>> endPos, TetrisController &tetrisController)
	{
		int i = 0;
		for (auto& obj : endPos)
		{
			tetrisPieces.insert({ std::pair<int, int> {obj.first, obj.second}, currentFallingPieces[i] });
			i++;
		}
		currentFallingPieces.clear();
		spawnTetrisPiece(tetrisController);
	}

	std::vector<std::pair<int, int>> FirstApp::generateClaimedSpots()
	{
		std::vector<std::pair<int, int>> claimedSpots;
		if (tetrisPieces.size() > 0)
		{
			for (auto const& x : tetrisPieces)
			{
				claimedSpots.push_back(std::pair{ x.first.first, x.first.second });
			}
		}
		return claimedSpots;
	}

	void FirstApp::checkForLine(TetrisController &tetrisController)
	{
		lines =  tetrisController.checkLines(generateClaimedSpots());
		if (lines.size() > 0)
		{
			removeAnimation = true;
		}
	}

	void FirstApp::removeLines()
	{
		if (lines.size() > 0)
		{
			for (int y : lines)
			{
				for (int x = 0; x < 10; x++)
				{
					auto it = tetrisPieces.find(std::pair{ x, y });
					if (it != tetrisPieces.end())
					{
						tetrisPieces.erase(std::pair{ x, y });
					}
				}
			}
			std::map<std::pair<int, int>, LveGameObject> newMap;
			for (int y : lines)
			{
				for (auto& obj : tetrisPieces)
				{
					if (obj.first.second > y)
					{
						obj.second.transform.translation.y += 0.6f * lines.size();
						newMap.insert({ std::pair{ obj.first.first, obj.first.second - 2 }, obj.second });
					}
					else
					{
						newMap.insert({ std::pair{ obj.first.first, obj.first.second - 2 }, obj.second });
					}
				}
			}

			tetrisPieces.clear();
			tetrisPieces = newMap;
		}
	}
}
