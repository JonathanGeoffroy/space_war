#include "Affichable.hpp"
RenderWindow* Affichable::window = NULL;

Affichable::Affichable(int dir) : Sprite() {
	direction = dir;
	int rotation;
	if(dir == HAUT)
		rotation = 0;
	else
		rotation = 180;
	SetRotation(rotation);
}

Affichable::Affichable(int dir, int rotat) {
	direction = dir;
	SetRotation(rotat);
}

Affichable::~Affichable() {}
/*
bool Affichable::initAll(RenderWindow *win) {
	window = win;
	return (Vaisseau::init() &&	Tir::init());
}
*/
void Affichable::setWindow(RenderWindow* win) {
	window = win;
}

int Affichable::getDirection() { return direction; }
void Affichable::setDirection(int dir) { direction = dir; }