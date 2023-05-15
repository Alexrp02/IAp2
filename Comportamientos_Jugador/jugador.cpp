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

bool ComportamientoJugador::casillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
{
	if (x.f>=mapa.size() or x.c>=mapa.size())
		return false ;
	if (mapa[x.f][x.c] == '?')
		return true;
	// Devolvemos si la casilla es transitable o no
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
};

// Tres parámetros de entrada: la acción que se quiere realizar, el estado actual y el mapa del juego
// Devuelve el estado que se genera si se puede avanzar, si no, devuelve como salida el mismo estado que el de entrada
// Lo mismo pero para las acciones del sonánmbulo.
stateN0 ComportamientoJugador::apply(const Action &accion, const stateN0 &state, const vector<vector<unsigned char>> &mapa)
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

nodeN1 ComportamientoJugador::apply(const Action &accion, const nodeN1 &node, const vector<vector<unsigned char>> &mapa)
{
	nodeN1 st_result = node;
	ubicacion sig_ubicacion;
	int coste = 0;
	switch (accion)
	{
	case actFORWARD:
		sig_ubicacion = nextCasilla(node.st.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st_result.st.sonambulo.f and sig_ubicacion.c == st_result.st.sonambulo.c))
		{
			st_result.st.jugador = sig_ubicacion;
			// Si es una casilla de bikini o zapatillas se lo ponemos.
			if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'K')
			{
				st_result.st.JhasBikini = true;
				st_result.st.JhasZapatillas = false;
			}
			if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'D')
			{
				st_result.st.JhasBikini = false;
				st_result.st.JhasZapatillas = true;
			}
		}

		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (st_result.st.terrenoJ)
		{
		case 'A':
			node.st.JhasBikini ? coste = 10 : coste = 100;
			break;
		case 'B':
			node.st.JhasZapatillas ? coste = 15 : coste = 50;
			break;
		case 'T':
			coste = 2;
			break;
		case '?':
			coste = costeDesconocida;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = node.g + coste;
		break;
	case actTURN_L:
		st_result.st.jugador.brujula = static_cast<Orientacion>((node.st.jugador.brujula + 6) % 8);

		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (node.st.terrenoJ)
		{
		case 'A':
			node.st.JhasBikini ? coste = 5 : coste = 25;
			break;
		case 'B':
			node.st.JhasZapatillas ? coste = 1 : coste = 5;
			break;
		case 'T':
			coste = 2;
			break;
		case '?':
			coste = costeDesconocida;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = node.g + coste;
		break;
	case actTURN_R:
		st_result.st.jugador.brujula = static_cast<Orientacion>((node.st.jugador.brujula + 2) % 8);

		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (node.st.terrenoJ)
		{
		case 'A':
			node.st.JhasBikini ? coste = 5 : coste = 25;
			break;
		case 'B':
			node.st.JhasZapatillas ? coste = 1 : coste = 5;
			break;
		case 'T':
			coste = 2;
			break;
		case '?':
			coste = costeDesconocida;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = node.g + coste;
		break;
	// Acciones del sonambulo
	case actSON_FORWARD:
		sig_ubicacion = nextCasilla(node.st.sonambulo);
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == node.st.jugador.f and sig_ubicacion.c == node.st.jugador.c))
		{
			st_result.st.sonambulo = sig_ubicacion;
			// Si es una casilla de bikini o zapatillas se lo ponemos.
			if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'K')
			{
				st_result.st.ShasBikini = true;
				st_result.st.ShasZapatillas = false;
			}
			if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'D')
			{
				st_result.st.ShasBikini = false;
				st_result.st.ShasZapatillas = true;
			}
		}

		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (node.st.terrenoS)
		{
		case 'A':
			node.st.ShasBikini ? coste = 10 : coste = 100;
			break;
		case 'B':
			node.st.ShasZapatillas ? coste = 15 : coste = 50;
			break;
		case 'T':
			coste = 2;
			break;
		case '?':
			coste = costeDesconocida;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = node.g + coste;
		break;
	case actSON_TURN_SL:
		st_result.st.sonambulo.brujula = static_cast<Orientacion>((node.st.sonambulo.brujula + 7) % 8);

		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (node.st.terrenoS)
		{
		case 'A':
			node.st.ShasBikini ? coste = 2 : coste = 7;
			break;
		case 'B':
			node.st.ShasZapatillas ? coste = 1 : coste = 3;
			break;
		case 'T':
			coste = 1;
			break;
		case '?':
			coste = costeDesconocida;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = node.g + coste;
		break;
	case actSON_TURN_SR:
		st_result.st.sonambulo.brujula = static_cast<Orientacion>((node.st.sonambulo.brujula + 1) % 8);

		// Dependiendo del tipo de terreno y si tiene o no las zapatillas o no, costará más o menos el desplazamiento
		switch (node.st.terrenoS)
		{
		case 'A':
			node.st.ShasBikini ? coste = 2 : coste = 7;
			break;
		case 'B':
			node.st.ShasZapatillas ? coste = 1 : coste = 3;
			break;
		case 'T':
			coste = 1;
			break;
		case '?':
			coste = costeDesconocida;
			break;
		default:
			coste = 1;
			break;
		}
		st_result.g = node.g + coste;
		break;
	default:
		break;
	}
	st_result.st.terrenoJ = mapa[st_result.st.jugador.f][st_result.st.jugador.c];
	st_result.st.terrenoS = mapa[st_result.st.sonambulo.f][st_result.st.sonambulo.c];
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
	default:
		break;
	}
}

bool sonambuloEnVision(const stateN1 &st)
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
	default:
		break;
	}
}

list<Action> ComportamientoJugador::AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	list<nodeN0> abiertos;
	set<stateN0> cerrados;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	abiertos.push_back(current_node);

	while (!abiertos.empty() and !SolutionFound)
	{
		// Quitamos el nodo que está siendo evaluado de abiertos y lo metemos en cerrados
		abiertos.pop_front();
		cerrados.insert(current_node.st);

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
		else if (cerrados.find(childForward.st) == cerrados.end())
		{
			childForward.secuencia.push_back(actFORWARD);
			abiertos.push_back(childForward);
		}

		if (!SolutionFound)
		{
			// Generar hijo actTURN_L
			nodeN0 childTurnL = current_node;
			childTurnL.st = apply(actTURN_L, current_node.st, mapa);
			if (cerrados.find(childTurnL.st) == cerrados.end())
			{
				childTurnL.secuencia.push_back(actTURN_L);
				abiertos.push_back(childTurnL);
			}

			// Generar hijo actTURN_R
			nodeN0 childTurnR = current_node;
			childTurnR.st = apply(actTURN_R, current_node.st, mapa);
			if (cerrados.find(childTurnR.st) == cerrados.end())
			{
				childTurnR.secuencia.push_back(actTURN_R);
				abiertos.push_back(childTurnR);
			}
		}

		if (!SolutionFound and !abiertos.empty())
		{
			current_node = abiertos.front();
			while (!abiertos.empty() and cerrados.find(current_node.st) != cerrados.end())
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

list<Action> ComportamientoJugador::AnchuraJugadorSonambulo(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	queue<nodeN0> abiertos;
	set<stateN0> cerrados;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.sonambulo.f == final.f and current_node.st.sonambulo.c == final.c);
	abiertos.push(current_node);

	while (!abiertos.empty() and !SolutionFound)
	{
		// Quitamos el nodo que está siendo evaluado de abiertos y lo metemos en cerrados
		// current_node = abiertos.front();
		abiertos.pop();
		cerrados.insert(current_node.st);

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
				break;
			}
			else if (cerrados.find(childSonForward.st) == cerrados.end())
			{
				childSonForward.secuencia.push_back(actSON_FORWARD);
				abiertos.push(childSonForward);
			}

			if (!SolutionFound)
			{
				// Generar hijo actTURN_L
				nodeN0 childSonTurnL = current_node;
				childSonTurnL.st = apply(actSON_TURN_SL, current_node.st, mapa);
				if (cerrados.find(childSonTurnL.st) == cerrados.end())
				{
					childSonTurnL.secuencia.push_back(actSON_TURN_SL);
					abiertos.push(childSonTurnL);
				}

				// Generar hijo actTURN_R
				nodeN0 childSonTurnR = current_node;
				childSonTurnR.st = apply(actSON_TURN_SR, current_node.st, mapa);
				if (cerrados.find(childSonTurnR.st) == cerrados.end())
				{
					childSonTurnR.secuencia.push_back(actSON_TURN_SR);
					abiertos.push(childSonTurnR);
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
			if (cerrados.find(childForward.st) == cerrados.end())
			{
				childForward.secuencia.push_back(actFORWARD);
				abiertos.push(childForward);
			}
			// Generar hijo actTURN_L
			nodeN0 childTurnL = current_node;
			childTurnL.st = apply(actTURN_L, current_node.st, mapa);
			if (cerrados.find(childTurnL.st) == cerrados.end())
			{
				childTurnL.secuencia.push_back(actTURN_L);
				abiertos.push(childTurnL);
			}

			// Generar hijo actTURN_R
			nodeN0 childTurnR = current_node;
			childTurnR.st = apply(actTURN_R, current_node.st, mapa);
			if (cerrados.find(childTurnR.st) == cerrados.end())
			{
				childTurnR.secuencia.push_back(actTURN_R);
				abiertos.push(childTurnR);
			}
		}

		if (!SolutionFound and !abiertos.empty())
		{
			current_node = abiertos.front();
			while (!abiertos.empty() and cerrados.find(current_node.st) != cerrados.end())
			{
				abiertos.pop();
				if (!abiertos.empty())
					current_node = abiertos.front();
			}
		}
	}

	if (SolutionFound)
		plan = current_node.secuencia;
	return plan;
}

list<Action> ComportamientoJugador::costeUniforme(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN1 current_node;
	priority_queue<nodeN1, vector<nodeN1>, compareEstado> abiertos;
	set<stateN1> cerrados;
	list<Action> plan;
	current_node.st = inicio;
	// Si el jugador empieza en una casilla de bikini o zapatillas se lo ponemos.
	if (mapa[inicio.jugador.f][inicio.jugador.c] == 'K')
	{
		current_node.st.JhasBikini = true;
		current_node.st.JhasZapatillas = false;
	}
	if (mapa[inicio.jugador.f][inicio.jugador.c] == 'D')
	{
		current_node.st.ShasBikini = false;
		current_node.st.ShasZapatillas = true;
	}
	current_node.st.terrenoJ = mapa[inicio.jugador.f][inicio.jugador.c];
	current_node.st.terrenoS = mapa[inicio.sonambulo.f][inicio.sonambulo.c];
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	abiertos.push(current_node);

	while (!abiertos.empty() and !SolutionFound)
	{
		// Quitamos el nodo que está siendo evaluado de abiertos y lo metemos en cerrados
		abiertos.pop();
		cerrados.insert(current_node.st);

		// Si el nodo que vamos a generar es la solución, entonces es la solución con menos coste y lo ponemos como solución
		if (current_node.st.jugador.c == final.c and current_node.st.jugador.f == final.f)
		{
			SolutionFound = true;
		}

		// Estados generados para el jugador
		// Generar hijo actFORWARD
		nodeN1 childForward = current_node;
		childForward = apply(actFORWARD, current_node, mapa);

		// Si el hijo generado no está en cerrados, entonces se añade a abiertos
		if (cerrados.find(childForward.st) == cerrados.end())
		{
			childForward.secuencia.push_back(actFORWARD);
			abiertos.push(childForward);
		}

		if (!SolutionFound)
		{
			// Generar hijo actTURN_L
			nodeN1 childTurnL = current_node;
			childTurnL = apply(actTURN_L, current_node, mapa);
			if (cerrados.find(childTurnL.st) == cerrados.end())
			{
				childTurnL.secuencia.push_back(actTURN_L);
				abiertos.push(childTurnL);
			}

			// Generar hijo actTURN_R
			nodeN1 childTurnR = current_node;
			childTurnR = apply(actTURN_R, current_node, mapa);
			if (cerrados.find(childTurnR.st) == cerrados.end())
			{
				childTurnR.secuencia.push_back(actTURN_R);
				abiertos.push(childTurnR);
			}
		}

		if (!SolutionFound and !abiertos.empty())
		{
			current_node = abiertos.top();
			while (!abiertos.empty() and cerrados.find(current_node.st) != cerrados.end())
			{
				abiertos.pop();
				if (!abiertos.empty())
					current_node = abiertos.top();
			}
		}
	}

	if (SolutionFound)
	{
		cout << "Coste de el camino: " << current_node.g << endl;
		plan = current_node.secuencia;
	}

	return plan;
}

// Heurística que devuelve la suma de la distancia de Chebyshev desde el sonambulo a la posición final y la distancia de Manhattan desde el jugador hasta el sonámbulo.
int heuristic(stateN1 st, ubicacion final)
{
	// Distancia de Chebyshev desde el sonámbulo a la posición final
	int distanciaChebyshev = max(abs(st.sonambulo.f - final.f), abs(st.sonambulo.c - final.c));
	// Distancia de Manhattan desde el jugador hasta el área de visión del sonámbulo
	// int distanciaManhattan = abs(st.jugador.f - st.sonambulo.f) + abs(st.jugador.c - st.sonambulo.c) - 6;
	return distanciaChebyshev ;//+ distanciaManhattan;
}

list<Action> ComportamientoJugador::A_star(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN1 current_node;
	priority_queue<nodeN1, vector<nodeN1>, compareEstado> abiertos;
	set<stateN1> cerrados;
	list<Action> plan;
	current_node.st = inicio;
	// Si el jugador empieza en una casilla de bikini o zapatillas se lo ponemos.
	if (mapa[inicio.jugador.f][inicio.jugador.c] == 'K')
	{
		current_node.st.JhasBikini = true;
		current_node.st.JhasZapatillas = false;
	}
	if (mapa[inicio.jugador.f][inicio.jugador.c] == 'D')
	{
		current_node.st.JhasBikini = false;
		current_node.st.JhasZapatillas = true;
	}

	// Si el sonambulo empieza en una casilla de bikini o zapatillas se lo ponemos.
	if (mapa[inicio.sonambulo.f][inicio.sonambulo.c] == 'K')
	{
		current_node.st.ShasBikini = true;
		current_node.st.ShasZapatillas = false;
	}
	if (mapa[inicio.sonambulo.f][inicio.sonambulo.c] == 'D')
	{
		current_node.st.ShasBikini = false;
		current_node.st.ShasZapatillas = true;
	}
	current_node.st.terrenoJ = mapa[inicio.jugador.f][inicio.jugador.c];
	current_node.st.terrenoS = mapa[inicio.sonambulo.f][inicio.sonambulo.c];
	bool SolutionFound = (current_node.st.sonambulo.f == final.f and current_node.st.sonambulo.c == final.c);
	abiertos.push(current_node);

	while (!abiertos.empty() and !SolutionFound)
	{
		// Quitamos el nodo que está siendo evaluado de abiertos y lo metemos en cerrados
		abiertos.pop();
		cerrados.insert(current_node.st);

		// Si el nodo que vamos a generar es la solución, entonces es la solución con menos coste y lo ponemos como solución
		if (current_node.st.sonambulo.c == final.c and current_node.st.sonambulo.f == final.f)
		{
			SolutionFound = true;
			cout << "Nodos abiertos: " << abiertos.size() << endl;
			cout << "Nodos cerrados: " << cerrados.size() << endl;
		}

		// Estados generados para el sonámbulo si está en visión.
		if (sonambuloEnVision(current_node.st) and !SolutionFound)
		{
			// Generar hijo actSON_FORWARD
			nodeN1 childSonForward = current_node;
			childSonForward = apply(actSON_FORWARD, current_node, mapa);
			childSonForward.h = heuristic(childSonForward.st, final);
			if (cerrados.find(childSonForward.st) == cerrados.end())
			{
				childSonForward.secuencia.push_back(actSON_FORWARD);
				abiertos.push(childSonForward);
			}

			// Generar hijo actTURN_L
			nodeN1 childSonTurnL = current_node;
			childSonTurnL = apply(actSON_TURN_SL, current_node, mapa);
			childSonTurnL.h = heuristic(childSonTurnL.st, final);
			if (cerrados.find(childSonTurnL.st) == cerrados.end())
			{
				childSonTurnL.secuencia.push_back(actSON_TURN_SL);
				abiertos.push(childSonTurnL);
			}

			// Generar hijo actTURN_R
			nodeN1 childSonTurnR = current_node;
			childSonTurnR = apply(actSON_TURN_SR, current_node, mapa);
			childSonTurnR.h = heuristic(childSonTurnR.st, final);
			if (cerrados.find(childSonTurnR.st) == cerrados.end())
			{
				childSonTurnR.secuencia.push_back(actSON_TURN_SR);
				abiertos.push(childSonTurnR);
			}
		}
		// Si el sonámbulo no está en visión y no se ha encontrado una solución, entonces generamos los estados que mueven al jugador
		if (!SolutionFound)
		{
			// Estados generados para el jugador
			// Generar hijo actFORWARD
			nodeN1 childForward = current_node;
			childForward = apply(actFORWARD, current_node, mapa);
			childForward.h = heuristic(childForward.st, final);

			// Si el hijo generado no está en cerrados, entonces se añade a abiertos
			if (cerrados.find(childForward.st) == cerrados.end())
			{
				childForward.secuencia.push_back(actFORWARD);
				abiertos.push(childForward);
			}

			// Generar hijo actTURN_L
			nodeN1 childTurnL = current_node;
			childTurnL = apply(actTURN_L, current_node, mapa);
			childTurnL.h = heuristic(childTurnL.st, final);
			if (cerrados.find(childTurnL.st) == cerrados.end())
			{
				childTurnL.secuencia.push_back(actTURN_L);
				abiertos.push(childTurnL);
			}

			// Generar hijo actTURN_R
			nodeN1 childTurnR = current_node;
			childTurnR = apply(actTURN_R, current_node, mapa);
			childTurnR.h = heuristic(childTurnR.st, final);
			if (cerrados.find(childTurnR.st) == cerrados.end())
			{
				childTurnR.secuencia.push_back(actTURN_R);
				abiertos.push(childTurnR);
			}
		}
		if (!SolutionFound and !abiertos.empty())
		{
			current_node = abiertos.top();
			while (!abiertos.empty() and cerrados.find(current_node.st) != cerrados.end())
			{
				abiertos.pop();
				if (!abiertos.empty())
					current_node = abiertos.top();
			}
		}
	}

	if (SolutionFound)
	{
		cout << "Coste de el camino: " << current_node.g << endl;
		plan = current_node.secuencia;
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

void ComportamientoJugador::VisualizaPlan(const stateN1 &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	stateN1 cst = st;
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

// Método para dibujar cuando la orientación no es diagonal
void dibujarRecto(const vector<unsigned char> &terreno, const ubicacion &st, vector<vector<unsigned char>> &matriz, int factor, bool sumaColumna)
{
	int cont = 1;

	int filaInicial;
	int columnaInicial;
	for (int i = 1; i <= 3; i++)
	{
		if (sumaColumna)
		{
			columnaInicial = filaInicial = i * factor;
		}
		else
		{
			filaInicial = i * factor;
			columnaInicial = i * (-factor);
		}
		for (int j = 0; j < 1 + i * 2; j++)
		{

			if (sumaColumna)
			{
				if (st.f + filaInicial >= 0 and st.f + filaInicial < matriz.size() and st.c + columnaInicial >= 0 and st.c + columnaInicial < matriz[0].size())
				{
					matriz[st.f + filaInicial][st.c + columnaInicial] = terreno[cont];
					cont++;
					columnaInicial -= factor;
				}
			}
			else
			{
				if (st.f + filaInicial >= 0 and st.f + filaInicial < matriz.size() and st.c + columnaInicial >= 0 and st.c + columnaInicial < matriz[0].size())
				{
					matriz[st.f + filaInicial][st.c + columnaInicial] = terreno[cont];
					cont++;
					filaInicial -= factor;
				}
			}
		}
	}
}

// Método para actualizar el mapa con la información de los sensores
void dibujarEnMatriz(const vector<unsigned char> &terreno, const stateN1 &st, vector<vector<unsigned char>> &matriz)
{
	switch (st.jugador.brujula)
	{
	case norte:
		dibujarRecto(terreno, st.jugador, matriz, -1, true);
		break;
	case sur:
		dibujarRecto(terreno, st.jugador, matriz, 1, true);
		break;
	case este:
		dibujarRecto(terreno, st.jugador, matriz, -1, false);
		break;
	case oeste:
		dibujarRecto(terreno, st.jugador, matriz, 1, false);
		break;
	}
}

// Función para contar casillas que no sean de un tipo en el vector
int contarCasillas(const vector<vector<unsigned char>> &matriz, unsigned char casilla)
{
	int cont = 0;
	for (int i = 0; i < matriz.size(); i++)
	{
		for (int j = 0; j < matriz[0].size(); j++)
		{
			if (matriz[i][j] != casilla)
				cont++;
		}
	}
	return cont;
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	// Actualizamos la variable de estado
	c_state.jugador.f = sensores.posF;
	c_state.jugador.c = sensores.posC;
	c_state.jugador.brujula = sensores.sentido;
	c_state.sonambulo.f = sensores.SONposF;
	c_state.sonambulo.c = sensores.SONposC;
	c_state.sonambulo.brujula = sensores.SONsentido;
	goal.f = sensores.destinoF;
	goal.c = sensores.destinoC;
	stateN1 c_stateN1;
	c_stateN1 = c_state;
	// Si no hay plan se genera uno
	if (!hayPlan and sensores.nivel != 4)
	{
		cout << "Generando plan..." << endl;
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
			plan = costeUniforme(c_stateN1, goal, mapaResultado);
			break;
		case 3:
			plan = A_star(c_stateN1, goal, mapaResultado);
			break;
		}
		if (plan.size() > 0)
		{
			VisualizaPlan(c_state, plan);
			hayPlan = true;
		}
	}
	else if (sensores.nivel == 4)
	{
		if(last_action==actWHEREIS)
			c_state1 = c_stateN1 ;
		// Actualizamos la variable de estado
		int a;
		// Si sabemos dónde estamos con certeza, actualizamos la variable de estado y el mapa
		if (!whereIs and !sensores.colision and !sensores.reset)
		{
			switch (last_action)
			{
			case actFORWARD:
				// Actualización en caso de avanzar
				switch (c_state1.jugador.brujula)
				{
				case norte:
					c_state1.jugador.f--;
					break;
				case noreste:
					c_state1.jugador.f--;
					c_state1.jugador.c++;
					break;
				case este:
					c_state1.jugador.c++;
					break;
				case sureste: /*Actualizacion*/
					c_state1.jugador.f++;
					c_state1.jugador.c++;
					break;
				case sur: /*Actualizacion*/
					c_state1.jugador.f++;
					break;
				case suroeste: /*Actualizacion*/
					c_state1.jugador.f++;
					c_state1.jugador.c--;
					break;
				case oeste: /*Actualizacion*/
					c_state1.jugador.c--;
					break;
				case noroeste: /*Actualizacion*/
					c_state1.jugador.f--;
					c_state1.jugador.c--;
					break;
				}
				break;
			case actTURN_L:
				a = c_state1.jugador.brujula;
				a = (a + 6) % 8;
				c_state1.jugador.brujula = static_cast<Orientacion>(a);
				break;
			case actTURN_R:
				a = c_state1.jugador.brujula;
				a = (a + 2) % 8;
				c_state1.jugador.brujula = static_cast<Orientacion>(a);
				break;
			case actSON_FORWARD:
				switch (c_state1.sonambulo.brujula)
				{
				case norte:
					c_state1.sonambulo.f--;
					break;
				case noreste:
					c_state1.sonambulo.f--;
					c_state1.sonambulo.c++;
					break;
				case este:
					c_state1.sonambulo.c++;
					break;
				case sureste: /*Actualizacion*/
					c_state1.sonambulo.f++;
					c_state1.sonambulo.c++;
					break;
				case sur: /*Actualizacion*/
					c_state1.sonambulo.f++;
					break;
				case suroeste: /*Actualizacion*/
					c_state1.sonambulo.f++;
					c_state1.sonambulo.c--;
					break;
				case oeste: /*Actualizacion*/
					c_state1.sonambulo.c--;
					break;
				case noroeste: /*Actualizacion*/
					c_state1.sonambulo.f--;
					c_state1.sonambulo.c--;
					break;
				}
				break;
			case actSON_TURN_SL:
				a = c_state1.sonambulo.brujula;
				a = (a + 7) % 8;
				c_state1.sonambulo.brujula = static_cast<Orientacion>(a);
				break;
			case actSON_TURN_SR:
				a = c_state1.sonambulo.brujula;
				a = (a + 1) % 8;
				c_state1.sonambulo.brujula = static_cast<Orientacion>(a);
				break;
			}

			// Tenemos que actualizar el mapa
			dibujarEnMatriz(sensores.terreno, c_state1, mapaResultado);
		}

		// Si no tenemos plan o hemos hecho reset, lo generamos
		if (!hayPlan and !whereIs){
			costeDesconocida = (contarCasillas(mapaResultado, '?') * factorDeAumento / (mapaResultado.size() * mapaResultado[0].size()));
			plan = A_star(c_state1, goal, mapaResultado);
			hayPlan = true ;
		}
			
		// Si hacemos reset tenemos que volver a saber la posición de los jugadores y además tenemos que recalcular el plan
		else if (sensores.reset){
			accion = actWHEREIS;
			hayPlan = false;
			whereIs = true;
		}

		// Si hay una colisión con una pared tenemos que recalcular el plan
		else if (sensores.colision and sensores.terreno[2] == 'M'){
			costeDesconocida = (contarCasillas(mapaResultado, '?') * factorDeAumento / (mapaResultado.size() * mapaResultado[0].size()));
			plan = A_star(c_state1, goal, mapaResultado);
			hayPlan = true;
		}
			
		// Si tenemos una colisión y no tenemos un muro delante, es que ha sido por el empuje de un lobo,
		// recalculamos plan pero antes tenemos que saber donde estamos
		else if (sensores.colision)
		{
			accion = actWHEREIS;
			hayPlan = false;
			whereIs = true;
		}
		// Si tenemos un precipicio delante, tenemos que recalcular el plan
		else if (sensores.terreno[2] == 'P'){
			costeDesconocida = (contarCasillas(mapaResultado, '?') * factorDeAumento / (mapaResultado.size() * mapaResultado[0].size()));
			plan = A_star(c_state1, goal, mapaResultado);
			hayPlan = true;
		}

		// Si el nivel en el que estamos es el 4, primero tenemos que usar actWHEREIS
		else if (sensores.nivel == 4 and whereIs)
		{
			accion = actWHEREIS;
			VisualizaPlan(c_state1, plan);
			whereIs = false;
			hayPlan = false;
		}
		if (plan.size() > 0 and !whereIs)
		{
			VisualizaPlan(c_state1, plan);
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

	last_action = accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
