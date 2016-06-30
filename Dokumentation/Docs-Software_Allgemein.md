*Studienarbeit SS 2016 - HAW Hamburg - optische Trackinganlage*
# Dokumentation - Software allgemein


## Schnittstelle und Abhängigkeiten
- Verbindung zum Reglersystem per UDP-Socket
- opencv v3.1.0

## Programme
Um das Programm aus dem Bachelorprojekt übersichtlicher und flexibler zu machen wurde das Programm in drei Teile geteilt. Somit sind drei Use Cases voneinander getrennt:

-  das Kalibireren der Kamera an sich (nur ein Mal nötig) ==> **Prog-CalibrateCamera**
-  das Kalibrieren der Kamerapositionen im Raum ==> **Prog-CalibratePositions**
-  das Tracking (benutzt Config-Dateien der anderen Programme) ==> **Prog-Trackingsystem**

## Anmerkungen
In dieser Dokumentation wird nicht explizit auf die Softwarearchitektur (siehe `Architecture-Model`) und die entsprechenden Designentscheidungen eingegangen.

Einzelne Teile der Programme, wie die Objekterkennung, sowie die Triangulation wurden in eine eigene Datei ausgelagert.

Der Ordner `Progs-configStore` dient als zentrale Ablage für die Config-Dateien, die von der Kalibrierprogrammen geschrieben und von Trackingsystem benutzt werden.


## Prog-Trackingsystem
Das Trackingsystem benötigt eine sysConfig.yml Datei nach entsprechender Dokumentation im `Architecture-Model`. Ein Beispiel liegt im Ordner `Progs-configStore`. Der Pfad muss im Programm in der Datei `main_tracking.cpp` angegeben werden.

Alle "Schalter" durch Präprozessordirektiven sind an der entsprechenden Stelle kommentiert.

#### UDP-Verbindung
Die aktuellen Positionsdaten werden zur Zeit über die Klasse `ComBachelorprojekt` versendet, welche das im Bachelorprojekt speziell für diesen Anwendungsfall entwickelte Protokoll verwendet. Diese ist in `Protocol-ComBachelorprojekt.md` dokumentiert.  
Man kann sich nachdem das Tracking läuft, d.h. alle Refernezbilder aufgenommen wurden, jederzeit im laufenden Betrieb mit dem System verbinden. Es ist jedoch nur eine Verbingung gleichzeitig möglich und die Verbindung kann nicht getrennt werden.


## Prog-CalibrateCamera
Diese Programm wird benötigt, um die intrinsichen Parameter einer Kamera zu bestimmen. Dies sind die Linsenverzeichnungsparameter und die Kameramatrix, die den Mittelpunkt des Sensors und die Brennweite (jeweils in Pixeln) enthält.  
Für die Kalibrierung wird ein das Schachbrettmuster `calibrateCamera-pattern.png` benötigt, das vor die Kamera gehalten werden muss. Wenn diese korrekt erkannt wurde (bunte Linien), kann die Kalibrierung begonnen werden.  
Zum start der halb-automatischen Kalibrierung muss die Taste "g" gedrückt werden. Nun werden mehere Bilder aufgenommen.  
Zwischen dem korrigierten und unkorrigierten Bild kann mit "u" gewechselt werden.  
Zum Beenden der aktuelle Kalibrierung muss "esc" gedrückt werden.

Die Benutzerschnittstelle sollte selbsterklärend sein, die Schritte werden hier aber noch einmal aufgelistet:

- Eingabe der Kamera Device ID
- Eingabe des Pfades der Config-Datei (mit Dateiendung .yml)

Diese beiden Parameter können auch beim Start als Parameter an das Programm übergeben werden. Device ID und Pfad durch ein Leerzeichen getrennt.

- calibrateCamera (führt die Kalibrierung durch)
- save (speichern nicht vergessen)
- exit (Programm beenden)


## Prog-CalibratePositions
Aufgrund einiger Probleme mit der Triangulation in openCV wird die Kameraposition und -Ausrichtung mittlerweile nicht mehr mit openCV und den Funktionen in `calibrate3D.h` durchgeführt.

Die Kameraposition wird zur Zeit manuell ermittelt und auch die Triangulation wurde selbst implementiert.

Daher liegt dieses Programm aktuell brach und sollte durch eine (semi-)automatische Kalibrierung der Kamerapositionen ersetzt werden!



## Debugging und Anmerkungen

#### Capture-Rule für Wireshark
`(ip.addr == 141.22.27.193) && (udp.port == 1362)`
