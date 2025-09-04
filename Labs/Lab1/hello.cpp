#ifndef CPP_HELLO
#define CPP_HELLO
#include <iostream>
#include <SFLM/Graphics.hpp>

using namespace std;
using namespace sf;

int main()
{
	cout << "Hello, World!" << endl;

	RenderWindow window({ 500, 250 }, "Lab 1", Style::Default);

	Font font;
	if (!font.loadFromFile("../../SFML-2.6.0/examples/opengl/resources/tuffy.ttf"))
		return 1;

	Text text("Hello World!", font, 75);
	text.setPosition(75, 75);

	while (true)
	{
		window.clear();
		window.draw(text);
		window.display();

	}
	return 0;
}
#endif CPP_HELLO