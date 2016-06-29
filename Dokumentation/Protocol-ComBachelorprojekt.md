*Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung*
# Dokumentation - SystemsConnectionProtocol


## Einleitung
Das SystemsConnectionProtocol ist die essentielle Verbindung zwischen dem Trackingsystem um dem Regelungssystem. Die Verbindung wird über Sockets und UDP-Pakete realisiert.

## Kenndaten

- Port 1362
- IP-Adresse Trackingsystem 141.22.27.193 (zur Zeit)

## Protokoll

### Vorbedingung
Bevor Daten ausgetauscht werden, muss sich das Reglersystem mit einem Paket mit der Nachricht `connect` beim Trackingsystem anmelden. Dies wird mit `you are connected` bestätigt.


### allgemeiner Telegrammaufbau für Daten vom Trackingsystem
- Identifier des Datentyps (1 Byte)
- Daten beliebiger Länge


### Positionsdaten (Tracking -> Regler)
Das Trackingsystem sendet periodisch die aktuell errechnete Ist-Position. Die Positionswerte sind 16-Bit integer die die Längeneinheit `mm` darstellen.

#### Telegrammaufbau
8 Byte Daten:

- ID: 0xDA
- X-Position (high byte)
- X-Position (low byte)
- Y-Position (high byte)
- Y-Position (low byte)
- Z-Position (high byte)
- Z-Position (low byte)
- Fehlercodes

#### Fehlercodes
```
Bit 0:     Tracking verloren
Bit 1:     Abstand der Geraden zu groß
Bit 2:     nc - für spätere Erweiterungen
Bit 3:     nc - für spätere Erweiterungen
Bit 7..4 : Fehlerdetails
```


### Trackingsystem abschalten (Tracking <- Regler)
Das Trackingsystem kann auch über den Socket beendet werden.

#### Telegrammaufbau
4 Byte String:

- "exit"

