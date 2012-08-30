#include "Tourelle.hpp"
#include "../Jeu.hpp"

Tourelle::Tourelle() : Vaisseau(Affichable::BAS, Vaisseau::TOURELLE) {
	tirEnCours = false;
	dirHorizontale = GAUCHE;
}

/**
  * L'IA de la tourelle consiste à tourner dans un sens, puis dans un autre, tout en tirant
  * En aucun cas elle ne se déplace
  * sa position initiale est tournée à 180° (vers le joueur)
  * elle tir de gauche à droite, puis de droite à gauche, avant de marquer un temps d'arrêt jusqu'à revenir en position initiale
  * Ensuite, elle recommence.
  */
void Tourelle::ia(Jeu* jeu) {
	int rotat = GetRotation();
	if(! tirEnCours) {
		//Dans ce cicle, on a pas encore tiré : on se déplace vers la gauche jusqu'à se trouver à -90 
		if(rotat <= 270 && dirHorizontale == GAUCHE) {
			//On commence à tirer !
			tirEnCours = true;
			dirHorizontale = DROITE;
		}
		else if (rotat == 180)
			dirHorizontale = GAUCHE;
		else
			SetRotation(rotat + dirHorizontale);
	}
	else {
		SetRotation(rotat + dirHorizontale);
		
		if(rotat <= -90) {
			tirEnCours = false;
			dirHorizontale = DROITE;
		}
		else
			tirer(jeu);
	}
}