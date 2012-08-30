#ifndef VAISSEAU_HPP
#define VAISSEAU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
#include "../Affichable.hpp"
using namespace sf;
using namespace std;
class Tir;
class Jeu;
class Vaisseau : 
	public Affichable {
	public:
		const static int JOUEUR = 0, ENNEMI = 1, CHERCHEUR = 2, TOURELLE = 3;
		const static int VITESSE_JOUEUR = 8;
		Vaisseau(int dir, int numVaisseau);
		~Vaisseau();
		static bool init();
		void deplacer(RenderWindow* win, int dx, int dy); 
		void incVie(int v);
		int getVie();
		bool estMort();
		bool touche(Tir* tir);
		void calculeRotation(float y, float x);
		void tirer(Jeu* jeu);
		virtual void ia(Jeu* jeu);
	private:
		static Image* img;
		static sf::SoundBuffer sbTir;
		int vie;
		int direction;
		Clock dernierTir;
		static Sound sonTir;
};
#endif