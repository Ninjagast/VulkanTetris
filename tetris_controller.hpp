#pragma once
#include "lve_game_object.hpp"
#include <string>
#include <map>

namespace lve
{
	class TetrisController
	{
	public:
		TetrisController(LveDevice& lveDevice);

		std::vector<std::pair<int, int>> moveRotatePiece(std::string move, std::vector<LveGameObject> &pieces, std::vector<std::pair<int, int>> claimedSpots);
		std::vector<int> checkLines(std::vector<std::pair<int, int>> claimedSpots);
		std::vector<LveGameObject> SpawnRandomPiece(LveDevice& lveDevice);
		std::vector<LveGameObject> createTetrisBoard();
		std::vector <std::pair<int, int>> moveDownCheckNextPiece(float dt, std::vector<LveGameObject> &pieces, std::vector<std::pair<int, int>> claimedSpots);

		int score = 0;
		int fallingState = false;
		std::vector<std::string> tetrisPieces = { "O", "I" };
		int piecesPicked = 2;

	private:
		std::vector<LveGameObject> createPiece(LveDevice& lveDevice, std::string piece);
		bool checkCollision(std::string directionCheck, std::vector<std::pair<int, int>> claimedSpots);

		std::shared_ptr<lve::LveModel> cubeModel;
		std::shared_ptr<lve::LveModel> wallModel; 
		int pieceData[6]; //x, width, y, height, rotation, pieceId
		float timeTillDrop = 0.5f;
	};
}
