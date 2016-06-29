*Studienarbeit SS 2016 - HAW Hamburg - optische Trackinganlage*
# Dokumentation - Objekterkennung

## Einleitung
Um den Copter (oder auch irgendein anderes Objekt) tracken zu können, muss diese erkennt werden. In dieser Dokumentation wird die Objekterkennung beschrieben, die im Bachelorprojekt entwickelt und implementiert wurde.


## Klasse ObjDetSimple
Die Objekterkennung ist recht simpel, aber auch primitiv implementiert. Der aktuelle Algorithmus ist auch nur möglich, da in einem Innenraum mit ausschließlich künstlicher Beleuchtung geflogen wird.

Zuerst muss dem ObjDetSimple-Objekt über `setReferenceFrame` ein Referenzbild gegeben werden. Danach wird über `detect` das aktuelle Frame übergeben und die erkannte Position auf dem Sensor zurückgegeben.

Das Tracking funktioniert, indem einfach die Differenz aus dem Referenzbild (Hintergrund) und dem aktuelle Bild gebildet wird. Somit sind in dem Bild nur noch die Änderungen, also der Quadrocopter (und Rauschen) zu sehen.  
Um das Rauschen zu filtern wird das Bild noch mit einem Weichzeichner und einem binären Threshold nachberarbeitet.  
Mit der openCV-Funktion `findContours` werden dann zusammenhängende Konturen gesucht, von deren größten dann der Mittelpunkt als Position ausgegeben wird.
