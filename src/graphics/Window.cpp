#include "../../hdr/graphics/Window.hpp"
#include "../../hdr/graphics/Render.hpp"

namespace engine {
	namespace render {
		namespace window {
			const float defaultAspectRatio = defaultWindowWidth 
				/ defaultWindowHeight; 
			const std::string gameTitle = std::string("2D Minecraft")
#ifdef mDebug
				+ std::string(" v ") + toString(mMajorVersion) 
				+ std::string(".") + toString(mMinorVersion);
#else 
				+ std::string(" - ") + mUpdateName;
#endif
			const gs::Vec2u defaultWindowSize = gs::Vec2u(
				defaultWindowWidth, defaultWindowHeight
			);
			const gs::Vec2f screenCenter = gs::Vec2f(
				defaultWindowWidth / 2.0f, defaultWindowHeight / 2.0f
			); 
			const int framerate = 60;

			sf::RenderWindow* winmain = nullptr; 
			bool isFullScreen = false;
			sf::Image icon;
			int renderingFramerate = 60;
			int currentFramerate = 0; 
			int currentUncappedFramerate = 0; 
			int ticks = 0;
			int mouseDelta = 0; 

			gs::Vec2u getDesktopWindowSize() {
				const auto& videoModes = sf::VideoMode::getFullscreenModes(); 
				return gs::Vec2u(videoModes[0].width, videoModes[0].height);
			}

			void open(bool fullscreen) {
				auto getStyle = [](bool fullscreen) {
					return fullscreen ? sf::Style::Fullscreen 
						: sf::Style::Default; 
				};

				const gs::Vec2u windowSize = getDesktopWindowSize();

				isFullScreen = fullscreen; 

				if (winmain == nullptr) {
					winmain = new sf::RenderWindow(
						sf::VideoMode(defaultWindowSize.x, defaultWindowSize.y),
						gameTitle, getStyle(isFullScreen)
					); 
				}
				else {
					winmain->create(
						sf::VideoMode(defaultWindowSize.x, defaultWindowSize.y),
						gameTitle, getStyle(isFullScreen)
					); 
				}

				if (windowSize != defaultWindowSize)
					winmain->setSize(windowSize); 

				if (icon.getPixelsPtr() == nullptr)
					icon.loadFromFile(render::assetDirectory 
						+ "textures/icon.png");

				initWindowStates(); 
			}
			void initWindowStates() {
				winmain->setIcon(
					icon.getSize().x, icon.getSize().y, 
					icon.getPixelsPtr()
				);
				gs::input::setWindow(winmain); 
				gs::input::priv::defaultWindowSize 
					= gs::Vec2f(defaultWindowSize); 
			}
			void update() {
				sf::Event event; 

				gs::input::updateInputs(); 
				mouseDelta = 0; 

				while (winmain->pollEvent(event)) {
					gs::input::updateEvents(event); 

					switch (event.type) {
					case sf::Event::Closed:
						winmain->close(); 
						break; 
					case sf::Event::MouseWheelScrolled:
						mouseDelta = event.mouseWheelScroll.delta; 
						break; 
					//case sf::Event::Resized:
					//{
					//	const gs::Vec2u windowSize = winmain->getSize();
					//
					//	winmain->setSize(gs::Vec2u(
					//		windowSize.x, static_cast<float>(windowSize.x) 
					//			/ defaultAspectRatio
					//	)); 
					//}
					//	break; 
					case sf::Event::KeyPressed:
						switch (event.key.code) {
						case sf::Keyboard::F11:
							open(!isFullScreen); 
							break; 
						}
						break; 
					}
				}

				ticks++; 
			}
			void close() {
				delete winmain; 
			}

			void begin(gs::Color fillColor) {
				winmain->clear(fillColor); 
			}
			void end() {
				winmain->display(); 
			}
		}
	}
}