#pragma once

// Dependencies
#include "../Resources.hpp"

namespace engine {
	extern std::string username; 

	struct Message {
		std::string text; 
		int time; 
		gs::Color color; 

		Message(); 
		~Message() = default; 
	};

	constexpr int maxNumOfChatMessages = 100; 
	constexpr int maxMessageLength = 50; 

	extern std::vector<Message> chat; 

	void addChatMessage(Message& message, bool fromPlayer = true); 
	void runCommand(const std::string& command); 
}