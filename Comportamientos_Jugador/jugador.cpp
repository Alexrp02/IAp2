#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	// Si no hay plan se genera uno
	if (!hayPlan)
	{
		// Invocar al método de búsqueda
		plan = AvanzaASaltosDeCaballo();
		hayPlan = true;
	}
	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size() == 0)
	{
		hayPlan = false;
	}

	// Incluir aquí el comportamiento del agente jugador

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

