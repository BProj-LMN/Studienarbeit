/*
 * triangulate.h
 *
 * function: Lotfusspunktverfahren zur Bestimmung des Schnittpunktes
 * zweier windschiefer Geraden innerhalb eines globalen Koordinatensystems
 *
 * author: Martin Kroh
 */

#ifndef TRIANGULATE_H_
#define TRIANGULATE_H_

#include "DataFormats.h"
//#include "Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>

#define MYEPS      1.0E-19
#define ANZ_KOORDS 3       // Anzahl Dimensionen des Koordinatensystems

int triangulate(VectRay& ray1, VectRay& ray2, Pos3D& pos) {
  /*** Eingabeparameter fuer Algomithmus umwandeln ***/
  float g1_posVec[3] = { float(ray1.pos.x), float(ray1.pos.y), float(ray1.pos.z) }; // Gerade 1 Ortsvektor
  float g1_dirVec[3] = { float(ray1.dir.x), float(ray1.dir.y), float(ray1.dir.z) }; // Gerade 1 Richtungsvektor
  float g2_posVec[3] = { float(ray2.pos.x), float(ray2.pos.y), float(ray2.pos.z) }; // Gerade 2 Ortsvektor
  float g2_dirVec[3] = { float(ray2.dir.x), float(ray2.dir.y), float(ray2.dir.z) }; // Gerade 2 Richtungsvektor

  /*** interne Variablen anlegen ***/
  int i = 0;
  float a[ANZ_KOORDS][ANZ_KOORDS];
  float b[ANZ_KOORDS][ANZ_KOORDS];
  float x[ANZ_KOORDS - 1][ANZ_KOORDS - 1];
  float y[ANZ_KOORDS - 1];
  float Skalar[ANZ_KOORDS - 1];
  float Verbindungsvektor[ANZ_KOORDS];
  float Verbindungsvektor_halbiert[ANZ_KOORDS];
  float Schnittpunkt[ANZ_KOORDS];

  /*** Interne Variablen zur Lösung des LGS ***/
  int S0 = 0;
  int S2 = 0;
  int S3 = 0;
  float q;
  float Pivotelement = 0.0;
  int S4 = 0;
  int S5 = 0;
  float Summe;

  /*** Ermittlung des LGS aus Orthogonalitätsbedingungen ***/
  for (i = 0; i < ANZ_KOORDS; i++) {
    a[i][0] = (g2_posVec[i] - g1_posVec[i]) * g1_dirVec[i];
    a[i][1] = g2_dirVec[i] * g1_dirVec[i];
    a[i][2] = (-1) * g1_dirVec[i] * g1_dirVec[i];
    b[i][0] = (g2_posVec[i] - g1_posVec[i]) * g2_dirVec[i];
    b[i][1] = g2_dirVec[i] * g2_dirVec[i];
    b[i][2] = (-1) * g1_dirVec[i] * g2_dirVec[i];
  }

  /*** 2x2-Matrix und 2x1-Vektor für LGS ***/
  x[0][0] = a[0][1] + a[1][1] + a[2][1];
  x[0][1] = a[0][2] + a[1][2] + a[2][2];
  x[1][0] = b[0][1] + b[1][1] + b[2][1];
  x[1][1] = b[0][2] + b[1][2] + b[2][2];
  y[0] = -(a[0][0] + a[1][0] + a[2][0]);
  y[1] = -(b[0][0] + b[1][0] + b[2][0]);

  /*** Vorwärts eliminieren ***/
  for (S0 = 0; S0 < (ANZ_KOORDS - 2); S0++) {
    Pivotelement = x[S0][S0];
    for (S2 = 1; S2 < ((ANZ_KOORDS - 1) - S0); S2++) {
      q = ((x[(S2 + S0)][S0] / Pivotelement) * (-1));
      for (S3 = 0; S3 < ((ANZ_KOORDS - 1) - S0); S3++) {
        x[(S0 + S2)][(S0 + S3)] = ((x[S0][(S0 + S3)] * q) + x[S0 + S2][(S0 + S3)]);
      };
      y[S0 + S2] = ((y[S0] * q) + y[S0 + S2]);
    }
  }

  /*** Rückwärts einsetzten ***/
  for (S4 = (ANZ_KOORDS - 2); S4 >= 0; S4--) {
    Summe = 0;
    for (S5 = (S4 + 1); S5 <= (ANZ_KOORDS - 2); S5++) {
      Summe = Summe + (x[S4][S5]) * Skalar[S5];
    }
    Skalar[S4] = ((y[S4]) - Summe) / x[S4][S4];
  }

  /*** Berechnung des Verbindungsvektors ***/
  for (i = 0; i < ANZ_KOORDS; i++) {
    Verbindungsvektor[i] = (g2_posVec[i] - g1_posVec[i]) + g2_dirVec[i] * Skalar[0] - g1_dirVec[i] * Skalar[1];
  }

  /*** Halbierung des Verbindungsvektors ***/
  for (i = 0; i < ANZ_KOORDS; i++) {
    Verbindungsvektor_halbiert[i] = 0.5 * Verbindungsvektor[i];
  }

  /*** Berechnung des Schnittpunktes im globalen Koordinatensystem ***/
  for (i = 0; i < ANZ_KOORDS; i++) {
    Schnittpunkt[i] = g1_posVec[i] + Skalar[1] * g1_dirVec[i] + Verbindungsvektor_halbiert[i];
  }

  /*** Rückgabewert (Referenz als Paramteter erhalten) zusammenstellen ***/
  pos.x = Schnittpunkt[0];
  pos.y = Schnittpunkt[1];
  pos.z = Schnittpunkt[2];

  /*** BERECHNUNG DES KLEINSTEN ABSTANDS ***/
  return sqrt(pow(Verbindungsvektor[0], 2) + pow(Verbindungsvektor[1], 2) + pow(Verbindungsvektor[2], 2));
}

#endif /* TRIANGULATE_H_ */
