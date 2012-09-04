#include "Vaisseau.hpp"
#include "../Tirs/Tir.hpp"
#include "../Jeu.hpp"

Image* Vaisseau::img = NULL;
sf::SoundBuffer Vaisseau::sbTir = sf::SoundBuffer();
sf::Sound Vaisseau::sonTir = sf::Sound();
Vaisseau::Vaisseau(int dir, int numVaisseau) : Affichable(dir) {
	vie = 100;
	SetImage(img[numVaisseau]);
	SetCenter(GetSize().x / 2, GetSize().y / 2);
}

Vaisseau::~Vaisseau() {}

/**
  * Permet de créer une salve contenant plusieurs ennemis
  * On suppose que l'on appelle cette méthode seulement si il n'y a plus d'ennemis sur le terrain
  * En conséquence, la deque passée en paramètre en d'abord vidé afin d'éviter d'éventuelle fuites de mémoires.
  * @param salve la deque à remplir d'ennemis
  * @param typeSalve le type de salve à créer. Doit correspondre à une constante donnée dans le header Vaisseau.hpp
  */
void Vaisseau::createSalve(deque<Vaisseau*>* salve, RenderWindow* window, int typeSalve) {
	salve->clear(); //Désallocation de mémoire
	Vaisseau* ennemi = NULL;
	switch(typeSalve) {
	case SALVE_ENNEMIS:
		for(int i = 0; i < NB_ENNEMIS_SALVE; i++) {
			ennemi = new Vaisseau();
			ennemi->SetPosition(
				sf::Randomizer::Random((int)(ennemi->GetSize().x / 2), (int)(window->GetWidth() - ennemi->GetSize().x)),
				sf::Randomizer::Random((int)(ennemi->GetSize().y / 2), (int)(window->GetHeight() / 2 - ennemi->GetSize().y))
			);
			salve->push_back(ennemi);
		}
		break;
	case SALVE_TOURELLES:
		for(int i = 0; i < NB_ENNEMIS_SALVE; i++) {
			ennemi = new Tourelle();
			ennemi->SetPosition(window->GetWidth() * i / NB_ENNEMIS_SALVE + ennemi->GetSize().x / 2, ennemi->GetSize().y);
			salve->push_back(ennemi);
		}
		break;
	case SALVE_CHERCHEURS:
		for(int i = 0; i < NB_ENNEMIS_SALVE; i++) {
			ennemi = new Chercheur();
			ennemi->SetPosition(window->GetWidth() * i / NB_ENNEMIS_SALVE + ennemi->GetSize().x / 2, ennemi->GetSize().y);
			salve->push_back(ennemi);
		}
		break;
	case SALVE_ENNEMIS_ET_TOURELLES:
		ennemi = new Tourelle();
		ennemi->SetPosition(window->GetWidth() / 3, window->GetHeight() / 3);
		salve->push_back(ennemi);
		ennemi = new Tourelle();
		ennemi->SetPosition(window->GetWidth() * 2 / 3, window->GetHeight() / 3);
		salve->push_back(ennemi);
		for(int i = 0; i < NB_ENNEMIS_SALVE - 2; i++) {
			ennemi = new Vaisseau();
			ennemi->SetPosition(window->GetWidth() * i / (NB_ENNEMIS_SALVE - 2) + ennemi->GetSize().x / 2, ennemi->GetSize().x);
			salve->push_back(ennemi);
		}
		break;
	case SALVE_CHERCHEURS_ET_TOURELLES:
		ennemi = new Tourelle();
		ennemi->SetPosition(window->GetWidth() / 3, window->GetHeight() / 3);
		salve->push_back(ennemi);
		ennemi = new Tourelle();
		ennemi->SetPosition(window->GetWidth() * 2 / 3, window->GetHeight() / 3);
		salve->push_back(ennemi);
		for(int i = 0; i < NB_ENNEMIS_SALVE - 2; i++) {
			ennemi = new Chercheur();
			ennemi->SetPosition(window->GetWidth() * i / (NB_ENNEMIS_SALVE - 2) + ennemi->GetSize().x / 2, ennemi->GetSize().x);
			salve->push_back(ennemi);
		}
		break;
	default:
		cout << "Cette salve n'existe pas !!!" << endl;
		break;
	}
}

bool Vaisseau::init() {
	bool allIsInited = true;
	/* *************** Chargement des images de tous les vaisseaux *************** */
	int nbImgInit = 4;
	std::string nomImg[4] = {"./images/joueur.png", "./images/ennemi.png", "./images/chercheur.png", "./images/tourelle.png"};
	Vaisseau::img = new Image[nbImgInit];
	for(int i = 0; i < nbImgInit; i++) {
		if(!img[i].LoadFromFile(nomImg[i])) {
			allIsInited = false;
		};
	}
	/* *************** Chargement du son lors d'un tir *************** */
	if(sbTir.LoadFromFile("./sons/tir.wav")) {
		sonTir.SetBuffer(sbTir);
	} else {
		allIsInited = false;
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
	SetRotation(degAngle);
}

void Vaisseau::incVie(int v) { vie += v; }
void Vaisseau::setVie(int v) { vie = v; }
int Vaisseau::getVie() { return vie; }
bool Vaisseau::estMort() { return vie <= 0; }
void Vaisseau::tirer(Jeu* jeu, float tempsEntre2Tirs) {
	if(dernierTir.GetElapsedTime() >= tempsEntre2Tirs) {
		Tir* t = new Tir(this);
		jeu->getTirs()->push_back(t);
		dernierTir.Reset();
		sonTir.Play();
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
  * Pour les ennemis "classiques", l'ia consiste simplement, toutes les 1.5 secondes, à:
  		* se déplacer à gauche ou droite au hasard (sous réserve que le vaisseau ne touche pas déjà un bord)
  		* tirer 
  */
void Vaisseau::ia(Jeu* jeu) {
	if(dernierTir.GetElapsedTime() >= 0.4f) {
		tirer(jeu, 0.4f);
		float dirIa = sf::Randomizer::Random(0, 2);
		RenderWindow* win = jeu->getWindow();
		if(dirIa >= 1.0f) {
			deplacer(win, -10, 0);
		} else {
			deplacer(win, 10, 0);
		}
	}
}