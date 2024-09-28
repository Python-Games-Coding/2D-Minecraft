#include "../../hdr/util/Chat.hpp"
#include "../../hdr/graphics/Window.hpp"
#include "../../hdr/util/Loader.hpp"
#include "../../hdr/entity/Player.hpp"

namespace engine {
	std::string username = "Player";

	Message::Message() : 
		time(render::window::ticks),
		color(gs::Color::White)
	{
	}

	std::vector<Message> chat;

	void addChatMessage(Message& message, bool fromPlayer) {
		const std::string commandText = message.text; 

		if (fromPlayer)
			message.text = "<" + username + "> " + message.text; 

		if (chat.size() >= maxNumOfChatMessages) 
			chat.erase(chat.begin()); 
		
		chat.push_back(message);

		if (commandText[0] == '/') 
			runCommand(commandText); 
	}
	void runCommand(const std::string& command) {
		StringVector& arguments = seperateWords(command.substr(1)); 
		Message outputMessage; 

		const int argumentCount = arguments.size(); 

		if (argumentCount == 0) return; 

		if (arguments[0] == "tp") {
			if (argumentCount != 3) return; 

			gs::Vec2f translatedPosition = player->position; 

			if (arguments[1].find('~') != std::string::npos) {
				arguments[1].erase(arguments[1].begin());

				if (!arguments[1].empty())
					translatedPosition.x += std::stoi(arguments[1]);
			}
			else
				translatedPosition.x = std::stoi(arguments[1]);
			
			if (arguments[2].find('~') != std::string::npos) {
				arguments[2].erase(arguments[2].begin());

				if (!arguments[2].empty())
					translatedPosition.y += std::stoi(arguments[2]);
			}
			else
				translatedPosition.y = std::stoi(arguments[2]);

			std::string translatedXposString = 
				std::to_string(translatedPosition.x);
			std::string translatedYposString =
				std::to_string(translatedPosition.y); 

			translatedXposString = translatedXposString.substr(
				0, translatedXposString.find('.') + 2
			);
			translatedYposString = translatedYposString.substr(
				0, translatedYposString.find('.') + 2
			); 

			player->position = translatedPosition; 
			outputMessage.text = "Teleported " + username + " to " 
				+ translatedXposString + ", " + translatedYposString; 
		}
		else if (arguments[0] == "seed") 
			outputMessage.text = "Seed: " + std::to_string(world->seed); 
		else if (arguments[0] == "setblock") {
			if (argumentCount != 4) return; 

			const gs::Vec2i blockPosition = gs::Vec2i(
				std::stoi(arguments[1]), std::stoi(arguments[2])
			);

			if (world->setBlock(blockPosition,
					static_cast<Block::Id>(std::stoi(arguments[3]))))
				outputMessage.text = "Block placed"; 
			else {
				outputMessage.text = "Block placement position out of bounds"; 
				outputMessage.color = gs::Color::Red; 
			}
		}
		else if (arguments[0] == "setwall") {
			if (argumentCount != 4) return;

			const gs::Vec2i wallPosition = gs::Vec2i(
				std::stoi(arguments[1]), std::stoi(arguments[2])
			); 

			if (world->setWall(wallPosition,
					static_cast<Wall::Id>(std::stoi(arguments[3]))))
				outputMessage.text = "Wall placed"; 
			else {
				outputMessage.text = "Wall placement position out of bounds"; 
				outputMessage.color = gs::Color::Red; 
			}
		}
		else if (arguments[0] == "time") {
			if (argumentCount != 3) return;

			if (arguments[1] == "set") {
				world->gameTime.gameTicks = std::stoi(arguments[2]);
				outputMessage.text = "Set the time to " + arguments[2]; 
			}
			else if (arguments[1] == "query") {
				if (arguments[2] == "daytime") {
					outputMessage.text = "Time is " 
						+ std::to_string(world->gameTime.gameTicks); 
				}
			}
		}
		else if (arguments[0] == "gamemode") {
			if (argumentCount != 2) return; 

			if (arguments[1] == "survival" || arguments[1] == "s" 
				|| arguments[1] == "0") 
			{
				gameMode = GameMode::Survival; 
				outputMessage.text = "Set own gamemode to Survival Mode";
			}
			else if (arguments[1] == "creative" || arguments[1] == "c"
				|| arguments[1] == "1")
			{
				gameMode = GameMode::Creative; 
				outputMessage.text = "Set own gamemode to Creative Mode"; 
			}
			else if (arguments[1] == "spectator" || arguments[1] == "sp"
				|| arguments[1] == "2")
			{
				gameMode = GameMode::Spectator; 
				outputMessage.text = "Set own gamemode to Spectator Mode"; 
			} 
		}
		else if (arguments[0] == "gamerule") {
			if (argumentCount != 3) return; 

			const bool tag = !(arguments[2] == "0" || arguments[2] == "false");
			bool updateOutput = false; 

			if (arguments[1] == "doDaylightCycle") {
				world->doDaylightCycle = tag; 
				updateOutput = true; 
			}
			else if (arguments[1] == "doMobSpawning") {
				world->doMobSpawning = tag; 
				updateOutput = true; 
			}

			if (updateOutput)
				outputMessage.text = "Game rule has been updated"; 
		}

		if (!outputMessage.text.empty()) 
			addChatMessage(outputMessage, false); 
	}
}