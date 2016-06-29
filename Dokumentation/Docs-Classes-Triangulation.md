*Studienarbeit SS 2016 - HAW Hamburg - optische Trackinganlage*
# Dokumentation - Triangulation

## Einleitung
Um die Position des Objektes (Quadrocopter) im Raum bestimmen zu können, müssen mindestens zwei Geraden bestimmt werden, auf denen sich das Objekt befindet.  
Dies wird alles in der **Klasse "CameraParams"** ausgeführt.

Der zweite Teil der Positionsbestimmung befasst sich mit der Bestimmung des "Schnittpunktes" der zwei Geraden.  
Diese Berechnungen finden in der  Utility-Klasse **"Triangulation"** statt.


## Bestimmung der Geraden durch Kamera und Objekt
Zuerst werden die Kameraposition und -Ausrichtung benötigt.

Die originale Dokumentation ist auch in `TA5_Dokumentation Koordinatentranformation.pdf` um im Matlab-Script im Ordner `TA5_Matlab` zu finden.  
Für beide Kameras wurde die Position des Kamera bestimmt. Diese ist als Vektor `o` bezeichnet. Um die Ausrichtung der Kameras bestimmen zu können wurde zusätzlich der Punkt an der Raumwand markiert, der in der Bildmitte der jeweiligen Kamera liegt. Dieser wird mit Vektor `p` bezeichnet.  
Somit ergibt sich der Richtungsvektor der Kamera `r` aus der vektoriellen Differenz `r = p - o`.
Um den letzen Freiheitsgrad der Kamera zu bestimmen, wird ein weiterer Punkt an der Wand benötigt. Dieser wurde auf der horizontalen Bildebene gewählt. Aus dem Punkt `t` ergibt sich `s = t - o`.

Die Position der Kamera (Translation des Koordinatensystems) lässt sich direkt auf dem Ortsvektor `o` ablesen.  
Die Euler-Rotationen der Kamera lassen sich aus den Vektoren `r` und `s` bestimmen. Es wurde absichtlich keine direkte Korrelation zwischen dem Kamerakoordinatensystem um dem Weltkoordinatensystem gewählt, damit der verwendeten Methode die Euler-Rotationen besser verständlich sind. Zum Schluss werden dann einfach die Achsen zwischen dem Kamera- und Weltkoordinatensystem korrekt zugeordnet.

Nun ist die Koordinatentransformation eines Vektors im Kamerakoordinatensystem in das Weltkoordinatensystem bekannt. Die Translation des Vektors kann hier ausgelassen werden, da für die Geradengleichung der Orts- und Richtungsvektor getrennt beachtet werden.  
Das Weltkoordinatensystem ist `X, Y, Z`, das Kamerakoordinatensystem `u, v, w`.

Der Vektor, auf dem sich das Objekt befindet, wird im Kamerakoordinatensystem wie folgt berechnet. Dafür sind die Werte `c_x`, `c_y` und `f` aus der Kameramatrix von openCV nötig. `c_x` und `c_y` beschreiben diejenige Pixelkoordinate am Mittelpunkt des Sensors. `f` bzw. `f_x` ist die Brennweite der Linse in Pixeln. Die Angabe in Pixel ist sinnvoll, da das Kamerakoordinatensystem dann in allen Achsen die Einheit Pixel hat und damit die physikalische Größe der Pixel nicht bekannt sein muss.  
Es ergibt sich also ein Vektor, auf dem sich das Objekt befindet:

```
         ( u - c_x )
g(u,v) = | v - c_x |
         ( f       )
          
u: Sensorkoordinate Nr. 1
v: Sensorkoordinate Nr. 2
```

Die Zuordnung von Kamera- und Weltkoordinaten ist definiert als:

```
u --> Y
v --> Z
w --> X
```

Für die Euler-Transformation wurde das Schema `Rotation um Z, dann Y', dann X''` verwendet. Die Berechnung der entsprechenden Winkel ist im Matlab-Script dokumentiert.


## Triangulation der Position
Da im 3D-Raum zwei sich schneidende Geraden sehr unwahrscheinlich sind, muss ein Verfahren angewendet werden, bei dem der geringste Abstand der Geraden als "Schnittpunkt" gewertet wird.

In diesem Fall wurde das sog. Lotfußpunktverfahren in der Methode **"Triangulation::triangulate()"** implementiert.