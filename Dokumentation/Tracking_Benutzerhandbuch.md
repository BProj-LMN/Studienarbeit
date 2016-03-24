*Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung*
# Dokumentation - TA5 - Tracking


## Schnittstelle und Abhängigkeiten
- Verbindung zum Reglersystem per UDP-Socket
- opencv v3.0.0

## Ablauf
Es muss immer zuerst das Trackingsystem gestartet werden. Für den normalen Betrieb sollte der Modus `loadAndTrack` verwendet werden. Dies kann auch direkt als Argument an das Programm übergeben werden.  
Danach kann sich der Regler entsprechend des `SystemsConnectionProtocol` mit dem Trackingsystem verbinden, sodass die Ist-Koordinaten kontiuierlich per UDP-Paket an das Regelungssystem übertragen werden.  
Bei der Socket-Verbindung ist zu beachten, dass im HAW-Netz keine UDP-Paktete zwischen dem WLAN- und LAN-Netz übertragen werden. Der Rechner für das Reglersystem muss also per Kabel angeschlossen sein (oder es muss ein eigenes WLAN aufgebaut werden).

## Modi
Bei Start des Trackingssystem können diese Modi aungewählt werden:

- loadConfig
- calibrateCamera
- calibrate3D
- setFrameMask1
- setFrameMask2
- save
- tracking
- exit
- save&exit
- loadAndTrack

Eine dieser Optionen kann auch per Programmparameter beim Aufruf des Programms ausgewählt werden.  
Alle Modi bis auf "tracking" und "exit" kehren wieder auf die Auswahl zwischen diesen Optionen zurück.

Das Programm kann im Tracking-Modus jederzeit mit einem Tastendruck abgebrochen werden.


### loadConfig
Lädt die Kamerakonfiguration aus den Dateien, die im Sourcecode angegeben sind.

### calibrateCamera
Mit dieser Option werden die Verzeichnungsparamerter der Kameras bestimmt, die sog. intrinsischen Parameter. Es wird erst die Kalibrierung der ersten Kamera gestartet, wenn diese abgeschlossen ist, wird die zweite Kamera kalibriert.

Für die Kalibrierung muss ein Schachbrettmuster vor die Kamera gehalten werden. Wenn diese korrekt erkannt wurde (bunte Linien), kann die Kalibrierung begonnen werden.  
Zum start der halb-automatischen Kalibrierung muss die Taste "g" gedrückt werden. Nun werden mehere Bilder aufgenommen.  
Zwischen dem korrigierten und unkorrigierten Bild kann mit "u" gewechselt werden.  
Zum Beenden der aktuelle Kalibrierung muss "esc" gedrückt werden.

Die Kalibierung speichert die intrinsischen Parameter in der Camera-Klasse. Damit alle Parameter der Camera-Klasse auch in der cam.xml Datei gespeichert werden, muss "tracking" oder "exit" aufgerufen werden.

### calibrate3D
"calibrate3D" wird verwendet, um die extrinsischen Parameter der Kameras zu bestimmen.  
TODO Daniel

### setFrameMask1 bzw. setFrameMask2
setFrameMask gibt die Möglichkeit das Bild aus der Kamera zu maskieren, damit störende Bereiche ausgeblendet werden.  
Diese Maske wird automatisch angewendet, wenn ein neuen Frame vom `Camera`-Objekt abgefragt wird.

### save
Speichert die `Camera`-Objekte in die korrespondierenden xml-Dateien.

### tracking
Mit "tracking" werden die Kameraparamteter in der korrespondierenden xml-Datei gespeichert und das Programm geht in den Hauptmodus über, mit dem der Copter getrackt wird und die Position an das Reglersystem übertragen wird.

### exit
Beendet das Programm ohne die aktuelle Konfiguration der Kameras in der korrespondierenden xml-Datei gespeichert werden

### save&exit
Abkürzung für das Ausführen der Optionen `save` und `exit`.

### loadAndTrack
Abkürzung für das Ausführen der Optionen `loadConfig` und `tracking`. Wird verwendet, um mit einem Komandozeilenparameter direkt in das Tracking zu starten und damit in (die zukünftige) Steuerung per Socket.
