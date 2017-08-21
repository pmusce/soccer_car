# Soccer Car

A small OpenGL game made for the Computer Graphic course A.A.2015/16.

## Introduzione
Soccer Car è un videogioco 3D interattivo di tipo "vehicular soccer video game". Il gioco è ambientato in un campo di calcio e si è al controllo di un auto d'epoca allo scopo di mandare la palla in rete.

## Installazione
*Soccer Car* gira in ambiente Linux e richiede che siano installate le librerie SDL, OpenGL e GLUT.

Per prima cosa bisogna compilare il codice sorgente, posizionandosi nella cartella di *Soccer Car* e lanciando:
```
make
```
Viene creata la cartella `build/` che conterrà i file `.o`. Al termine della compilazione il file eseguibile `play` viene creato nella directory principale.

Ora è possibile avviare *Soccer Car*:
```
./play
```

## Gameplay


## Controlli
Controllo auto:
* *W* - Accellera
* *S* - Frena
* *A* - Sterza a sinistra
* *D* - Sterza a destra
* *SPACE* - Salto

Controllo funzionalita extra:
* *TAB* - Apre la lista dei tasti
* *F1* - Cambia camera
* *F2* - Cambia palla
* *F3* - Fanali auto on/off

In modalità *mouse camera* è possibile controllare la camera utilizzando
il mouse:
* *MOUSE WHEEL* - Zoom in/out
* *CLICK + DRAG* - Muove camera

## Sviluppo

### Librerie utilizzate
### Mesh
In questo progetto si è deciso di utilizzare mesh salvate in file `.obj`, utilizzando anche le coordinate texture ivi contentute per il texture mapping e caricando le proprietà dei materiali utilizzati dalle mesh dai file `.mtl`.

Si è esteso quindi `mesh.h` e `mesh.c` (usati in progetto_car3),  implementando il caricamento materiali e texture da file.

#### Car
Il modello dell'auto è ottentua scaricando una mesh ad alto numero di verdici dalla libreria online *TurboSquid*. Successivamente è stato modificato in Blender, riducendo il numero di vertici (per migliorare le performance del rendering real-time) ed esportando le diverse parti dell'auto in file `.obj` separati.

#### Ball
È possibile scegliere fra due diversi tipi di palla:
1. Pallone da calcio
2. Sfera su cui è proiettata la texture di un'immagine personale



#### SkyBox
#### Campo
#### Goal

### Camera
### HUD

### Gameplay
#### Movimento auto
#### Collisioni
#### Punteggio
