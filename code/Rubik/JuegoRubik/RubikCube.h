#ifndef RUBIK_CUBE_H_
#define RUBIK_CUBE_H_

#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <queue>
#include <algorithm>
#include "defineRubik.h"

// usa 26 shaders para cada cubo


class Cube
{
private:
	static const int NFACES = 6;
public:
	char colors[NFACES];
	~Cube();
};

class RubikCube3x3
{
public:
	enum class STATE_ANIMATION { NONE, F, f, U, u, L, l, R, r, B, b, D, d, SOLVE };
	enum class PAINT_MODE { BREATHE, STATIC };
private:
	static const int NCUBES = 27;
	static const int MAX_N_TEXTURES = 2;
	// cada cubo de rubik tiene estas variables
	Cube cubes[NCUBES]; // 9(cuadrante) * 3(nivel) = 27 cubos en total
	STATE_ANIMATION state_animation = STATE_ANIMATION::NONE;

	std::string cubeString;
	solver::Rubik rubikSolver;
	std::vector<char> solution;
	std::queue<STATE_ANIMATION> solutionStates;

	float angle = 0.0f, step = 0.0f; // estas variables se usan al momento de realizar los movimientos del cubo


	void HandleRubikMoves(char movement);

public:
	// MOVIMIENTOS DEL CUBO
	//// rota el FRONT del cubo en sentido horario
	bool F(float parts);
	//// rota el FRONT del cubo en sentido antihorario
	bool f(float parts);
	//// rota el RIGHT del cubo en sentido horario
	bool R(float parts);
	//// rota el RIGHT del cubo en sentido antihorario
	bool r(float parts);
	//// rota el UP del cubo en sentido horario
	bool U(float parts);
	//// rota el UP del cubo en sentido antihorario
	bool u(float parts);
	//// rota el BACK del cubo en sentido horario
	bool B(float parts);
	//// rota el BACK del cubo en sentido antihorario
	bool b(float parts);
	//// rota el LEFT del cubo en sentido horario
	bool L(float parts);
	//// rota el LEFT del cubo en sentido antihorario
	bool l(float parts);
	//// rota el DOWN del cubo en sentido horario
	bool D(float parts);
	//// rota el DOWN del cubo en sentido antihorario
	bool d(float parts);
};


#endif // !RUBIK_CUBE_H