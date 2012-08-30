#include <SFML/Graphics.hpp>
#include <iostream>
#include "Affichable.hpp"
#include "Jeu.hpp"
int main() {
	sf::RenderWindow app(sf::VideoMode(800, 600), "space war");
	app.SetFramerateLimit(60);
	app.ShowMouseCursor(false);
	app.SetCursorPosition(app.GetWidth() / 2, app.GetHeight() / 2);
	if(! (Vaisseau::init() && Tir::init() && Jeu::init()) )
		return EXIT_FAILURE;
	Jeu jeu(&app);
	jeu.jouer();
	app.Close();
	return EXIT_SUCCESS;
}