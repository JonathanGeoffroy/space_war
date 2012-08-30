#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Jeu.hpp"

int main() {
	sf::RenderWindow app(sf::VideoMode(800, 600), "space war");
	app.SetFramerateLimit(60);
	app.UseVerticalSync(true);
	app.ShowMouseCursor(false);
	app.EnableKeyRepeat (false);
	app.SetCursorPosition(app.GetWidth() / 2, app.GetHeight() / 2);
	if(! (Vaisseau::init() && Tir::init() && Jeu::init()) )
		return EXIT_FAILURE;
	Jeu jeu(&app);
	do {
		jeu.reinitialisation();
		jeu.jouer();
	} while(jeu.recommencerPartie());
	app.Close();
	return EXIT_SUCCESS;
}