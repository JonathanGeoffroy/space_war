#ifndef AFFICHABLE_HPP
#define AFFICHABLE_HPP

#include <SFML/Graphics.hpp>
using namespace sf;
class Affichable : 
	public Sprite {
public:
	static RenderWindow* window;
	const static int HAUT = -1, BAS = 1;
	Affichable(int dir);
	Affichable(int dir, int rotat);
	~Affichable();
	void setWindow(RenderWindow* win);
	int getDirection();
	void setDirection(int dir);
protected:
	int direction;
};
#endif