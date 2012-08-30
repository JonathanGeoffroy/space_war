#ifndef CHERCHEUR_HPP
#define CHERCHEUR_HPP

#include "Vaisseau.hpp"

class  Jeu;
class Chercheur :
	public Vaisseau 
{
public:
	Chercheur();
	virtual void ia(Jeu* jeu);
};
#endif