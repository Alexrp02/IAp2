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

  bool operator<(const stateN0 &n) const
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

    // Si todos los campos son iguales, no hay diferencia de orden
    return false;
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
    return (st < n.st);
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
    // Inicializamos el mapaResultado con los precipicios de los bordes
    for (unsigned int i = 0; i < size; i++)
    {
      // Filas
      mapaResultado[0][i] = 'P';
      mapaResultado[1][i] = 'P';
      mapaResultado[2][i] = 'P';
      mapaResultado[size - 1][i] = 'P';
      mapaResultado[size - 2][i] = 'P';
      mapaResultado[size - 3][i] = 'P';

      // Columnas
      mapaResultado[i][0] = 'P';
      mapaResultado[i][1] = 'P';
      mapaResultado[i][2] = 'P';
      mapaResultado[i][size - 1] = 'P';
      mapaResultado[i][size - 2] = 'P';
      mapaResultado[i][size - 3] = 'P';
    }

    // Inicializar Variables de Estado
    hayPlan = false;
  }
  // Constructor para cuando se conoce el mapa
  ComportamientoJugador(std::vector<std::vector<unsigned char>> mapaR) : Comportamiento(mapaR)
  {
    // Inicializamos el mapaResultado con los precipicios de los bordes
    for (unsigned int i = 0; i < mapaR.size(); i++)
    {
      // Filas
      mapaResultado[0][i] = 'P';
      mapaResultado[1][i] = 'P';
      mapaResultado[2][i] = 'P';
      mapaResultado[mapaR.size() - 1][i] = 'P';
      mapaResultado[mapaR.size() - 2][i] = 'P';
      mapaResultado[mapaR.size() - 3][i] = 'P';

      // Columnas
      mapaResultado[i][0] = 'P';
      mapaResultado[i][1] = 'P';
      mapaResultado[i][2] = 'P';
      mapaResultado[i][mapaR.size() - 1] = 'P';
      mapaResultado[i][mapaR.size() - 2] = 'P';
      mapaResultado[i][mapaR.size() - 3] = 'P';
    }
    // Inicializar Variables de Estado
    hayPlan = false;
  }
  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
  void VisualizaPlan(const stateN1 &st, const list<Action> &plan);

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  

private:
  // Declarar Variables de Estado
  bool hayPlan;
  bool whereIs = true; // Variable para saber si hay que ejecutar accion actWHEREIS
  list<Action> plan;
  stateN0 c_state;
  stateN1 c_state1;
  Action last_action ;
  ubicacion goal;
  ubicacion bateria;
  bool bateriaVista = false; 
  bool needRecarga = false ;
  int costeDesconocida = 1 ;
  int factorDeAumento = 3;
  void dibujarRecto(const vector<unsigned char> &terreno, const ubicacion &st, vector<vector<unsigned char>> &matriz, int factor, bool sumaColumna);
  void dibujarEnMatriz(const vector<unsigned char> &terreno, const stateN1 &st, vector<vector<unsigned char>> &matriz);
  bool casillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa);
  stateN0 apply(const Action &accion, const stateN0 &state, const vector<vector<unsigned char>> &mapa);
  nodeN1 apply(const Action &accion, const nodeN1 &node, const vector<vector<unsigned char>> &mapa);
  list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
  list<Action> AnchuraJugadorSonambulo(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
  list<Action> costeUniforme(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) ;
  list<Action> A_star(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) ;
  list<Action> A_star_jugador(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) ;
};

#endif
