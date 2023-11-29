#include	"CUndulationPlaneMesh.h"

void CUndulationPlaneMesh::MakeUndulation(float min,float max)
{
		std::mt19937 mt;
		std::uniform_real_distribution<float> dist(min, max);

		for (auto& v : m_vertices) {
			v.Position.y = dist(mt);
		}

}

void CUndulationPlaneMesh::MakeUndulationPerlinnoise(float max,double octave,double persistence)
{
	PerlinNoise perlin;
	for (auto& v : m_vertices) {

//		double d = perlin.noise(
//			v.Position.x*0.1,
//			0,
//			v.Position.z);

		double d = perlin.octaveNoise(
			v.Position.x,
			0,
			v.Position.z,
			8,
			0.98);

		v.Position.y = d*max;
	}

}


