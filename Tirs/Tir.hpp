#ifndef TIR_HPP
#define TIR_HPP

#include <SFML/Graphics.hpp>
#include "../Affichable.hpp"

class Vaisseau;
class Tir :
	public Affichable
{
public:
	const static int VITESSE_TIR = 12;
	Tir(int dir);
	Tir(int dir, int rotat, int x, int y);
	Tir(Vaisseau* v);
	~Tir();
	static bool init();
	void deplace();
private:
	static Image img;
};
#endif