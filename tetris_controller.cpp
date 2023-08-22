#include  "tetris_controller.hpp"
#include "lve_model.hpp"
#include "lve_game_object.hpp"

#include <memory>
#include <algorithm>
#include <random>
#include <iostream>
#include <map>



namespace lve
{
	// temporary helper function, creates a 1x1x1 cube centered at offset
	std::unique_ptr<LveModel> createCubeModel(LveDevice& device, glm::vec3 offset) {
		std::vector<LveModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<LveModel>(device, vertices);
	}

	std::unique_ptr<LveModel> createWallModel(LveDevice& device, glm::vec3 offset) {
		std::vector<LveModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.1f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.1f, .1f, .1f}},
			{{-.5f, -.5f, .5f}, {.1f, .1f, .1f}},
			{{-.5f, -.5f, -.5f}, {.1f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.1f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.1f, .1f, .1f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.2f, .2f, .2f}},
			{{.5f, .5f, .5f}, {.2f, .2f, .2f}},
			{{.5f, -.5f, .5f}, {.2f, .2f, .2f}},
			{{.5f, -.5f, -.5f}, {.2f, .2f, .2f}},
			{{.5f, .5f, -.5f}, {.2f, .2f, .2f}},
			{{.5f, .5f, .5f}, {.2f, .2f, .2f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.3f, .3f, .3f}},
			{{.5f, -.5f, .5f}, {.3f, .3f, .3f}},
			{{-.5f, -.5f, .5f}, {.3f, .3f, .3f}},
			{{-.5f, -.5f, -.5f}, {.3f, .3f, .3f}},
			{{.5f, -.5f, -.5f}, {.3f, .3f, .3f}},
			{{.5f, -.5f, .5f}, {.3f, .3f, .3f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.3f, .3f, .3f}},
			{{.5f, .5f, .5f}, {.3f, .3f, .3f}},
			{{-.5f, .5f, .5f}, {.3f, .3f, .3f}},
			{{-.5f, .5f, -.5f}, {.3f, .3f, .3f}},
			{{.5f, .5f, -.5f}, {.3f, .3f, .3f}},
			{{.5f, .5f, .5f}, {.3f, .3f, .3f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.4f, .4f, .4f}},
			{{.5f, .5f, 0.5f}, {.4f, .4f, .4f}},
			{{-.5f, .5f, 0.5f}, {.4f, .4f, .4f}},
			{{-.5f, -.5f, 0.5f}, {.4f, .4f, .4f}},
			{{.5f, -.5f, 0.5f}, {.4f, .4f, .4f}},
			{{.5f, .5f, 0.5f}, {.4f, .4f, .4f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.5f, .5f, .5f}},
			{{.5f, .5f, -0.5f}, {.5f, .5f, .5f}},
			{{-.5f, .5f, -0.5f}, {.5f, .5f, .5f}},
			{{-.5f, -.5f, -0.5f}, {.5f, .5f, .5f}},
			{{.5f, -.5f, -0.5f}, {.5f, .5f, .5f}},
			{{.5f, .5f, -0.5f}, {.5f, .5f, .5f}},

		};
		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<LveModel>(device, vertices);
	}

	TetrisController::TetrisController(LveDevice &lveDevice)
	{
		cubeModel = createCubeModel(lveDevice, { .0f, .0f, .0f });
		wallModel = createWallModel(lveDevice, { .0f, .0f, .0f });
	}

	std::vector<LveGameObject> TetrisController::createPiece(LveDevice &lveDevice, std::string piece)
	{

		if (piece == "O")
		{
			pieceData[2] = 20;
			std::vector<LveGameObject> returnVector;

			//left bottom
			auto cube = LveGameObject::createGameObject();
			cube.model = cubeModel;
			cube.transform.translation = { .0f, -6.6f, 20.f };
			cube.transform.scale = { .5f, .5f, .5f };

			//right bottom
			auto cube2 = LveGameObject::createGameObject();
			cube2.model = cubeModel;
			cube2.transform.translation = { .6f, -6.6f, 20.f };
			cube2.transform.scale = { .5f, .5f, .5f };

			//left top
			auto cube3 = LveGameObject::createGameObject();
			cube3.model = cubeModel;
			cube3.transform.translation = { .0f, -6.0f, 20.f };
			cube3.transform.scale = { .5f, .5f, .5f };

			//right top
			auto cube4 = LveGameObject::createGameObject();
			cube4.model = cubeModel;
			cube4.transform.translation = { .6f, -6.0f, 20.f };
			cube4.transform.scale = { .5f, .5f, .5f };

			returnVector.push_back(cube);
			returnVector.push_back(cube2);
			returnVector.push_back(cube3);
			returnVector.push_back(cube4);

			pieceData[0] = 4;
			pieceData[1] = 2;
			pieceData[3] = 2;
			pieceData[5] = 4;
			return returnVector;
		}
		else if (piece == "I")
		{
			pieceData[2] = 18;
			std::vector<LveGameObject> returnVector;

			//top
			auto cube = LveGameObject::createGameObject();
			cube.model = cubeModel;
			cube.transform.translation = { .0f, -4.8f, 20.f };
			cube.transform.scale = { .5f, .5f, .5f };


			//top mid
			auto cube2 = LveGameObject::createGameObject();
			cube2.model = cubeModel;
			cube2.transform.translation = { .0f, -5.4f, 20.f };
			cube2.transform.scale = { .5f, .5f, .5f };

			//bot mid
			auto cube3 = LveGameObject::createGameObject();
			cube3.model = cubeModel;
			cube3.transform.translation = { .0f, -6.0f, 20.f };
			cube3.transform.scale = { .5f, .5f, .5f };

			//bot
			auto cube4 = LveGameObject::createGameObject();
			cube4.model = cubeModel;
			cube4.transform.translation = { .0f, -6.6f, 20.f };
			cube4.transform.scale = { .5f, .5f, .5f };

			returnVector.push_back(cube);
			returnVector.push_back(cube2);
			returnVector.push_back(cube3);
			returnVector.push_back(cube4);

			pieceData[0] = 4;
			pieceData[1] = 1;
			pieceData[3] = 4;
			pieceData[5] = 3;
			return returnVector;
		}
	}

	std::vector<LveGameObject> TetrisController::createTetrisBoard()
	{
		std::vector<LveGameObject> returnVector;

		//wall left
		auto wall1 = LveGameObject::createGameObject();
		wall1.model = wallModel;
		wall1.transform.translation = { -3.0f, -0.25f, 20.f };
		wall1.transform.scale = { .5f, 13.0f, .5f };

		//wall right
		auto wall2 = LveGameObject::createGameObject();
		wall2.model = wallModel;
		wall2.transform.translation = { 3.6f, -0.25f, 20.f };
		wall2.transform.scale = { .5f, 13.0f, .5f };

		//wall bottom
		auto wall3 = LveGameObject::createGameObject();
		wall3.model = wallModel;
		wall3.transform.translation = { 0.3f, 6.6f, 20.f };
		wall3.transform.scale = { 7.12f, .5f, .5f };

		returnVector.push_back(wall1);
		returnVector.push_back(wall2);
		returnVector.push_back(wall3);
		return returnVector;
	}

	std::vector<LveGameObject> TetrisController::SpawnRandomPiece(LveDevice& lveDevice)
	{
		if (piecesPicked == 2)
		{
			std::shuffle(std::begin(tetrisPieces), std::end(tetrisPieces), std::default_random_engine(std::random_device{}()));
			piecesPicked = 0;
		}
		std::vector returnVector = createPiece(lveDevice, tetrisPieces[piecesPicked]);
		//std::vector returnVector = createPiece(lveDevice, "O");
		piecesPicked += 1;

		return returnVector;
	}

	std::vector<std::pair<int, int>> TetrisController::moveRotatePiece(std::string move, std::vector<LveGameObject> &pieces, std::vector<std::pair<int, int>> claimedSpots)
	{
		if (pieces.size() > 0)
		{
			if (move == "hard_drop")
			{
				std::vector<std::pair<int, int>> endPos = moveDownCheckNextPiece(10, pieces, claimedSpots);
				for (int i = 0; i < 22; i++)
				{
					std::vector<std::pair<int, int>> endPos = moveDownCheckNextPiece(10, pieces, claimedSpots);

					if (endPos.size() > 0)
					{
						break;
					}
				}
				return endPos;
			}
			else if (move == "right")
			{
				if (pieceData[0] + pieceData[1] + 1 > 10)
				{
				}
				else
				{
					for (auto& obj : pieces)
					{
						obj.transform.translation.x += 0.6f;
					}
					pieceData[0] += 1;
				}

			}
			else if (move == "left")
			{
				if (pieceData[0] - 1 < 0)
				{
				}
				else
				{
					for (auto& obj : pieces)
					{
						obj.transform.translation.x -= 0.6f;
					}
					pieceData[0] -= 1;
				}

			}
			else if (move == "rotate_left")
			{

			}
			else if (move == "rotate_right")
			{

			}
		}
	}

	bool TetrisController::checkCollision(std::string directionCheck, std::vector<std::pair<int, int>>  claimedSpots)
	{
		if (directionCheck == "down")
		{
			for (auto& obj : claimedSpots)
			{
				if (pieceData[0] == obj.first || (pieceData[0] + 1 == obj.first) && pieceData[5] == 4)
				{
					if (pieceData[2] - 1 == obj.second)
					{
						return true;
					}
				}
			}
			if (pieceData[2] <= 0)
			{
				return true;
			}
		}
		return false;
	}

	std::vector<std::pair<int, int>> TetrisController::moveDownCheckNextPiece(float dt, std::vector<LveGameObject> &pieces, std::vector<std::pair<int, int>> claimedSpots)
	{
		timeTillDrop -= dt;
		if (timeTillDrop <= 0)
		{
			if (pieces.size() > 0)
			{
				timeTillDrop = 0.5f;
				if (checkCollision("down", claimedSpots))
				{
					std::vector<std::pair<int, int>> returnVec;
					for (int i = 0; i < pieceData[1]; i++)
					{
						returnVec.push_back(std::pair<int, int>{pieceData[0] + i, pieceData[2]});
						for (int j = 1; j < pieceData[3]; j++)
						{
							returnVec.push_back(std::pair<int, int>{pieceData[0] + i, pieceData[2] + j});
						}
					}
					return returnVec;
				}
				else
				{
					for (auto& obj : pieces)
					{
						obj.transform.translation.y += 0.6f;
					}
					pieceData[2] -= 1;
				}
			}
		}
	}

	//std::vector<std::pair<x, y>
	std::vector<int> TetrisController::checkLines(std::vector<std::pair<int, int>> claimedSpots)
	{
		if (claimedSpots.size() > 0)
		{
			std::map<int, int> columnData;
			std::vector<int> returnVector;

			for (auto& obj : claimedSpots)
			{
				auto it = columnData.find(obj.second);
				if (it != columnData.end())
				{
					it->second = 1 + it->second;
				}
				else
				{
					columnData.insert({obj.second, 1} );
				}
			}

			for (auto& obj : columnData)
			{
				if (obj.second == 10)
				{
					returnVector.push_back(obj.first);
				}
			}
			return returnVector;
		}
	}
}

