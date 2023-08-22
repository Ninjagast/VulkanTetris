#pragma once

#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include <map>

namespace lve
{
	class KeyboardMovementController
	{
	public:
		struct KeyMappings {
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
			int switchMovement = GLFW_KEY_TAB;
			int hardDrop = GLFW_KEY_SPACE;
		};

		std::map<int, bool> keyPressMap;

		void moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject);
		bool checkModeSwitch(GLFWwindow* window);
		bool checkKeyPress(GLFWwindow* window, int key);
		std::string getTetrisMove(GLFWwindow* window);

		KeyMappings	 keys{};
		float moveSpeed{ 3.f };
		float lookSpeed{ 1.5f };
		bool cameraMoveMode = true;
	};
}
