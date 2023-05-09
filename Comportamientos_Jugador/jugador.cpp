#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

ubicacion nextCasilla(const ubicacion &pos)
{
	ubicacion next = pos;
	switch (pos.brujula)
	{
	// Dependiendo de la orientación, sumaremos o restaremos a f o c si avanzamos
	case norte:
		next.f--;
		break;
	case noreste:
		next.f--;
		next.c++;
		break;
	case este:
		next.c++;
		break;
	case sureste:
		next.f++;
		next.c++;
		break;
	case sur:
		next.f++;
		break;
	case suroeste:
		next.f++;
		next.c--;
		break;
	case oeste:
		next.c--;
		break;
	case noroeste:
		next.f--;
		next.c--;
		break;
	default:
		break;
	}
	return next;
};

bool Find(const stateN0 &item, const list<nodeN0> &lista)
{
	auto it = lista.begin();
	while (it != lista.end() and !((*it).st == item))
		it++;

	return (!(it == lista.end()));
};

bool casillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
{
	// Devolvemos si la casilla es transitable o no
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
};

// Tres parámetros de entrada: la acción que se quiere realizar, el estado actual y el mapa del juego
// Devuelve el estado que se genera si se puede avanzar, si no, devuelve como salida el mismo estado que el de entrada
// Lo mismo pero para las acciones del sonánmbulo.
stateN0 apply(const Action &accion, const stateN0 &state, const vector<vector<unsigned char>> &mapa)
{
	stateN0 st_result = state;
	ubicacion sig_ubicacion;
	switch (accion)
	{
	case actFORWARD:
		sig_ubicacion = nextCasilla(state.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == state.sonambulo.f and sig_ubicacion.c == state.sonambulo.c))
		{
			st_result.jugador = sig_ubicacion;
		}
		break;
	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
		break;
	case actTURN_R:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 2) % 8);
		break;
	case actSON_FORWARD:
		sig_ubicacion = nextCasilla(state.sonambulo);
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == state.jugador.f and sig_ubicacion.c == state.jugador.c))
		{
			st_result.sonambulo = sig_ubicacion;
		}
		break;
	case actSON_TURN_SL:
		st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 7) % 8);
		break;
	case actSON_TURN_SR:
		st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 1) % 8);
		break;
	default:
		break;
	}
	return st_result;
};

estado apply(const Action &accion, const estado &state, const vector<vector<unsigned char>> &mapa, char terreno)
{
	estado st_result = state;
	ubicacion sig_ubicacion;
	int coste = 0;
	switch (accion)
	{
	case actFORWARD:
		sig_ubicacion = nextCasilla(state.nodo.st.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == state.nodo.st.sonambulo.f and sig_ubicacion.c == state.nodo.st.sonambulo.c))
		{
			st_result.nodo.st.jugador = sig_ubicacion;
		}
		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (terreno)
		{
		case 'A':
			state.hasBikini ? coste = 10 : 100;
			break;
		case 'B':
			state.hasZapatillas ? coste = 15 : 50;
			break;
		case 'T':
			coste = 2;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = st_result.g + coste;
	case actTURN_L:
		st_result.nodo.st.jugador.brujula = static_cast<Orientacion>((st_result.nodo.st.jugador.brujula + 6) % 8);
		
		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (terreno)
		{
		case 'A':
			state.hasBikini ? coste = 25 : 5;
			break;
		case 'B':
			state.hasZapatillas ? coste = 5 : 1;
			break;
		case 'T':
			coste = 2;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = st_result.g + coste;
		break;
	case actTURN_R:
		st_result.nodo.st.jugador.brujula = static_cast<Orientacion>((st_result.nodo.st.jugador.brujula + 2) % 8);
		
		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (terreno)
		{
		case 'A':
			state.hasBikini ? coste = 25 : 5;
			break;
		case 'B':
			state.hasZapatillas ? coste = 5 : 1;
			break;
		case 'T':
			coste = 2;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = st_result.g + coste;
		break;
	case actSON_FORWARD:
		sig_ubicacion = nextCasilla(state.nodo.st.sonambulo);
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == state.nodo.st.jugador.f and sig_ubicacion.c == state.nodo.st.jugador.c))
		{
			st_result.nodo.st.sonambulo = sig_ubicacion;
		}
		
		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (terreno)
		{
		case 'A':
			state.hasBikini ? coste = 10 : 100;
			break;
		case 'B':
			state.hasZapatillas ? coste = 15 : 50;
			break;
		case 'T':
			coste = 2;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = st_result.g + coste;
		break;
	case actSON_TURN_SL:
		st_result.nodo.st.sonambulo.brujula = static_cast<Orientacion>((st_result.nodo.st.sonambulo.brujula + 7) % 8);
		
		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (terreno)
		{
		case 'A':
			state.hasBikini ? coste = 7 : 2;
			break;
		case 'B':
			state.hasZapatillas ? coste = 3 : 1;
			break;
		case 'T':
			coste = 1;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = st_result.g + coste;
		break;
	case actSON_TURN_SR:
		st_result.nodo.st.sonambulo.brujula = static_cast<Orientacion>((st_result.nodo.st.sonambulo.brujula + 1) % 8);
		
		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (terreno)
		{
		case 'A':
			state.hasBikini ? coste = 7 : 2;
			break;
		case 'B':
			state.hasZapatillas ? coste = 3 : 1;
			break;
		case 'T':
			coste = 1;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = st_result.g + coste;
		break;
	default:
		break;
	}
	return st_result;
};

bool comprobarSolucion(ubicacion jugador, ubicacion final)
{
	return (jugador.f == final.f and jugador.c == final.c);
};

// Función que comprueba para las 4 orientaciones posibles del jugador (norte, este, oeste y sur) si el sonambulo está
// en visión.
bool sonambuloEnVision(const stateN0 &st)
{
	int difFila = st.jugador.f - st.sonambulo.f;
	int difColumna = st.jugador.c - st.sonambulo.c;
	switch (st.jugador.brujula)
	{
	case norte:
		return ((difFila == 1 and abs(difColumna) <= 1) or (difFila == 2 and abs(difColumna) <= 2) or (difFila == 3 and abs(difColumna) <= 3));
		break;
	case este:
		return ((difColumna == -1 and abs(difFila) <= 1) or (difColumna == -2 and abs(difFila) <= 2) or (difColumna == -3 and abs(difFila) <= 3));
		break;
	case oeste:
		return ((difColumna == 1 and abs(difFila) <= 1) or (difColumna == 2 and abs(difFila) <= 2) or (difColumna == 3 and abs(difFila) <= 3));
		break;
	case sur:
		return ((difFila == -1 and abs(difColumna) <= 1) or (difFila == -2 and abs(difColumna) <= 2) or (difFila == -3 and abs(difColumna) <= 3));
		break;
	}
};

list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	list<nodeN0> abiertos;
	set<nodeN0> cerrados;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	abiertos.push_back(current_node);

	while (!abiertos.empty() and !SolutionFound)
	{
		// Quitamos el nodo que está siendo evaluado de abiertos y lo metemos en cerrados
		abiertos.pop_front();
		cerrados.insert(current_node);

		// Estados generados para el jugador
		// Generar hijo actFORWARD
		nodeN0 childForward = current_node;
		childForward.st = apply(actFORWARD, current_node.st, mapa);
		// Si el hijo generado tiene la misma posición que la final, lo pondremos como solución
		if (childForward.st.jugador.c == final.c and childForward.st.jugador.f == final.f)
		{
			childForward.secuencia.push_back(actFORWARD);
			current_node = childForward;
			SolutionFound = true;
		}
		// Si el hijo generado no está en cerrados, entonces se añade a abiertos
		else if (cerrados.find(childForward) == cerrados.end())
		{
			childForward.secuencia.push_back(actFORWARD);
			abiertos.push_back(childForward);
		}

		if (!SolutionFound)
		{
			// Generar hijo actTURN_L
			nodeN0 childTurnL = current_node;
			childTurnL.st = apply(actTURN_L, current_node.st, mapa);
			if (cerrados.find(childTurnL) == cerrados.end())
			{
				childTurnL.secuencia.push_back(actTURN_L);
				abiertos.push_back(childTurnL);
			}

			// Generar hijo actTURN_R
			nodeN0 childTurnR = current_node;
			childTurnR.st = apply(actTURN_R, current_node.st, mapa);
			if (cerrados.find(childTurnR) == cerrados.end())
			{
				childTurnR.secuencia.push_back(actTURN_R);
				abiertos.push_back(childTurnR);
			}
		}

		if (!SolutionFound and !abiertos.empty())
		{
			current_node = abiertos.front();
			while (!abiertos.empty() and cerrados.find(current_node) != cerrados.end())
			{
				abiertos.pop_front();
				if (!abiertos.empty())
					current_node = abiertos.front();
			}
		}
	}

	if (SolutionFound)
		plan = current_node.secuencia;
	return plan;
}

list<Action> AnchuraJugadorSonambulo(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	list<nodeN0> abiertos;
	set<nodeN0> cerrados;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.sonambulo.f == final.f and current_node.st.sonambulo.c == final.c);
	abiertos.push_back(current_node);

	while (!abiertos.empty() and !SolutionFound)
	{
		// Quitamos el nodo que está siendo evaluado de abiertos y lo metemos en cerrados
		abiertos.pop_front();
		cerrados.insert(current_node);

		// Estados generados para el sonámbulo si está en visión.
		if (sonambuloEnVision(current_node.st))
		{
			// Generar hijo actSON_FORWARD
			nodeN0 childSonForward = current_node;
			childSonForward.st = apply(actSON_FORWARD, current_node.st, mapa);
			// Si el hijo generado tiene la misma posición que la final, lo pondremos como solución
			if (childSonForward.st.sonambulo.c == final.c and childSonForward.st.sonambulo.f == final.f)
			{
				childSonForward.secuencia.push_back(actSON_FORWARD);
				current_node = childSonForward;
				SolutionFound = true;
			}
			else if (cerrados.find(childSonForward) == cerrados.end())
			{
				childSonForward.secuencia.push_back(actSON_FORWARD);
				abiertos.push_back(childSonForward);
			}

			if (!SolutionFound)
			{
				// Generar hijo actTURN_L
				nodeN0 childSonTurnL = current_node;
				childSonTurnL.st = apply(actSON_TURN_SL, current_node.st, mapa);
				if (cerrados.find(childSonTurnL) == cerrados.end())
				{
					childSonTurnL.secuencia.push_back(actSON_TURN_SL);
					abiertos.push_back(childSonTurnL);
				}

				// Generar hijo actTURN_R
				nodeN0 childSonTurnR = current_node;
				childSonTurnR.st = apply(actSON_TURN_SR, current_node.st, mapa);
				if (cerrados.find(childSonTurnR) == cerrados.end())
				{
					childSonTurnR.secuencia.push_back(actSON_TURN_SR);
					abiertos.push_back(childSonTurnR);
				}
			}
		}

		if (!SolutionFound)
		{
			// Estados generados para el jugador
			// Generar hijo actFORWARD
			nodeN0 childForward = current_node;
			childForward.st = apply(actFORWARD, current_node.st, mapa);

			// Si el hijo generado no está en cerrados, entonces se añade a abiertos
			if (cerrados.find(childForward) == cerrados.end())
			{
				childForward.secuencia.push_back(actFORWARD);
				abiertos.push_back(childForward);
			}
			// Generar hijo actTURN_L
			nodeN0 childTurnL = current_node;
			childTurnL.st = apply(actTURN_L, current_node.st, mapa);
			if (cerrados.find(childTurnL) == cerrados.end())
			{
				childTurnL.secuencia.push_back(actTURN_L);
				abiertos.push_back(childTurnL);
			}

			// Generar hijo actTURN_R
			nodeN0 childTurnR = current_node;
			childTurnR.st = apply(actTURN_R, current_node.st, mapa);
			if (cerrados.find(childTurnR) == cerrados.end())
			{
				childTurnR.secuencia.push_back(actTURN_R);
				abiertos.push_back(childTurnR);
			}
		}

		if (!SolutionFound and !abiertos.empty())
		{
			current_node = abiertos.front();
			while (!abiertos.empty() and cerrados.find(current_node) != cerrados.end())
			{
				abiertos.pop_front();
				if (!abiertos.empty())
					current_node = abiertos.front();
			}
		}
	}

	if (SolutionFound)
		plan = current_node.secuencia;
	return plan;
}

list<Action> costeUniforme(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, char terreno)
{
	estado current_node;
	priority_queue<estado> abiertos;
	set<estado> cerrados;
	list<Action> plan;
	current_node.nodo.st = inicio;
	bool SolutionFound = (current_node.nodo.st.jugador.f == final.f and current_node.nodo.st.jugador.c == final.c);
	abiertos.push(current_node);

	while (!abiertos.empty() and !SolutionFound)
	{
		// Quitamos el nodo que está siendo evaluado de abiertos y lo metemos en cerrados
		abiertos.pop();
		cerrados.insert(current_node);

		// // Estados generados para el sonámbulo si está en visión.
		// if (sonambuloEnVision(current_node.nodo.st))
		// {
		// 	// Generar hijo actSON_FORWARD
		// 	estado childSonForward = current_node;
		// 	childSonForward = apply(actSON_FORWARD, current_node, mapa, terreno);
		// 	// Si el hijo generado tiene la misma posición que la final, lo pondremos como solución
		// 	if (childSonForward.nodo.st.sonambulo.c == final.c and childSonForward.nodo.st.sonambulo.f == final.f)
		// 	{
		// 		childSonForward.nodo.secuencia.push_back(actSON_FORWARD);
		// 		current_node = childSonForward;
		// 		SolutionFound = true;
		// 	}
		// 	else if (cerrados.find(childSonForward) == cerrados.end())
		// 	{
		// 		childSonForward.nodo.secuencia.push_back(actSON_FORWARD);
		// 		abiertos.push(childSonForward);
		// 	}

		// 	if (!SolutionFound)
		// 	{
		// 		// Generar hijo actTURN_L
		// 		estado childSonTurnL = current_node;
		// 		childSonTurnL = apply(actSON_TURN_SL, current_node, mapa, terreno);
		// 		if (cerrados.find(childSonTurnL) == cerrados.end())
		// 		{
		// 			childSonTurnL.nodo.secuencia.push_back(actSON_TURN_SL);
		// 			abiertos.push(childSonTurnL);
		// 		}

		// 		// Generar hijo actTURN_R
		// 		estado childSonTurnR = current_node;
		// 		childSonTurnR = apply(actSON_TURN_SR, current_node, mapa, terreno);
		// 		if (cerrados.find(childSonTurnR) == cerrados.end())
		// 		{
		// 			childSonTurnR.nodo.secuencia.push_back(actSON_TURN_SR);
		// 			abiertos.push(childSonTurnR);
		// 		}
		// 	}
		// }

		// Estados generados para el jugador
		// Generar hijo actFORWARD
		estado childForward = current_node;
		childForward = apply(actFORWARD, current_node, mapa, terreno);
		// Si el hijo generado tiene la misma posición que la final, lo pondremos como solución
		if (childForward.nodo.st.jugador.c == final.c and childForward.nodo.st.jugador.f == final.f)
		{
			childForward.nodo.secuencia.push_back(actSON_FORWARD);
			current_node = childForward;
			SolutionFound = true;
		}else
		// Si el hijo generado no está en cerrados, entonces se añade a abiertos
		if (cerrados.find(childForward) == cerrados.end())
		{
			childForward.nodo.secuencia.push_back(actFORWARD);
			abiertos.push(childForward);
		}

		if (!SolutionFound)
		{
			// Generar hijo actTURN_L
			estado childTurnL = current_node;
			childTurnL = apply(actTURN_L, current_node, mapa, terreno);
			if (cerrados.find(childTurnL) == cerrados.end())
			{
				childTurnL.nodo.secuencia.push_back(actTURN_L);
				abiertos.push(childTurnL);
			}

			// Generar hijo actTURN_R
			estado childTurnR = current_node;
			childTurnR = apply(actTURN_R, current_node, mapa, terreno);
			if (cerrados.find(childTurnR) == cerrados.end())
			{
				childTurnR.nodo.secuencia.push_back(actTURN_R);
				abiertos.push(childTurnR);
			}
		}

		// if (!SolutionFound and !abiertos.empty())
		// {
		// 	current_node = abiertos.top();
		// 	while (!abiertos.empty() and cerrados.find(current_node) != cerrados.end())
		// 	{
		// 		abiertos.pop();
		// 		if (!abiertos.empty())
		// 			current_node = abiertos.top();
		// 	}
		// }
		current_node = abiertos.top();
	}

	if (SolutionFound)
	{
		cout << "Coste de el camino: " << current_node.g << endl;
		plan = current_node.nodo.secuencia;
	}

	return plan;
}

// Función que pone a 0 todos los elementos de una matriz
void AnularMatriz(vector<vector<unsigned char>> &matriz)
{
	for (int i = 0; i < matriz.size(); i++)
	{
		for (int j = 0; j < matriz[0].size(); j++)
		{
			matriz[i][j] = 0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
	stateN0 cst = st;
	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case actFORWARD:
			cst.jugador = nextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_L:
			cst.jugador.brujula = static_cast<Orientacion>((cst.jugador.brujula + 6) % 8);
			break;
		case actTURN_R:
			cst.jugador.brujula = static_cast<Orientacion>((cst.jugador.brujula + 2) % 8);
			break;
		case actSON_FORWARD:
			cst.sonambulo = nextCasilla(cst.sonambulo);
			mapaConPlan[cst.sonambulo.f][cst.sonambulo.c] = 2;
			break;
		case actSON_TURN_SL:
			cst.sonambulo.brujula = static_cast<Orientacion>((cst.sonambulo.brujula + 7) % 8);
			break;
		case actSON_TURN_SR:
			cst.sonambulo.brujula = static_cast<Orientacion>((cst.sonambulo.brujula + 1) % 8);
			break;
		}
		it++;
	}
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	// Si no hay plan se genera uno
	if (!hayPlan)
	{
		cout << "Generando plan..." << endl;
		// Actualizamos la variable de estado
		c_state.jugador.f = sensores.posF;
		c_state.jugador.c = sensores.posC;
		c_state.jugador.brujula = sensores.sentido;
		c_state.sonambulo.f = sensores.SONposF;
		c_state.sonambulo.c = sensores.SONposC;
		c_state.sonambulo.brujula = sensores.SONsentido;
		goal.f = sensores.destinoF;
		goal.c = sensores.destinoC;

		// Invocar al método de búsqueda dependiendo del nivel en el que estemos
		switch (sensores.nivel)
		{
		case 0:
			plan = AnchuraSoloJugador(c_state, goal, mapaResultado);
			break;
		case 1:
			plan = AnchuraJugadorSonambulo(c_state, goal, mapaResultado);
			break;
		case 2:
			plan = costeUniforme(c_state, goal, mapaResultado, sensores.terreno[0]);
		}
		if (plan.size() > 0)
		{
			VisualizaPlan(c_state, plan);
			hayPlan = true;
		}
	}

	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size() == 0)
	{
		cout << "Se completó el plan" << endl;
		cout << "Se ha tardado " << sensores.tiempo << endl;
		hayPlan = false;
	}

	// Incluir aquí el comportamiento del agente jugador

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
