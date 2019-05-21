#include <SFML/Graphics.hpp>

#include <iostream>

void blur(sf::Texture &texture)
{
	auto img = texture.copyToImage();
	auto size = img.getSize();

	auto med = [&](int x, int y)
	{
		constexpr int n = 30, offset = n / 2;
		int r = 0, g = 0, b = 0, m = 0;

		for(int i = x - offset; i < n + x - offset; i++)
		{
			if(i < 0 || i >= size.x) continue;

			for(int j = y - offset; j < n + y - offset; j++)
			{
				if(j < 0 || j >= size.y) continue;

				auto color = img.getPixel(i, j);
				r += color.r;
				g += color.g;
				b += color.b;

				++m;
			}
		}

		if(m == 0) return img.getPixel(x, y);

		r /= m, g /= m, b /= m;

		return sf::Color(r, g, b);
	};

	for(int i = 0; i < size.x; i++)
		for(int j = 0; j < size.y; j++)
		{
			auto color = med(i, j);
			img.setPixel(i, j, color);
		}

	texture.update(img);
}

int main()
{
	sf::Texture texture;

	if(!texture.loadFromFile("lily.jpg") ) return EXIT_FAILURE;

	sf::Sprite sprite(texture);
	auto size = texture.getSize();
	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Blur");

	blur(texture);

	/*
	int a = 5;
	int b = 15;

	std::cout << a << ", " << b << '\n';

	a ^= b;
	b ^= a;
	a ^= b;

	std::cout << a << ", " << b << '\n';
	*/

	window.setFramerateLimit(60u);

	while(window.isOpen() )
	{
		sf::Event event;

		while(window.pollEvent(event) )
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::Resized:
					sprite.setScale(
							event.size.width / sprite.getLocalBounds().width,
							event.size.height / sprite.getLocalBounds().height
							);
					break;
				case sf::Event::KeyPressed:
					switch(event.key.code)
					{
						case sf::Keyboard::Key::Q:
						case sf::Keyboard::Key::Escape:
							window.close();
							break;
					}
			}
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return EXIT_SUCCESS;
}
