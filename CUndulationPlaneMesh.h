#pragma once
#include	<random>
#include	"CPlaneMesh.h"
#include	"Perlinnoise.h"
#include	"CPlane.h"

class CUndulationPlaneMesh : public CPlaneMesh {
public:
	void MakeUndulation(float min, float max);
	void MakeUndulationPerlinnoise(float max, double octave, double persistence);
};
