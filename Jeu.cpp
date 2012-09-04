#include "Jeu.hpp"
sf::Image Jeu::imgViseur = sf::Image();

Jeu::Jeu(RenderWindow* win) : 	joueur(Affichable::HAUT, Vaisseau::JOUEUR),
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

Jeu::Jeu(const Jeu& j): joueur(Affichable::HAUT, Vaisseau::JOUEUR),
						musique()
{

}
Jeu::~Jeu() {
	ennemis.clear();
	tirs.clear();
}

bool Jeu::init() {
	return Jeu::imgViseur.LoadFromFile("./images/viseur.png");
}

void Jeu::reinitialisation() {
	joueur.setVie(100);
	joueur.SetPosition(window->GetWidth() /2 - joueur.GetSize().x, window->GetHeight() - joueur.GetSize().y / 2);
	viseur.SetPosition(sf::Vector2f(window->GetWidth()/2, window->GetHeight()/2));
	ennemis.clear();
	tirs.clear();
}

void Jeu::jouer() {
	bool continuer = true;
	Event event;
	Vaisseau* vaissTmp;
	musique.Play();
	bool enTrainDeTirer = false;
	int deplacementHorizontal = 0;
	float degres = 0;
	while(continuer && !joueur.estMort()) {
		while(window->GetEvent(event)) {
			switch(event.Type) {
			case Event::KeyPressed:
				switch(event.Key.Code) {
					case Key::Escape:
						continuer = false;
						break;
					case Key::Left:
					case Key::Q:
						deplacementHorizontal = -1;
						break;
					case Key::Right:
					case Key::D:
						deplacementHorizontal = 1;
						break;
					default:break;
				}
				break;
			case Event::KeyReleased:
				switch(event.Key.Code) {
					case Key::Left:
					case Key::Q:
					case Key::Right:
					case Key::D:
						deplacementHorizontal = 0;
						break;
					default: break;
				}
				break;

			case Event::MouseMoved:
				viseur.SetPosition(event.MouseMove.X - (viseur.GetSize().x / 2), event.MouseMove.Y - (viseur.GetSize().y / 2));
				degres = atan2f(
							-(viseur.GetPosition().x - joueur.GetPosition().x),
							joueur.GetPosition().y - viseur.GetPosition().y
						);
				degres = (degres * 180) / 3.14f;
				joueur.SetRotation(degres);
				break;
			case Event::MouseButtonPressed:
				if(event.MouseButton.Button == sf::Mouse::Left) {
					enTrainDeTirer = true;
				} else if (event.MouseButton.Button == sf::Mouse::Right) {

				}
				break;
			case Event::MouseButtonReleased:
				if(event.MouseButton.Button == sf::Mouse::Left) {
					enTrainDeTirer = false;
				}
			default: break;
			}
		}
		while(ennemis.size() < 5) {
			// TODO : implémentation de salves d'ennemis

			/* 
			  On place un nouvel ennemis au hasard sur l'ecran :
				en abscisse : n'importe où sur l'ecran
				en ordonnée : sur la première moitié de l'ecran
			*/
			switch(Randomizer::Random(1, 3)) {
				case Vaisseau::ENNEMI:
					vaissTmp = new Vaisseau();
					break;
				case Vaisseau::CHERCHEUR:
					vaissTmp = new Chercheur();
					break;
				case Vaisseau::TOURELLE:
					vaissTmp = new Tourelle();
					break;
			}
			vaissTmp->SetPosition(sf::Randomizer::Random((int)(vaissTmp->GetSize().x / 2), (int)(window->GetWidth() - vaissTmp->GetSize().x)), sf::Randomizer::Random((int)(vaissTmp->GetSize().y / 2), (int)(window->GetHeight() / 2 - vaissTmp->GetSize().y)));
			ennemis.push_back(vaissTmp);
		}
		/* 
		 Permet de déplacer le joueur, sans tenir compte du temps de latence de EnableKeyRepeat
		 Même chose pour le clic et le tir : on est plus obligé de cliquer plein de fois : on peut laisser appuyé!
		*/
		if(deplacementHorizontal != 0)
			joueur.deplacer(window, Vaisseau::VITESSE_JOUEUR * deplacementHorizontal, 0);
		if(enTrainDeTirer)
			joueur.tirer(this);

		changeTime();
		affiche();
		window->Display();
	}

	musique.Pause(); //Pour reprendre la chanson au même endroit si on décide de recommencer une partie
	/* ****************** Désalocation de la mémoire ****************** */
	//ennemis.clear();
	//tirs.clear();
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

bool Jeu::recommencerPartie() {
	int largeur = 400;
	int hauteur = 300;
	int debutX = window->GetWidth() - (largeur * 1.5f);
	int debutY = window->GetHeight() - (hauteur * 1.5f);
	Shape cadre = Shape::Rectangle(debutX, debutY, debutX + largeur, debutY + hauteur, Color(255, 255, 255, 128));
	String recommencer = String("Vous avouez-vous vaincu ?", Font::GetDefaultFont());
	recommencer.SetPosition(debutX, debutY);
	String reponseOui = String("Recommencer !!!", Font::GetDefaultFont());
	reponseOui.SetPosition(debutX, debutY + hauteur/3);
	String reponseNon = String("Abandonner ...", Font::GetDefaultFont());
	reponseNon.SetPosition(debutX, debutY + hauteur*2/3);
	Event event;
	window->ShowMouseCursor(true);

	while (true) {
		while(window->GetEvent(event)) {
			if(event.Type == Event::MouseButtonReleased) {
				//Si on clique dans le cadre :
				if(event.MouseButton.X >= debutX && event.MouseButton.X <= debutX + largeur
				  		&& event.MouseButton.Y >= debutY && event.MouseButton.Y <= debutY + hauteur) {
					if(event.MouseButton.Y >= reponseNon.GetPosition().y) {
						window->ShowMouseCursor(false);
						return false;
					} else if (event.MouseButton.Y >= reponseOui.GetPosition().y) {
						window->ShowMouseCursor(false);
						return true;
					}
				}
			}
		}
		window->Clear();
		affiche();
		window->Draw(cadre);
		window->Draw(recommencer);
		window->Draw(reponseOui);
		window->Draw(reponseNon);
		window->Display();
	}
}

deque<Vaisseau*>* Jeu::getEnnemis() { return &ennemis; }
deque<Tir*>* Jeu::getTirs() { return &tirs; }
Vaisseau* Jeu::getJoueur() { return &joueur; }
RenderWindow* Jeu::getWindow() { return window; }
