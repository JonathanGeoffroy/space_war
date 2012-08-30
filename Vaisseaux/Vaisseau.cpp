#include "Vaisseau.hpp"
#include "../Tirs/Tir.hpp"
#include "../Jeu.hpp"

Image* Vaisseau::img = NULL;
Vaisseau::Vaisseau(int dir, int numVaisseau) : Affichable(dir) {
	vie = 100;
	SetImage(img[numVaisseau]);
	SetCenter(GetSize().x / 2, GetSize().y / 2);
}

Vaisseau::~Vaisseau() {}

bool Vaisseau::init() {
	bool allIsInited = true;
	int nbImgInit = 4;
	std::string nomImg[4] = {"./images/joueur.png", "./images/ennemi.png", "./images/chercheur.png", "./images/tourelle.png"};
	Vaisseau::img = new Image[nbImgInit];
	for(int i = 0; i < nbImgInit; i++) {
		if(!img[i].LoadFromFile(nomImg[i])) {
			allIsInited = false;
		};
	}
	return allIsInited;
}

bool Vaisseau::touche(Tir* tir) {
	Vector2f posTir = tir->GetPosition();
	Vector2f sizeTir = tir->GetSize();
	Vector2f posThis = GetPosition();
	Vector2f sizeThis = GetSize();
	return (posTir.x  - sizeTir.x / 2 > posThis.x - sizeThis.x / 2 && posTir.x + sizeTir.x / 2 < posThis.x + sizeThis.x / 2
		&& posTir.y - sizeTir.y / 2 <= posThis.y + sizeThis.y / 2 && posTir.y + sizeTir.y / 2 >= posThis.y - sizeThis.y / 2);
}

// FIXME : vérifier que l'angle est bien calculé !!!!
void Vaisseau::calculeRotation(float y, float x) {
	float arcTan = atan2f(y, x);
	float degAngle;
	float demiPi = 3.14 / 2;
	if(arcTan >= demiPi) {
		degAngle = arcTan - demiPi;
		degAngle = - (degAngle * 90);
	}
	else {
		degAngle = demiPi - arcTan;
		degAngle = degAngle * 90;
	}
	std::cout << "angle :" << std::endl; 
	SetRotation(degAngle);
}

void Vaisseau::incVie(int v) { vie += v; }
int Vaisseau::getVie() { return vie; }
bool Vaisseau::estMort() { return vie <= 0; }
void Vaisseau::tirer(Jeu* jeu) {
	if(dernierTir.GetElapsedTime() >= 0.2f) {
		Tir* t = new Tir(this);
		jeu->getTirs()->push_back(t);
		dernierTir.Reset();
	}
}

/**
  * Déplace le vaisseau, tout en empêchant que celui-ci sorte de la fenêtre passée en paramètre
  */
void Vaisseau::deplacer(RenderWindow* win, int dx, int dy) {
	Vector2f pos = GetPosition();
	Vector2f size = GetSize();
	if(pos.x + dx - size.x / 2 >= 0 && pos.x + dx + size.x / 2 <= win->GetWidth()
			&& pos.y + dy - size.y / 2 >= 0 && pos.y + dy + size.y / 2 <= win->GetHeight()) {
		Move(dx, dy);
	}
}
/**
  * Pour les ennemis "classiques", l'ia consiste simplement à:
  		* se déplacer à gauche ou droite au hasard (sous réserve que le vaisseau ne touche pas déjà un bord)
  		* tirer une fois sur 3 en moyenne; avec une petite gestion du hasard
  */
void Vaisseau::ia(Jeu* jeu) {
	/*
	Vector2f pos = GetPosition();
	int deplacement;
	if(pos.x + (GetSize().x / 2) >= jeu->getWindow()->GetWidth()) {
		deplacement = -1;
	} else if(pos.x <= 0) {
		deplacement = 1;
	} else {
		float dirIa = sf::Randomizer::Random(-1, 1);
		if(dirIa > 0) {
			deplacement = 1;
		} else {
			deplacement = -1;
		}
	}
	Move(deplacement, 0);

	float tirIa = sf::Randomizer::Random(0, 3);
	if(tirIa >= 2) {
		tirer(jeu);
	}
	*/
	if(dernierTir.GetElapsedTime() >= 1.5) {
		tirer(jeu);
		float dirIa = sf::Randomizer::Random(0, 2);
		RenderWindow* win = jeu->getWindow();
		if(dirIa >= 1.0f) {
			deplacer(win, -10, 0);
		} else {
			deplacer(win, 10, 0);
		}
	}
}