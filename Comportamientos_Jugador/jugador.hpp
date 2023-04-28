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

struct nodeN0
{
  stateN0 st;
  list<Action> secuencia;

  bool operator==(const nodeN0 &n) const
  {
    return (st == n.st);
  }

  bool operator<(const nodeN0 &n) const
  {
    if (st.jugador.f < n.st.jugador.f)
      return true;
    else if (st.jugador.f == n.st.jugador.f and st.jugador.c < n.st.jugador.c)
      return true;
    else if (st.jugador.f == n.st.jugador.f and st.jugador.c == n.st.jugador.c and st.jugador.brujula < n.st.jugador.brujula)
      return true;
    else
    {
      if (st.sonambulo.f < n.st.sonambulo.f)
        return true;
      else if (st.sonambulo.f == n.st.sonambulo.f and st.sonambulo.c < n.st.sonambulo.c)
        return true;
      else if (st.sonambulo.f == n.st.sonambulo.f and st.sonambulo.c == n.st.sonambulo.c and st.sonambulo.brujula < n.st.sonambulo.brujula)
        return true;
      else
        return false;
    }
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

  void VisualizaPlan(const stateN0 &st, const list<Action> &plan);

  Action think(Sensores sensores);
  int interact(Action accion, int valor);

private:
  // Declarar Variables de Estado
  bool hayPlan;
  list<Action> plan;
  stateN0 c_state;
  ubicacion goal;
};

#endif
