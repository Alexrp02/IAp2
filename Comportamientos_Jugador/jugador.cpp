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
	default:
		break;
	}
	return st_result;
};

bool comprobarSolucion(ubicacion jugador, ubicacion final)
{
	return (jugador.f == final.f and jugador.c == final.c);
};

// bool AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
// {
//   stateN0 current_state = inicio;
//   list<stateN0> abiertos;
//   list<stateN0> cerrados;
//   abiertos.push_back(current_state);
//   bool solutionFound = (current_state.jugador.f == final.f and current_state.jugador.c == final.c);

//   while (!abiertos.empty() and !solutionFound)
//   {
//     abiertos.pop_front();
//     cerrados.push_back(current_state);

//     // Generar hijo actFORWARD
//     stateN0 childForward = apply(actFORWARD, current_state, mapa);

//     // Si la casilla generada es el objetivo, se ha encontrado la solución
//     if (comprobarSolucion(childForward.jugador, final))
//     {
//       current_state = childForward;
//       solutionFound = true;
//     }
//     // Si no, si este no está ni en abiertos ni en cerrados, lo agregamos a abiertos
//     else if (!Find(childForward, abiertos) and !Find(childForward, cerrados))
//     {
//       abiertos.push_back(childForward);
//     }

//     // Si la solución fue el hijo de actFORWARD, no generamos más hijos, pues tenemos la solución
//     if (!solutionFound)
//     {
//       // Generar hijo actTURN_L
//       stateN0 childTurnL = apply(actTURN_L, current_state, mapa);
//       if (!Find(childTurnL, abiertos) and !Find(childTurnL, cerrados))
//       {
//         abiertos.push_back(childTurnL);
//       }

//       // Generar hijo actTURN_R
//       stateN0 childTurnR = apply(actTURN_R, current_state, mapa);
//       if (!Find(childTurnR, abiertos) and !Find(childTurnR, cerrados))
//       {
//         abiertos.push_back(childTurnR);
//       }
//     }

//     if (!solutionFound and !abiertos.empty())
//       current_state = abiertos.front();
//   }
//   return solutionFound;
// } ;

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

		// Invocar al método de búsqueda
		plan = AnchuraSoloJugador(c_state, goal, mapaResultado);
		VisualizaPlan(c_state, plan);
		hayPlan = true;
	}

	if (hayPlan and plan.size() > 0)
	{
		cout << "Ejecutando la siguiente acción del plan" << endl;
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size() == 0)
	{
		cout << "Se completó el plan" << endl;
		hayPlan = false;
	}

	// Incluir aquí el comportamiento del agente jugador

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
