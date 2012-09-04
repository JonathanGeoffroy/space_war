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
		const static int SALVE_ENNEMIS = 0, SALVE_CHERCHEURS = 1, SALVE_TOURELLES = 2, SALVE_CHERCHEURS_ET_TOURELLES = 3, SALVE_ENNEMIS_ET_TOURELLES = 4, SALVE_ALL = 5;
		const static int NB_TYPE_SALVE = 6, NB_ENNEMIS_SALVE = 5;
		const static int VITESSE_JOUEUR = 8;
		Vaisseau(int dir = Affichable::BAS, int numVaisseau = 1);
		~Vaisseau();
		static bool init();
		static void createSalve(deque<Vaisseau*>* salve, RenderWindow* window, int typeSalve);
		void deplacer(RenderWindow* win, int dx, int dy); 
		void incVie(int v);
		void setVie(int v);
		int getVie();
		bool estMort();
		bool touche(Tir* tir);
		void calculeRotation(float y, float x);
		void tirer(Jeu* jeu, float tempsEntre2Tirs = 0.2f);
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