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
	SetPosition(v->GetPosition().x, v->GetPosition().y - (v->GetSize().y / 2) - GetSize().y);
	SetCenter(GetSize().x / 2, GetSize().y / 2);
}
Tir::~Tir() {}

bool Tir::init() {
	std::string nomImg = "./images/tir.png";
	img = Image();
	return (img.LoadFromFile(nomImg));
}

void Tir::deplace() {
	Move(/*GetRotation()*/ 0, direction * VITESSE_TIR);
}

