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

struct stateN1
{
  ubicacion jugador;
  ubicacion sonambulo;
  bool JhasBikini = false;
  bool JhasZapatillas = false;
  bool ShasBikini = false;
  bool ShasZapatillas = false;
  char terrenoJ ;
  char terrenoS ;

  bool operator==(const stateN1 &x) const
  {
    return (jugador == x.jugador && sonambulo == x.sonambulo  and 
    JhasBikini==x.JhasBikini and JhasZapatillas==x.JhasZapatillas and ShasBikini==x.ShasBikini and ShasZapatillas==x.ShasZapatillas);
  }

  stateN1& operator=(const stateN0 &x) {
    jugador = x.jugador;
    sonambulo = x.sonambulo;
    return *this;
  }

  bool operator<(const stateN1 &n) const
  {
    // Comparar el estado del jugador
    if (jugador.f != n.jugador.f)
      return jugador.f < n.jugador.f;
    if (jugador.c != n.jugador.c)
      return jugador.c < n.jugador.c;
    if (jugador.brujula != n.jugador.brujula)
      return jugador.brujula < n.jugador.brujula;

    // Comparar el estado del sonámbulo
    if (sonambulo.f != n.sonambulo.f)
      return sonambulo.f < n.sonambulo.f;
    if (sonambulo.c != n.sonambulo.c)
      return sonambulo.c < n.sonambulo.c;
    if (sonambulo.brujula != n.sonambulo.brujula)
      return sonambulo.brujula < n.sonambulo.brujula;
    // Comparamos objetos del jugador
    else if (JhasBikini!=n.JhasBikini)
      return JhasBikini<n.JhasBikini;
    else if (JhasZapatillas!=n.JhasZapatillas)
      return JhasZapatillas<n.JhasZapatillas;
    // El estado de el jugador es el mismo, comparamos el del sonámbulo
    else if (ShasBikini!=n.ShasBikini)
      return ShasBikini<n.ShasBikini;
    else if (ShasZapatillas!=n.ShasZapatillas)
      return ShasZapatillas<n.ShasZapatillas;
    else return false;
  }
};

struct nodeN1
{
  stateN1 st;
  list<Action> secuencia;
  int g = 0;
  int h = 0;

  bool operator==(const nodeN1 &n) const
  {
    return (st == n.st);
  }
};

struct compareEstado{
  bool operator()(const nodeN1 &n1, const nodeN1 &n2) const
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
