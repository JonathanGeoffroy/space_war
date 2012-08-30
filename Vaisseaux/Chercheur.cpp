#include "Chercheur.hpp"
#include "../Jeu.hpp"
Chercheur::Chercheur() : Vaisseau(Affichable::BAS, Vaisseau::CHERCHEUR) {}
void Chercheur::ia(Jeu* jeu) {
	Vaisseau* joueur = jeu->getJoueur();
	int thisGauche = GetPosition().x;
	int thisDroite = thisGauche + (GetSize().x / 2);
	int joueurGauche = joueur->GetPosition().x;
	int joueurDroite = joueurGauche + (joueur->GetSize().x / 2);
	RenderWindow* win = jeu->getWindow();
	if(thisDroite < joueurGauche) {
		deplacer(win, 10, 0);
	} else if (thisGauche > joueurDroite) {
		deplacer(win, -10, 0);
	} else {
		tirer(jeu);
	}
}