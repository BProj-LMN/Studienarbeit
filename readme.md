*Studienarbeit SS 2016 - HAW Hamburg - Quadrocopter-Tracking*
# Readme (Github)

## Einleitung
Im Wintersemester 2015/16 wurde im Rahmen eines sog. Bachelorprojektes des Studiengangs Mechatronik ein Trackingsystem für einen Quadcopter (ready-to-fly) entwickelt. Beim Bachelorprojekt handelt es sich um eine Gruppenarbeit im 5. Semester, anhand derer Projektmanagementmethoden erlernt und ausprobiert werden sollen.

Das Trackingsystem arbeitet mit 2 Monochrom-Kameras und die Position im Raum wird per Triangulation ermittelt. Die Positionsdaten wurden verwendet, um den Copter automatisch in einem Raum fliegen zu lassen. Hierfür wurde noch eine entsprechende Regelungssortware entwickelt, die auch mit der Fernsteuerung des Copters kommunizert hat.  
Für die Bildverarbeitung wurde die freie Bibliothek [OpenCV](http://opencv.org/) verwendet.

## Aufgabe der Studienarbeit
Die Software für das Tracking, die im Bachelorprojekt entwickelt wurde, war jedoch nur ein Prototyp und daher auch sehr stark an die bestehende Hardware gebunden. Außerdem war die Architektur nicht zufriedenstellend, da diese unflexibel und schlecht testbar ist. Dies liegt jedoch daran, dass erst die Arbeit mit OpenCV erlernt werden musste und die Anforderungen nicht von Anfang an klar waren (wie fast immer bei Architekturentscheidungen).  
Außerdem gab es im Team zwar das Gefühl, dass das Trackingsystem langsam ist, die Performance wurde jedoch nie gemessen. Daher sollen die Datenrate und vor allem die Latenz ermittelt werden.

In der Studienarbeit wird nun zuerst die Performance des "alten" Systems analysiert.  
Danach soll eine neue Architektur entworfen und implementiert werden. Diese soll sich dadurch auszeichnen, dass sie gut konfigurierbar ist (auf andere Kamerasysteme anpassbar), Module einfach auszutauschen sind und dadurch eine gute Testbarkeit gegeben ist.  
Zuletzt wird das System mit der neuen Architektur den selben Performanceanalysen, wie zu Beginn der Arbeit, unterzogen, um einen Vergleich durchführen zu können.


## Organisation des Projektes
Alle Dateien sind genrell UTF-8 codiert und haben UNIX-style line breaks! Dateien, die im Code referenziert werden (können) sollten keine Leerzeichen enthalten.

### Ordnerstruktur
```
+-- Doku-Bachelorprojekt (Doku zur Einordnung des Systems / Hintergrundwissen)
+-- Dokumentation
+-- Prog-Trackingsystem      (Hauptprogamm)
+-- Prog-CalibrateCamera     (zum Kalibrieren der Linsenverzeichnung)
+-- Prog-CalibratePositions  (zum Einmessen der Kamerapositionen)
+-- Progs-configStore        (zentraler Ort zum Austausch von Config-Dateien zw. den Programmen)
+-- readme.md
```

Anmerkung: Das monolithische Programm aus dem Bachelorprojekt wurde in drei Teile aufgeteilt, um die Programme übersichtlicher und (theoretisch) unabhängig voneinander zu machen. Außerdem werden so die Use-Cases "Kalibrierung" und "Tracking" klarer voneinander getrennt.

### Dokumentation
Die Dokumentation erfolgt aus Bequemlichkeit so weit es möglich ist in Github-flavoured Markdown. UML-Diagramme werden mit StarUML erstellt, werden aber spätestens mit einer abgeschlossenen Version auch als PDF hinzugefügt.  
Recherchen und Messungen werden (vorerst) nicht im Git-Repo dokumentiert.

## coding und git style guide
Da dies mein Projekt ist, versuche ich mich an meine Regeln zu halten: <https://gist.github.com/jbeyerstedt/dc4dfe8859d3a274a19d>
