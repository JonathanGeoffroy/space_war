#ifndef JEU_HPP
#define JEU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h> // FIXME : existe-il la même chose en CPP ? 
#include <deque>
#include "Tirs/Tir.hpp"
#include "Vaisseaux/Vaisseau.hpp"
using namespace sf;

class Jeu {
public:
	Jeu(RenderWindow* win);
	~Jeu();
	static bool init();
	void jouer();
	void affiche();
	void changeTime();
	Vaisseau* getJoueur();
	deque<Vaisseau*>* getEnnemis();
	deque<Tir*>* getTirs();
	RenderWindow* getWindow();
private:
	static sf::Image imgViseur;
	static sf::SoundBuffer sbTir;
	RenderWindow* window;
	deque<Vaisseau*> ennemis;
	Vaisseau joueur;
	deque<Tir*> tirs;
	Sprite viseur;
	Sound sonTir;
	Music musique;
};
#endif