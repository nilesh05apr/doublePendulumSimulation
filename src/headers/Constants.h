#pragma once
#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <cmath>
#include <math.h>

const float g = 9.8; //gravitational constant
const float FPS = 50; //frame rate
const float PI = 3.141592653589793238;
const int trailsize = 5000;


struct SinglePendulum {
	const float damp = 1;
};

struct DoublePendulum {
	const float damp[2] = { 1.0,1.0 };
};


#endif __CONSTANTS_H
