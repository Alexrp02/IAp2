#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <queue>
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
    // Comparar el estado del jugador
    if (st.jugador.f != n.st.jugador.f)
      return st.jugador.f < n.st.jugador.f;
    if (st.jugador.c != n.st.jugador.c)
      return st.jugador.c < n.st.jugador.c;
    if (st.jugador.brujula != n.st.jugador.brujula)
      return st.jugador.brujula < n.st.jugador.brujula;

    // Comparar el estado del sonámbulo
    if (st.sonambulo.f != n.st.sonambulo.f)
      return st.sonambulo.f < n.st.sonambulo.f;
    if (st.sonambulo.c != n.st.sonambulo.c)
      return st.sonambulo.c < n.st.sonambulo.c;
    if (st.sonambulo.brujula != n.st.sonambulo.brujula)
      return st.sonambulo.brujula < n.st.sonambulo.brujula;

    // Si todos los campos son iguales, no hay diferencia de orden
    return false;
  }
};

struct estado
{
  nodeN0 nodo;
  int g = 0;
  int h = 0;
  bool hasBikini = false;
  bool hasZapatillas = false;
  char terrenoJ ;
  char terrenoS ;

  bool operator==(const estado &n) const
  {
    return (nodo == n.nodo and hasBikini==n.hasBikini and hasZapatillas==n.hasZapatillas);
  }

  bool operator<(const estado &n) const
  {
    if(!(nodo==n.nodo))
      return nodo<n.nodo;
    else if (hasBikini!=n.hasBikini)
      return hasBikini<n.hasBikini;
    else if (hasZapatillas!=n.hasZapatillas)
      return hasZapatillas<n.hasZapatillas;
    else return false;
  }
};

struct compareEstado{
  bool operator()(const estado &n1, const estado &n2) const
  {
    return (n1.g + n1.h) > (n2.g + n2.h);
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
