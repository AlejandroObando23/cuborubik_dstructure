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
// cuando el solver se est� resolviendo

// orientation: h significa horario, a significa antihorario
void RubikCube3x3::CalculateRotation(float& parts, glm::vec3& axis, glm::mat4& rotationMatrix, char orientation)
{
	step = (angle + step >= 90.0f) ? 90.0f - angle : 90.0f / parts;
	// en este caso rotaremos por el step
	float w = cosf(glm::radians((orientation == 'h' ? -step : step) / 2));
	float v = sinf(glm::radians((orientation == 'h' ? -step : step) / 2));
	glm::vec3 rotAxis = v * axis;
	glm::quat q(w, rotAxis); // usamos un quaternion para las rotaciones
	rotationMatrix = glm::mat4_cast(q); // casteamos la rotaci�n del quaternion a matriz
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


	// ahora configuramos como est� el cubo por primera vez
	// esta es la configuraci�n inicial del solver del cubo de rubik
	cubeString = "w w w w w w w w w\n"
		"o o o o o o o o o\n"
		"g g g g g g g g g\n"
		"r r r r r r r r r\n"
		"l l l l l l l l l\n"
		"y y y y y y y y y";
	// ingresamos este string al stringstream
	std::stringstream ssCubeString;
	ssCubeString << cubeString;
	// ingresamos este stringstream al solver para su posterior resoluci�n
	ssCubeString >> rubikSolver;
}


// FUNCIONES PARA EL DIBUJO DEL CUBO Y ANIMACIONES
// configura las matrices modelos de cada cubo del cubo de rubik, 
// tambi�n se configura una transformaci�n global a todos los cubos, de ser el caso
void RubikCube3x3::ApplyTransformation(glm::mat4 glob_trans)
{
	// aplicando la transformaci�n global a todos los cubos
	global_model = glob_trans;
	inverse_global_model = glm::inverse(global_model);
}
// pasamos como par�metros las matrices de vista y proyecci�n globales
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

void RubikCube3x3::Solve(STATE_ANIMATION& some_state)
{
	//std::stringstream ss;
	//ss << rubikSolver;
	//std::cout << "Cube form to solve: \n";
	//std::cout << ss.str() << "\n";
	rubikSolver.solve(solution);
	//std::cout << "Solution: \n";
	for (int movement = 0; movement < solution.size(); ++movement) {
		//std::cout << solution[movement] << " ";
		if (solution[movement] == 'F')
			solutionStates.push(STATE_ANIMATION::F);
		else if (solution[movement] == 'f')
			solutionStates.push(STATE_ANIMATION::f);
		else if (solution[movement] == 'W')
			solutionStates.push(STATE_ANIMATION::B);
		else if (solution[movement] == 'w')
			solutionStates.push(STATE_ANIMATION::b);
		else if (solution[movement] == 'R')
			solutionStates.push(STATE_ANIMATION::R);
		else if (solution[movement] == 'r')
			solutionStates.push(STATE_ANIMATION::r);
		else if (solution[movement] == 'L')
			solutionStates.push(STATE_ANIMATION::L);
		else if (solution[movement] == 'l')
			solutionStates.push(STATE_ANIMATION::l);
		else if (solution[movement] == 'U')
			solutionStates.push(STATE_ANIMATION::U);
		else if (solution[movement] == 'u')
			solutionStates.push(STATE_ANIMATION::u);
		else if (solution[movement] == 'D')
			solutionStates.push(STATE_ANIMATION::D);
		else if (solution[movement] == 'd')
			solutionStates.push(STATE_ANIMATION::d);
	}
	//std::cout << "\n\n";
	solution.clear();
	// una vez contruida la soluci�n, se pone el primer estado
	some_state = solutionStates.front();
	solutionStates.pop(); // un pop porque ya consumimos un movimiento
}


void RubikCube3x3::AssociateVAO(GLuint VAO)
{
	RVAO = VAO;
}


// retorna verdadero cuando la animaci�n ha terminado
bool RubikCube3x3::f(float parts)
{
	glm::mat4 R(1.0f);
	glm::vec3 z = glm::vec3(0.0f, 0.0f, 1.0f);
	CalculateRotation(parts, z, R, 'a');
	angle += step;

	cubes[6].model = R * cubes[6].model;
	cubes[7].model = R * cubes[7].model;
	cubes[8].model = R * cubes[8].model;
	cubes[15].model = R * cubes[15].model;
	cubes[16].model = R * cubes[16].model;
	cubes[17].model = R * cubes[17].model;
	cubes[24].model = R * cubes[24].model;
	cubes[25].model = R * cubes[25].model;
	cubes[26].model = R * cubes[26].model;
	if (angle >= 90)
	{
		angle = 0.0f;
		// ahora debemos actualizar las coordenadas y modelos de los cubos que cambian por la rotaci�n
		Cube tmp_6 = cubes[6], tmp_7 = cubes[7], tmp_8 = cubes[8];
		Cube tmp_15 = cubes[15], tmp_17 = cubes[17];
		Cube tmp_24 = cubes[24], tmp_25 = cubes[25], tmp_26 = cubes[26];
		cubes[6] = tmp_8, cubes[7] = tmp_17, cubes[8] = tmp_26;
		cubes[15] = tmp_7, cubes[17] = tmp_25;
		cubes[24] = tmp_6, cubes[25] = tmp_15, cubes[26] = tmp_24;
		return true;
	}
	return false;
}


bool RubikCube3x3::r(float parts)
{
	glm::mat4 R(1.0f);
	glm::vec3 x = glm::vec3(1.0f, 0.0f, 0.0f);
	CalculateRotation(parts, x, R, 'a');
	angle += step;
	cubes[8].model = R * cubes[8].model;
	cubes[5].model = R * cubes[5].model;
	cubes[2].model = R * cubes[2].model;
	cubes[17].model = R * cubes[17].model;
	cubes[14].model = R * cubes[14].model;
	cubes[11].model = R * cubes[11].model;
	cubes[26].model = R * cubes[26].model;
	cubes[23].model = R * cubes[23].model;
	cubes[20].model = R * cubes[20].model;
	if (angle >= 90)
	{
		angle = 0.0f;
		Cube tmp_8 = cubes[8], tmp_5 = cubes[5], tmp_2 = cubes[2];
		Cube tmp_17 = cubes[17], tmp_11 = cubes[11];
		Cube tmp_26 = cubes[26], tmp_23 = cubes[23], tmp_20 = cubes[20];
		cubes[8] = tmp_2, cubes[5] = tmp_11, cubes[2] = tmp_20;
		cubes[17] = tmp_5, cubes[11] = tmp_23;
		cubes[26] = tmp_8, cubes[23] = tmp_17, cubes[20] = tmp_26;
		return true;
	}
	return false;
}


bool RubikCube3x3::u(float parts)
{
	glm::mat4 R;
	glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);
	CalculateRotation(parts, y, R, 'a');
	angle += step;
	cubes[0].model = R * cubes[0].model;
	cubes[1].model = R * cubes[1].model;
	cubes[2].model = R * cubes[2].model;
	cubes[3].model = R * cubes[3].model;
	cubes[4].model = R * cubes[4].model;
	cubes[5].model = R * cubes[5].model;
	cubes[6].model = R * cubes[6].model;
	cubes[7].model = R * cubes[7].model;
	cubes[8].model = R * cubes[8].model;
	if (angle >= 90)
	{
		angle = 0.0f;
		Cube tmp_0 = cubes[0], tmp_1 = cubes[1], tmp_2 = cubes[2];
		Cube tmp_3 = cubes[3], tmp_5 = cubes[5];
		Cube tmp_6 = cubes[6], tmp_7 = cubes[7], tmp_8 = cubes[8];
		cubes[0] = tmp_2, cubes[1] = tmp_5, cubes[2] = tmp_8;
		cubes[3] = tmp_1, cubes[5] = tmp_7;
		cubes[6] = tmp_0, cubes[7] = tmp_3, cubes[8] = tmp_6;
		return true;
	}
	return false;
}

bool RubikCube3x3::b(float parts)
{
	glm::mat4 R(1.0f);
	glm::vec3 z = glm::vec3(0.0f, 0.0f, -1.0f); // z negativo
	CalculateRotation(parts, z, R, 'a');
	angle += step;
	cubes[2].model = R * cubes[2].model;
	cubes[1].model = R * cubes[1].model;
	cubes[0].model = R * cubes[0].model;
	cubes[11].model = R * cubes[11].model;
	cubes[10].model = R * cubes[10].model;
	cubes[9].model = R * cubes[9].model;
	cubes[20].model = R * cubes[20].model;
	cubes[19].model = R * cubes[19].model;
	cubes[18].model = R * cubes[18].model;
	if (angle >= 90)
	{
		angle = 0.0f;
		Cube tmp_2 = cubes[2], tmp_1 = cubes[1], tmp_0 = cubes[0];
		Cube tmp_11 = cubes[11], tmp_9 = cubes[9];
		Cube tmp_20 = cubes[20], tmp_19 = cubes[19], tmp_18 = cubes[18];
		cubes[2] = tmp_0, cubes[1] = tmp_9, cubes[0] = tmp_18;
		cubes[11] = tmp_1, cubes[9] = tmp_19;
		cubes[20] = tmp_2, cubes[19] = tmp_11, cubes[18] = tmp_20;
		return true;
	}
	return false;
}

bool RubikCube3x3::l(float parts)
{
	glm::mat4 R(1.0f);
	glm::vec3 x = glm::vec3(-1.0f, 0.0f, 0.0f); // negative x
	CalculateRotation(parts, x, R, 'a');
	angle += step;
	cubes[0].model = R * cubes[0].model;
	cubes[3].model = R * cubes[3].model;
	cubes[6].model = R * cubes[6].model;
	cubes[9].model = R * cubes[9].model;
	cubes[12].model = R * cubes[12].model;
	cubes[15].model = R * cubes[15].model;
	cubes[18].model = R * cubes[18].model;
	cubes[21].model = R * cubes[21].model;
	cubes[24].model = R * cubes[24].model;
	if (angle >= 90)
	{
		angle = 0.0f;
		Cube tmp_0 = cubes[0], tmp_3 = cubes[3], tmp_6 = cubes[6];
		Cube tmp_9 = cubes[9], tmp_15 = cubes[15];
		Cube tmp_18 = cubes[18], tmp_21 = cubes[21], tmp_24 = cubes[24];
		cubes[0] = tmp_6, cubes[3] = tmp_15, cubes[6] = tmp_24;
		cubes[9] = tmp_3, cubes[15] = tmp_21;
		cubes[18] = tmp_0, cubes[21] = tmp_9, cubes[24] = tmp_18;
		return true;
	}
	return false;
}


bool RubikCube3x3::d(float parts)
{
	glm::mat4 R(1.0f);
	glm::vec3 y = glm::vec3(0.0f, -1.0f, 0.0f); // y negativo
	CalculateRotation(parts, y, R, 'a');
	angle += step;
	cubes[24].model = R * cubes[24].model;
	cubes[25].model = R * cubes[25].model;
	cubes[26].model = R * cubes[26].model;
	cubes[21].model = R * cubes[21].model;
	cubes[22].model = R * cubes[22].model;
	cubes[23].model = R * cubes[23].model;
	cubes[18].model = R * cubes[18].model;
	cubes[19].model = R * cubes[19].model;
	cubes[20].model = R * cubes[20].model;
	if (angle >= 90)
	{
		angle = 0.0f;
		Cube tmp_24 = cubes[24], tmp_25 = cubes[25], tmp_26 = cubes[26];
		Cube tmp_21 = cubes[21], tmp_23 = cubes[23];
		Cube tmp_18 = cubes[18], tmp_19 = cubes[19], tmp_20 = cubes[20];
		cubes[24] = tmp_26, cubes[25] = tmp_23, cubes[26] = tmp_20;
		cubes[21] = tmp_25, cubes[23] = tmp_19;
		cubes[18] = tmp_24, cubes[19] = tmp_21, cubes[20] = tmp_18;
		return true;
	}
	return false;
}