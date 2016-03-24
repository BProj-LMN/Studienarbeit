*Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung*
# Dokumentation - TA5 - Socket


## Socket Client und Server

- UDP Socket Client: SocketClient Klasse  
- UDP Socket Server: Socket Klasse


### Konventionen
Die Methoden geben ihren Status als integer Rückgabewert zurück. Dabei gilt:

- 0 ==> OK
- -1 ==> ERROR

Einige Methoden geben aber auch ein bool-Wert zurück, da hierbei der Rückgabewert eine spezielle Bedeutung hat. Diese ist dann weiter unten erklärt.


### Socket Client
Der UDP-Client verbindet sich mit dem UDP-Server. Damit der Server Nachrichten an den Client senden kann, muss dieser erst einmal eine Nachricht an den Server senden, damit die IP-Adresse des Clients dem Server bekannt ist.

#### Einstellungen (#define)
Die einzige Einstellung, die nicht zur Laufzeit geändert werden kann, ist die maximale Nachrichtenlänge. Diese wird mit `MESSAGE_LEN` vorgegenen.

#### Methoden
- SocketClient(char ipAddress[], int port);
- void evaluate();
- bool get_message(std::string & message);
- int sendMessage(char message[MESSAGE_LEN]);

Mit `SocketClient` wird ein Objekt des Clients erstellt. Hierfür muss zuerst die IP-Adresse des Servers angegeben werden. Diese wird einfach als char-Array übergeben. Außerdem muss dem Client der Port des Servers mitgeteilt werden.

In jedem Programmzyklus sollte `evaluate()` aufgerufen werden, damit der Socket nach neuen Paketen anbgefragt wird.

Mit `get_message` wird der Inhalt des letzten Paktets in den string gespeichert, der der Methode übergeben wird. Der Rückgabewert gibt an, ob das Paket neu ist, d.h. nicht schon einmal mit diese Methode abgefragt wurde.

Mit `sendMessage` wird die übergebene Nachricht gesendet. Der Rückgabewert gibt an, ob ein Fehler beim Senden aufgetreten ist. Der genaue Fehler wird in stderr ausgegeben.


### Socket Server
Der Server öffnet einen Socket auf einem vorgegebenen Port und wartet auf eintreffende Pakete. Auf diese Pakete kann dann geantwortet werden. Mit der aktuellen Implementation ist leider nicht möglich mehere Clients gleichzeitig zu bedienen!

#### Einstellungen (#define)
Die einzige Einstellung, die nicht zur Laufzeit geändert werden kann, ist die maximale Nachrichtenlänge. Diese wird mit `MESSAGE_LEN` vorgegenen.

#### Methoden
- Socket(int port);
- void evaluate();
- bool is_clientConnected();
- bool get_message(std::string & message);
- int sendMessage(char message[MESSAGE_LEN]);

Mit `Socket` wird ein Objekt des Server erstellt. Hierfür muss ein Port angegeben werden, der dann natürlich auch dem Client mitgeteilt werden muss.

In jedem Programmzyklus sollte `evaluate()` aufgerufen werden, damit der Socket nach neuen Paketen anbgefragt wird.

Mit `is_clientConnected()` kann abgefragt werden, ob der Client schon ein `connect`-Paket gesendet hat. Nur wenn der Client connected ist, können Pakete gesendet werden.

Mit `get_message` wird der Inhalt des letzten Paktets in den string gespeichert, der der Methode übergeben wird. Der Rückgabewert gibt an, ob das Paket neu ist, d.h. nicht schon einmal mit diese Methode abgefragt wurde.

Mit `sendMessage` wird die übergebene Nachricht gesendet. Der Rückgabewert gibt an, ob ein Fehler beim Senden aufgetreten ist. Der genaue Fehler wird in stderr ausgegeben.


### Designhinweise
`sendMessage` wird ein char-Array übergeben, da die "internen" Methoden der Sockets auch ein char-Array erwarten. Bei `get_message` wird jedoch eine Referenz auf einen std::string erwartet, da mit strings das Kopieren per einfacher Zuweisung durchgeführt werden kann.


## Installationshinweise

#### Socket auf localhost von Windows:

Für alle, die es nicht wissen: die IP-Adresse vom localhost ist 127.0.0.1

- in Systemsteuerung auf "Windows features aktivieren"
- Checkbox bei "Internetinformationsdienste" anklicken
- mit minGW und fehlendem winsock experimentieren oder visualStudio verwenden

Unter Windows darf anscheinend kein Port vergeben werden, der höher als 5000 ist!!!
