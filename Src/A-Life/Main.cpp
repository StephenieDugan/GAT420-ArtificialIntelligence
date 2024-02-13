#include "CA.h"
#include "Node.h"
#include "Renderer.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include "GameofLife.h"

int main(int, char**)
{
	float temperature = 30.0;
	bool sunny = true;
	float windSpeed = 5.0;
	bool highHumidity = true;

	node_t* playSportsAction = new action_t("Play outdoor sports");
	node_t* stayInsideAction = new action_t("Stay inside");

	node_t* weatherDecision = new decision_t<bool>("Weather (Sunny)", sunny, ePredicate::Equal, true);
	node_t* temperatureDecision = new decision_t<float>("Temperature (Hot)", temperature, ePredicate::Greater, 25.0); 
	node_t* windyDecision = new decision_t<float>("Windy?", windSpeed, ePredicate::Equal, true);
	node_t* windSpeedDecision = new decision_t<float>("Wind Speed (High)", windSpeed, ePredicate::Greater, 10.0);
	node_t* humidityDecision = new decision_t<bool>("Humidity (High)", highHumidity, ePredicate::Equal, true);
						//						Weather(Sunny)
						//					 /				   \
							   Temperature(Hot)			 Humidity(High)
						//	  /    |         \			/			\
					Play Sports  Stay Inside Windy?	Play Sports  Stay Inside
//												|
						//                Wind Speed (High)
						//                      / \
									 Play Sports  Stay Inside

	weatherDecision->trueNode = temperatureDecision;
	weatherDecision->falseNode = stayInsideAction;

	temperatureDecision->trueNode = playSportsAction;
	temperatureDecision->falseNode = humidityDecision;

	humidityDecision->trueNode = windyDecision;
	humidityDecision->falseNode = playSportsAction;

	windyDecision->trueNode = stayInsideAction;
	windyDecision->falseNode = windSpeedDecision;

	windSpeedDecision->trueNode = playSportsAction;
	windSpeedDecision->falseNode = stayInsideAction;

	// Traverse the decision tree from the root
	traverse(weatherDecision);

	// Cleanup
	delete playSportsAction;
	delete stayInsideAction;
	delete weatherDecision;
	delete temperatureDecision;
	delete humidityDecision;
	delete windyDecision;
	delete windSpeedDecision;

	return 0;
	/*glm::ivec2 screen_size{800, 600};
	glm::ivec2 env_size{ 800, 600 };

	// create renderer
	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("AI", screen_size.x, screen_size.y);

	// create render texture
	std::shared_ptr<Texture> texture = std::make_unique<Texture>(env_size.x, env_size.y, renderer);

	// create environment
	std::unique_ptr<Environment> env = std::make_unique<GameofLife>(env_size.x, env_size.y, texture);
	env->Initialize();

	bool quit = false;
	while (!quit)
	{
		// sdl events
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
			env->KeyDown(event.key.keysym.sym);
			break;

		case SDL_MOUSEMOTION:
			env->MousePositionNormalized(glm::vec2{ event.motion.x, event.motion.y } / (glm::vec2)screen_size);
			break;

		case SDL_MOUSEBUTTONDOWN:
			env->MouseButtonDown(event.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			env->MouseButtonUp(event.button.button);
			break;
		}

		// update environment
		env->Step();
		texture->Copy(env->GetColorBuffer());

		renderer.RenderCopy(texture);
		renderer.Present();
	}

	renderer.Shutdown();

	return 0;*/
}
