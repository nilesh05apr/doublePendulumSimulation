#include "DPendulum.h"
#include "Constants.h"
const int wheight = 600;
const int wwidth = 600;
const char* wtitle = "Double Pendulum Simulation";

int main() {
	srand(time(nullptr));

	float len_1 = 1.2;
	float len_2 = 1.3;

	float mass_1 = 10.0;
	float mass_2 = 10.0;

	float angle_1 = PI / 2;
	float angle_2 = PI;

	float damp_1 = 0.999;
	float damp_2 = 0.998;


	sf::RenderWindow window(sf::VideoMode(wwidth,wheight),wtitle);
	window.setFramerateLimit(FPS);
	sf::Event e;

	DPendulum pend(len_1, len_2, mass_1, mass_2, angle_1, angle_2, damp_1, damp_2);
	pend.setupRenderObjects(600, 600);

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) window.close();

			if (e.type == sf::Event::MouseButtonPressed) pend.clicked(sf::Mouse::getPosition(window));

			if (e.type == sf::Event::KeyPressed) {
				switch (e.key.code)
				{
				case sf::Keyboard::Escape: window.close(); break;
				case sf::Keyboard::Space: pend.changestate(); break;
				case sf::Keyboard::R: pend.rodvisibility(); break;
				}
			}
		}
		pend.update();
		pend.render();

		window.clear(sf::Color(0.5,0.5,0.5));
		window.draw(pend);
		window.display();
	}
	return 0;
}