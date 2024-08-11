#include "RubikCube.h"


// class cube initializations
Cube::Cube()
{
	model = glm::mat4(1.0f);
	pos = glm::vec3(0.0f); // at the origin
	colors[0] = 'W'; // UP - White
	colors[1] = 'O'; // LEFT - Orange
	colors[2] = 'G'; // FRONT - Green
	colors[3] = 'R'; // RIGHT - Red
	colors[4] = 'B'; // BACK - Blue
	colors[5] = 'Y'; // DOWN - Yellow
}

Cube::Cube(const Cube& another_cube)
{
	pos = another_cube.pos;
	model = another_cube.model;
	for(int i = 0; i < NFACES; ++i)
		colors[i] = another_cube.colors[i];
}

Cube::Cube(glm::mat4 model_, glm::vec3 pos_)
{
	model = model_;
	pos = pos_;
	colors[0] = 'W'; // UP - White
	colors[1] = 'O'; // LEFT - Orange
	colors[2] = 'G'; // FRONT - Green
	colors[3] = 'R'; // RIGHT - Red
	colors[4] = 'B'; // BACK - Blue
	colors[5] = 'Y'; // DOWN - Yellow
}


Cube::~Cube() {}


///////////////////////////////////////////////////////////////////
// RUBIK CUBE 3X3
///////////////////////////////////////////////////////////////////

///////////////////////
// PRIVATE INTERFACE //
// ////////////////////
// Aplica los movimientos al solver y se encarga de eliminar los movimientos
// cuando el solver se está resolviendo

// orientation: h significa horario, a significa antihorario
void RubikCube3x3::CalculateRotation(float& parts, glm::vec3& axis, glm::mat4& rotationMatrix, char orientation)
{
	step = (angle + step >= 90.0f) ? 90.0f - angle : 90.0f / parts;
	// en este caso rotaremos por el step
	float w = cosf(glm::radians((orientation == 'h' ? -step : step) / 2));
	float v = sinf(glm::radians((orientation == 'h' ? -step : step) / 2));
	glm::vec3 rotAxis = v * axis;
	glm::quat q(w, rotAxis); // usamos un quaternion para las rotaciones
	rotationMatrix = glm::mat4_cast(q); // casteamos la rotación del quaternion a matriz
}

//////////////////////
// PUBLIC INTERFACE //
//////////////////////
RubikCube3x3::RubikCube3x3()
{
	// inicializando los atributos
	RVAO = 0; // valor por defecto del VAO
	global_model = glm::mat4(1.0f);
	inverse_global_model = glm::inverse(global_model);


	// primer nivel
	// posiciones
	cubes[0].pos = glm::vec3(-1, 1, -1);
	cubes[1].pos = glm::vec3(0, 1, -1);
	cubes[2].pos = glm::vec3(1, 1, -1);
	cubes[3].pos = glm::vec3(-1, 1, 0);
	cubes[4].pos = glm::vec3(0, 1, 0);
	cubes[5].pos = glm::vec3(1, 1, 0);
	cubes[6].pos = glm::vec3(-1, 1, 1);
	cubes[7].pos = glm::vec3(0, 1, 1);
	cubes[8].pos = glm::vec3(1, 1, 1);
	// colores negros L se interpreta como el color negro
	cubes[0].colors[2] = 'L', cubes[0].colors[3] = 'L', cubes[0].colors[5] = 'L';
	cubes[1].colors[1] = 'L', cubes[1].colors[2] = 'L', cubes[1].colors[3] = 'L', cubes[1].colors[5] = 'L';
	cubes[2].colors[1] = 'L', cubes[2].colors[2] = 'L', cubes[2].colors[5] = 'L';
	cubes[3].colors[2] = 'L', cubes[3].colors[3] = 'L', cubes[3].colors[4] = 'L', cubes[3].colors[5] = 'L';
	cubes[4].colors[1] = 'L', cubes[4].colors[2] = 'L', cubes[4].colors[3] = 'L', cubes[4].colors[4] = 'L', cubes[4].colors[5] = 'L';
	cubes[5].colors[1] = 'L', cubes[5].colors[2] = 'L', cubes[5].colors[4] = 'L', cubes[5].colors[5] = 'L';
	cubes[6].colors[3] = 'L', cubes[6].colors[4] = 'L', cubes[6].colors[5] = 'L';
	cubes[7].colors[1] = 'L', cubes[7].colors[3] = 'L', cubes[7].colors[4] = 'L', cubes[7].colors[5] = 'L';
	cubes[8].colors[1] = 'L', cubes[8].colors[4] = 'L', cubes[8].colors[5] = 'L';

	// segundo nivel
	// posiciones
	cubes[9].pos = glm::vec3(-1, 0, -1);
	cubes[10].pos = glm::vec3(0, 0, -1);
	cubes[11].pos = glm::vec3(1, 0, -1);
	cubes[12].pos = glm::vec3(-1, 0, 0);
	// cubes[13].model = glm::vec3(0, 0, 0); omitimos el 13, porque es el centro y no lo necesitamos
	cubes[14].pos = glm::vec3(1, 0, 0);
	cubes[15].pos = glm::vec3(-1, 0, 1);
	cubes[16].pos = glm::vec3(0, 0, 1);
	cubes[17].pos = glm::vec3(1, 0, 1);
	// colores
	cubes[9].colors[0] = 'L', cubes[9].colors[2] = 'L', cubes[9].colors[3] = 'L', cubes[9].colors[5] = 'L';
	cubes[10].colors[0] = 'L', cubes[10].colors[1] = 'L', cubes[10].colors[2] = 'L', cubes[10].colors[3] = 'L', cubes[10].colors[5] = 'L';
	cubes[11].colors[0] = 'L', cubes[11].colors[1] = 'L', cubes[11].colors[2] = 'L', cubes[11].colors[5] = 'L';
	cubes[12].colors[0] = 'L', cubes[12].colors[2] = 'L', cubes[12].colors[3] = 'L', cubes[12].colors[4] = 'L', cubes[12].colors[5] = 'L';
	cubes[14].colors[0] = 'L', cubes[14].colors[1] = 'L', cubes[14].colors[2] = 'L', cubes[14].colors[4] = 'L', cubes[14].colors[5] = 'L';
	cubes[15].colors[0] = 'L', cubes[15].colors[3] = 'L', cubes[15].colors[4] = 'L', cubes[15].colors[5] = 'L';
	cubes[16].colors[0] = 'L', cubes[16].colors[1] = 'L', cubes[16].colors[3] = 'L', cubes[16].colors[4] = 'L', cubes[16].colors[5] = 'L';
	cubes[17].colors[0] = 'L', cubes[17].colors[1] = 'L', cubes[17].colors[4] = 'L', cubes[17].colors[5] = 'L';

	// tercer nivel
	// posiciones
	cubes[18].pos = glm::vec3(-1, -1, -1);
	cubes[19].pos = glm::vec3(0, -1, -1);
	cubes[20].pos = glm::vec3(1, -1, -1);
	cubes[21].pos = glm::vec3(-1, -1, 0);
	cubes[22].pos = glm::vec3(0, -1, 0);
	cubes[23].pos = glm::vec3(1, -1, 0);
	cubes[24].pos = glm::vec3(-1, -1, 1);
	cubes[25].pos = glm::vec3(0, -1, 1);
	cubes[26].pos = glm::vec3(1, -1, 1);
	// colores
	cubes[18].colors[0] = 'L', cubes[18].colors[2] = 'L', cubes[18].colors[3] = 'L';
	cubes[19].colors[0] = 'L', cubes[19].colors[1] = 'L', cubes[19].colors[2] = 'L', cubes[19].colors[3] = 'L';
	cubes[20].colors[0] = 'L', cubes[20].colors[1] = 'L', cubes[20].colors[2] = 'L';
	cubes[21].colors[0] = 'L', cubes[21].colors[2] = 'L', cubes[21].colors[3] = 'L', cubes[21].colors[4] = 'L';
	cubes[22].colors[0] = 'L', cubes[22].colors[1] = 'L', cubes[22].colors[2] = 'L', cubes[22].colors[3] = 'L', cubes[22].colors[4] = 'L';
	cubes[23].colors[0] = 'L', cubes[23].colors[1] = 'L', cubes[23].colors[2] = 'L', cubes[23].colors[4] = 'L';
	cubes[24].colors[0] = 'L', cubes[24].colors[3] = 'L', cubes[24].colors[4] = 'L';
	cubes[25].colors[0] = 'L', cubes[25].colors[1] = 'L', cubes[25].colors[3] = 'L', cubes[25].colors[4] = 'L';
	cubes[26].colors[0] = 'L', cubes[26].colors[1] = 'L', cubes[26].colors[4] = 'L';

	for (int i = 0; i < NCUBES; ++i) {
		if (i == 13) continue;
		cubes[i].pos += 0.05f * cubes[i].pos;
		animator.init_pos[i] = cubes[i].pos;
	}


	// ahora configuramos como está el cubo por primera vez
	// esta es la configuración inicial del solver del cubo de rubik
	cubeString = "w w w w w w w w w\n"
		"o o o o o o o o o\n"
		"g g g g g g g g g\n"
		"r r r r r r r r r\n"
		"l l l l l l l l l\n"
		"y y y y y y y y y";
	// ingresamos este string al stringstream
	std::stringstream ssCubeString;
	ssCubeString << cubeString;
	// ingresamos este stringstream al solver para su posterior resolución
	ssCubeString >> rubikSolver;
}


// FUNCIONES PARA EL DIBUJO DEL CUBO Y ANIMACIONES
// configura las matrices modelos de cada cubo del cubo de rubik, 
// también se configura una transformación global a todos los cubos, de ser el caso
void RubikCube3x3::ApplyTransformation(glm::mat4 glob_trans)
{
	// aplicando la transformación global a todos los cubos
	global_model = glob_trans;
	inverse_global_model = glm::inverse(global_model);
}
// pasamos como parámetros las matrices de vista y proyección globales
void RubikCube3x3::HandleDrawing(glm::mat4& view, glm::mat4& projection, STATE_ANIMATION& move_state, PAINT_MODE& animation)
{
	static float turbo = 0.70f;
	float parts = 90.0f * (1.0f - turbo);
	

	switch (move_state)
	{
		

		case STATE_ANIMATION::F:
		{
			
			if (F(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('F');
				
			}
			break;
		}
		case STATE_ANIMATION::f:
		{
			if (f(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('f');
			}
			break;
		}
		case STATE_ANIMATION::R:
		{
			if (R(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('R');
			}
			break;
		}
		case STATE_ANIMATION::r:
		{
			if (r(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('r');
			}
			break;
		}
		case STATE_ANIMATION::U:
		{
			if (U(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('U');
			}
			break;
		}
		case STATE_ANIMATION::u:
		{
			if (u(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('u');
			}
			break;
		}
		case STATE_ANIMATION::B:
		{
			if (B(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('B');
				
			}
			break;
		}
		case STATE_ANIMATION::b:
		{
			if (b(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('b');
			}
			break;
		}
		case STATE_ANIMATION::L:
		{
			if (L(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('L');
				
			}
			break;
		}
		case STATE_ANIMATION::l:
		{
			if (l(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('l');
				
			}
			break;
		}
		case STATE_ANIMATION::D:
		{
			if (D(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('D');
			}
			break;
		}
		case STATE_ANIMATION::d:
		{
			if (d(parts)) {
				move_state = solutionStates.empty() ? STATE_ANIMATION::NONE : solutionStates.front();
				HandleRubikMoves('d');
			}
			break;
		}
	}
	
	switch (animation)
	{
		case PAINT_MODE::STATIC: {
			for (int i = 0; i < NCUBES; ++i) {
				if (i == 13) continue;
				cubes[i].pos = animator.scalar * cubes[i].pos;
			}
			break;
		}
	}

	DrawCube(view, projection);
	
}
