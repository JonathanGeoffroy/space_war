#ifndef TOURELLE_HPP
#define TOURELLE_HPP
#include "Vaisseau.hpp"
class Jeu;
class Tourelle :
	public Vaisseau
{
public:
	Tourelle();
	virtual void ia(Jeu* jeu);
private:
	const static int GAUCHE = -1, DROITE = 1;
	bool tirEnCours;
	int dirHorizontale;
};

#endif