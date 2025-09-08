#ifndef CPP_HELLO
#define CPP_HELLO
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main()
{
	cout << "Hello, World!" << endl;

	RenderWindow window({ 500, 250 }, "Lab 1", Style::Default);

	Font font;
	if (!font.loadFromFile("../../SFML-2.6.2/examples/opengl/resources/tuffy.ttf"))
		return 1;

	Text text("Hello World!", font, 75);
	text.setPosition(75, 75);

	const int START_SECONDS = 10;
	Clock clock;

	bool showZeroThisFrame = false;

	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
		}

		int elapsed = static_cast<int>(clock.getElapsedTime().asSeconds());
		int remaining = START_SECONDS - elapsed;
		if (remaining < 0) remaining = 0;

		text.setString(to_string(remaining));

		window.clear();
		window.draw(text);
		window.display();

		if (remaining == 0) {
			if (showZeroThisFrame) {
				window.close();
			}
			else {
				showZeroThisFrame = true;
			}
		}
	}
	return 0;
}
#endif CPP_HELLO