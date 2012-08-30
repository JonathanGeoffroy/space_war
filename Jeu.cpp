#include "Jeu.hpp"
sf::Image Jeu::imgViseur = sf::Image();
sf::SoundBuffer Jeu::sbTir = sf::SoundBuffer();

Jeu::Jeu(RenderWindow* win) : 	joueur(Affichable::HAUT, Vaisseau::JOUEUR),
								sonTir(sbTir),
								musique()
{
	window = win;
	ennemis = deque<Vaisseau*>();
	tirs = deque<Tir*>();
	joueur.SetPosition(window->GetWidth() /2 - joueur.GetSize().x, window->GetHeight() - joueur.GetSize().y / 2);
	musique.OpenFromFile("./sons/musique.ogg");
	viseur.SetImage(imgViseur);
	viseur.SetPosition(sf::Vector2f(win->GetWidth()/2, win->GetHeight()/2));
}

Jeu::~Jeu() {}

bool Jeu::init() {
	return Jeu::imgViseur.LoadFromFile("./images/viseur.png") && 
		   Jeu::sbTir.LoadFromFile("./sons/tir.wav");
}

void Jeu::jouer() {
	bool continuer = true;
	Event event;
	Vaisseau* vaissTmp;
	musique.Play();
	while(continuer && ! joueur.estMort()) {
		while(window->GetEvent(event)) {
			switch(event.Type) {
			case Event::KeyPressed:
				switch(event.Key.Code) {
					case Key::Escape:
						continuer = false;
						break;
					case Key::Left:
					case Key::Q:
						joueur.deplacer(window, -Vaisseau::VITESSE_JOUEUR, 0);
						break;
					case Key::Right:
					case Key::D:
						joueur.deplacer(window, Vaisseau::VITESSE_JOUEUR, 0);
						break;
					default:break;
				}
				break;
			case Event::MouseMoved:
				viseur.SetPosition(event.MouseMove.X - (viseur.GetSize().x / 2), event.MouseMove.Y - (viseur.GetSize().y / 2));
				/*
				joueur.calculeRotation(
								joueur.GetPosition().y - viseur.GetPosition().y,
								viseur.GetPosition().x - joueur.GetPosition().x
							); // FIXME : ne fonctionne pas !
				*/
				break;
			case Event::MouseButtonPressed:
				if(event.MouseButton.Button == sf::Mouse::Left) {
					sonTir.Play();
					Tir *t = new Tir(&joueur);
					tirs.push_back(t);
				} else if (event.MouseButton.Button == sf::Mouse::Right) {

				}
				break;
			default: break;
			}
		}
		while(ennemis.size() < 5) {
			vaissTmp = new Vaisseau(Affichable::BAS, Vaisseau::ENNEMI);
			/* On place un nouvel ennemis au hasard sur l'ecran :
				en abscisse : n'importe où sur l'ecran
				en ordonnée : sur la première moitié de l'ecran
			*/
			vaissTmp->SetPosition(sf::Randomizer::Random(0, (int)(window->GetWidth() - vaissTmp->GetSize().x)), sf::Randomizer::Random(0, (int)(window->GetHeight() / 2 - vaissTmp->GetSize().y)));
			ennemis.push_back(vaissTmp);
		}
		changeTime();
		affiche();
	}
}

void Jeu::affiche() {
	window->Clear();
	window->Draw(joueur);
	for(deque<Vaisseau*>::iterator it = ennemis.begin(); it != ennemis.end(); ++it) {
		if(*it != NULL)
			window->Draw(*(*it));
	}
	for(deque<Tir*>::iterator it = tirs.begin(); it != tirs.end(); ++it) {
			window->Draw(*(*it));
	}
	window->Draw(viseur);
	window->Display();
}

void Jeu::changeTime() {
	/* ************************* Gestion du déplacement des tirs ************************* */
	Tir* currentTir;
	for(deque<Tir*>::iterator tir = tirs.begin(); tir != tirs.end(); ++tir) {
		currentTir = *tir;
		currentTir->deplace();
		if(currentTir->GetPosition().x + currentTir->GetSize().x < 0 || currentTir->GetPosition().x >= window->GetWidth()
		|| currentTir->GetPosition().y + currentTir->GetSize().y < 0 || currentTir->GetPosition().y >= window->GetHeight()) {
			tirs.erase(tir);
			break;
		}
		else if(currentTir->getDirection() == Affichable::HAUT) {
			for(deque<Vaisseau*>::iterator vaiss = ennemis.begin(); vaiss != ennemis.end(); ++vaiss) {
				if((*vaiss)->touche(currentTir)) {
					tirs.erase(tir);

					(*vaiss)->incVie(-110);
					if((*vaiss)->estMort()) {
						ennemis.erase(vaiss);
					}
					return; //On a fait exploser le missile : plus la peine de trouver d'autres victimes!
				}
			}
		} else {
			if(joueur.touche(currentTir)) {
				tirs.erase(tir);
				joueur.incVie(-10);
				return; // FIXME : ça plante sans ça: mais normalement on devrait continuer les vérifs !
			}
		}
	}

	/* ************************* Gestion de l'IA des vaisseaux ennemis ************************* */
	for(deque<Vaisseau*>::iterator vaiss = ennemis.begin(); vaiss != ennemis.end(); ++vaiss) {
		(*vaiss)->ia(this);
	}
}

deque<Vaisseau*>* Jeu::getEnnemis() { return &ennemis; }
deque<Tir*>* Jeu::getTirs() { return &tirs; }
Vaisseau* Jeu::getJoueur() { return &joueur; }
RenderWindow* Jeu::getWindow() { return window; }