#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

// Estructura para representar estados para el nivel 0
struct stateN0
{
  ubicacion jugador;
  ubicacion sonambulo;

  bool operator==(const stateN0 &x) const
  {
    return (jugador == x.jugador && sonambulo == x.sonambulo);
  }
};

class ComportamientoJugador : public Comportamiento
{
public:
  // Constructor para cuando no se conoce el mapa
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    // Inicializar Variables de Estado
    hayPlan = false;
  }
  // Constructor para cuando se conoce el mapa
  ComportamientoJugador(std::vector<std::vector<unsigned char>> mapaR) : Comportamiento(mapaR)
  {
    // Inicializar Variables de Estado
    hayPlan = false;
  }
  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);

private:
  // Declarar Variables de Estado
  bool hayPlan;
  list<Action> plan;

  // Función para avanzar a saltos de caballo
  // Sirve de ejemplo para cargar un plan
  list<Action> AvanzaASaltosDeCaballo()
  {
    list<Action> secuencia;
    secuencia.push_back(actFORWARD);
    secuencia.push_back(actFORWARD);
    secuencia.push_back(actTURN_R);
    secuencia.push_back(actFORWARD);
    return secuencia;
  }
};

bool AnchuraSoloJugador(const stateN0 &inicio, const stateN0 &final, const vector<vector<unsigned char>> &mapa)
{
  stateN0 current_state = inicio;
  list<stateN0> abiertos;
  list<stateN0> cerrados;
  abiertos.push_back(current_state);
  bool solutionFound = (current_state.jugador.f == final.jugador.f and current_state.jugador.c == final.jugador.c);

  while (!abiertos.empty() and !solutionFound)
  {
    abiertos.pop_front();
    cerrados.push_back(current_state);

    // Generar hijo actFORWARD
    stateN0 childForward = apply(actFORWARD, current_state, mapa);

    // Si la casilla generada es el objetivo, se ha encontrado la solución
    if (comprobarSolucion(childForward.jugador, final.jugador))
    {
      current_state = childForward;
      solutionFound = true;
    }
    // Si no, si este no está ni en abiertos ni en cerrados, lo agregamos a abiertos
    else if (!Find(childForward, abiertos) and !Find(childForward, cerrados))
    {
      abiertos.push_back(childForward);
    }

    // Si la solución fue el hijo de actFORWARD, no generamos más hijos, pues tenemos la solución
    if (!solutionFound)
    {
      // Generar hijo actTURN_L
      stateN0 childTurnL = apply(actTURN_L, current_state, mapa);
      if (!Find(childTurnL, abiertos) and !Find(childTurnL, cerrados))
      {
        abiertos.push_back(childTurnL);
      }

      // Generar hijo actTURN_R
      stateN0 childTurnR = apply(actTURN_R, current_state, mapa);
      if (!Find(childTurnR, abiertos) and !Find(childTurnR, cerrados))
      {
        abiertos.push_back(childTurnR);
      }
    }

    if (!solutionFound and !abiertos.empty())
      current_state = abiertos.front();
  }
}

bool comprobarSolucion(ubicacion jugador, ubicacion final)
{
  return (jugador.f == final.f and jugador.c == final.c);
}

#endif
