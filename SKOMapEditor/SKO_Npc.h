#ifndef __SKO_NPC_H_
#define __SKO_NPC_H_

#include <vector>
#include <iostream>
#include <cstdio>
#include "TextComponent.h"

class SKO_Npc
{

public:
	SKO_Npc();
	void Respawn();

	//coords
	float x = 0;
	float y = 0;
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	float x_speed = 0;
	int y_speed = 0;
	int w = 0;
	int h = 0;
	//spawn
	int sx = 0;
	int sy = 0;
	unsigned long long int AI_ticker = 0;       
	unsigned long long int AI_period = 0;

	bool facing_right = true; 
	bool ground = false;
	int current_frame = 0;
	unsigned long long int animation_ticker = 0;

	int sprite = 0;
	int quest = 0;
	std::string finalPage = "";
	std::vector<OPI_Text::TextComponent*> lines;

	static const int NUM_LINES = 10;
	static const int MAX_PAGES = 10;
};

#endif

