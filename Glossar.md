*Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung*
# Glossar

- Quadrocopter oder Copter: beschreibt das Fluggerät
- Not-Aus: sofortiger Stopp aller Motoren (als Hardware-Schalter)
- Not-Halt: kontrolliertes Sinken
- Tracking: zweideutig:
	- 2D-Position des Copters auf einem Kamerabild
	- gesamtes Verfahren, um die Ist-Position zu ermitteln
- Sollwert: Endposition, die der Copter erreichen soll
- Trajektorie: Flugbahn. Dient als Führungsgröße für den Regler
- Führungsgröße: Sollwert des Flugreglers
- Istwert: aktuelle Position des Copters im Raum
- User-Interface (UI): 
- User: Person, die der Software die Soll-Position, sowie weitere Steuerkomandos vorgibt
- Regler: zweideutig:
	- als Flugregler: nur der Regler im Regelkreis
	- als Gesamtpaket aus Regler, Flugsteuerung (+Trajektorie) und Anbindung Fernsteuerung
- Flugregler: Regler, der den Copter an einer vorgegebenen Soll-Position (Führungsgröße) hält. Dieser regelt den Flug in kleinen Schritten.
- Flugsteuerung (Flightcontroller): Die Flugsteuerung ist dafür zuständig die Trajektorie zu berechnen und für die verschiedenen Flugmodi/ Sonderfunktionen den Regler bzw. die Fernsteuerung anzusprechen.

