#include "Tir.hpp"
#include "../Vaisseaux/Vaisseau.hpp"
Image Tir::img = Image();
Tir::Tir(int dir) : Affichable(dir) {
	SetImage(img);
	SetCenter(GetSize().x / 2, GetSize().y / 2);
}
Tir::Tir(int dir, int rotat, int x, int y) : Affichable(dir, rotat) {
	SetImage(img);
	SetPosition(x, y);
	SetCenter(GetSize().x / 2, GetSize().y / 2);
}
Tir::Tir(Vaisseau* v) : Affichable(v->getDirection(), v->GetRotation()) {
	SetImage(img);
	if(v->getDirection() == HAUT)
		SetPosition(v->GetPosition().x, v->GetPosition().y - (v->GetSize().y / 2) - (GetSize().y / 2));
	else
		SetPosition(v->GetPosition().x, v->GetPosition().y + (v->GetSize().y / 2) + (GetSize().y / 2));
	SetCenter(GetSize().x / 2, GetSize().y / 2);
}
Tir::~Tir() {}

bool Tir::init() {
	std::string nomImg = "./images/tir.png";
	img = Image();
	return (img.LoadFromFile(nomImg));
}

void Tir::deplace() {
	int y = direction * VITESSE_TIR;
	int x = (int)( y * tanf(GetRotation() * (3.14f / 180)));
	Move(x, y);
}

