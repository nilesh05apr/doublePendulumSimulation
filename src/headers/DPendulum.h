#pragma once
#ifndef __DPENDULUM_H
#define __DPENDULUM_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <utility>

class DPendulum: public sf::Sprite {
private:
	std::pair<float, float> angle;
	std::pair<float, float> damp;
	std::pair<float, float> length;
	std::pair<float, float> angacl;	
	std::pair<float, float> angvel;	
	std::pair<float, float> mass;	
	std::pair<float, float> posx;	
	std::pair<float, float> posy;
	std::pair<bool, bool> moving;
	bool hold = false;
	bool showrod = true;

	sf::RenderTexture textr;
	sf::Vertex rvertices[3];
	sf::VertexBuffer vb;
	std::pair<sf::CircleShape, sf::CircleShape> bob;
	std::vector<sf::Vertex> trails;

	

	void updatexy();
	int bobclicked(sf::Vector2i mousepos);


public:
	DPendulum(float len_1, float len_2, float mass_1, float mass_2, float angle_1, float angle_2,float damp_1, float damp_2);
	void setupRenderObjects(float width, float height);

	void update();
	void render();

	void changestate();
	void changemoving(int id);
	void movebob(sf::Vector2i mousepos);
	void clicked(sf::Vector2i mousepos);

	void rodvisibility();
};


#endif __DPENDULUM_H