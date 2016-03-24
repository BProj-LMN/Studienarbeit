*Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung*
# Dokumentation - TA5 - Tracking


## Schnittstelle und Abhängigkeiten
- Verbindung zum Reglersystem per UDP-Socket
- opencv v3.0.0


## Software-Struktur
Die Software wurde recht stark modular entwickelt. Grundsätzlich gibt es folgende Teile:

- main-Routine
- Socket-Klasse
- ObjectDetection-Klasse
- Camera-Klasse
- triangulate.h - mit Funktion `triangulate`
- zur Kalibrierung des Systems:
	- Einstellung einer Maske für alle Bilder
		- calibrateFrameMask.h - mit Funtion `calibrateFrameMask`
	- Einstellung der intrinsischen Kameraparameter
		- calibrateCamera.h - mit Funktion `executeDistCalib`
		- calibrateCamera_Settings
		- calibrateCamera_Helpers.h
	- Einstellung der extrinsischen Kameraparameter
		- calibrate3D.h - mit Funktion `calibrate3D`
		- calibrate3D_Helpers.h

Für eine genaue Beschreibung der Klassen, siehe `TA5-classDiagram.mdj`.

##### Kommentar zur Kalibrierung der extrinsischen Parameter
Aufgrund einiger Probleme mit der Triangulation in openCV wird die Kameraposition und -Ausrichtung mittlerweile nicht mehr mit openCV und den Funktionen in `calibrate3D.h` durchgeführt.

Die Kameraposition wird zur Zeit manuell ermittelt und auch die Triangulation wurde selbst implementiert. Mehr zu den Berechnungen im entsprechenden Kapitel.

### Klasse Socket
siehe separate Dokumentation in `TA5-Tracking/Dokumentation/TA5_Sockets.md`.

### Klasse Camera
Die Camera-Klasse ist der zentrale Punkt des Trackingsystems. Im Camera-Objekt werden alle Parameter der Kamera gespeichert und die Geradenparameter für die Triangulation werden vorbereitet. Außerdem wird über das Kameraobjekt der Videostream der Kameras ausgewertet.

Die Parameter der Kameras werden (bei Bedarf) in eine xml-Datei gespreichert, sodass diese bei einem Programmneustart nicht neu ermittelt werden müssen, sondern einfach nur geladen werden müssen.

Die Methoden für die Positionsbestimmung werden im Kapitel der Triangulation noch einmal beschrieben.

### Klasse ObjectDetection
Die Objekterkennung ist recht simpel, aber auch privimiv implementiert. Der aktuelle Algorithmus ist auch nur möglich, da in einem Innenraum mit ausschließlich künstlicher Beleuchtung geflogen wird.

Pro Kamera wird ein ObjectDetection-Objekt angelegt, dem das entsprechende Camera-Objekt bekannt ist.  
Zuerst muss dem ObjectDetection-Objekt über `setReferenceFrame` ein Referenzbild gegeben werden. Danach wird über `detectObject` das aktuelle Frame übergeben und die erkante Position auf dem Sensor zurückgegeben.

Das Tracking funktionier, indem einfach die Differenz aus dem Referenzbild (Hintergrund) und dem aktuelle Bild gebildet wird. Somit sind in dem Bild nur noch die Änderungen, also der Quadrocopter (und Rauschen) zu sehen.  
Um das Rauschen zu filtern wird das Bild noch mit einem Weichzeichner und einem binären Threshold nachberarbeitet.  
Mit der openCV-Funktion `findContours` werden dann zusammenhängende Konturen gesucht, von deren größten dann der Mittelpunkt als Position ausgegeben wird.


## Triangulation der Position im Raum
Um die Position des Objektes (Quadrocopter) im Raum bestimmen zu können, müssen zwei Geraden bestimmt werden, auf denen sich das Objekt befindet. Danach wird die näheste Annäherung der beiden Geraden errechnet (Triangulation). Zuerst werden aber die Kameraposition und -Ausrichtung benötigt.

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


## Debugging und Anmerkungen

#### Capture-Rule für Wirewhark
`(ip.addr == 141.22.27.193) && (udp.port == 1362)`

#### Anmerkungen
- entferne NIE den Aus-Schalter aus deinem Programm, wenn diese nur korrekt beendet werden kann, wenn die Destuktoren ausgeführt werden.

