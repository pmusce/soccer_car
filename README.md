# Soccer Car

A small OpenGL game made for the Computer Graphic course A.A.2015/16.

## Introduzione
Soccer Car è un videogioco 3D interattivo di tipo "vehicular soccer video game". Il gioco è ambientato in un campo di calcio e si è al controllo di un auto d'epoca allo scopo di mandare la palla in rete.

***

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

***

## Gameplay

***

## Controlli
Controllo auto:
* `W` - Accellera
* `S` - Frena
* `A` - Sterza a sinistra
* `D` - Sterza a destra
* `SPACE` - Salto

Controllo funzionalita extra:
* `TAB` - Apre la lista dei tasti
* `F1` - Cambia camera
* `F2` - Cambia palla
* `F3` - Fanali auto on/off

In modalità *mouse camera* è possibile controllare la camera utilizzando
il mouse:
* `MOUSE WHEEL` - Zoom in/out
* `CLICK + DRAG` - Muove camera

***

## Sviluppo

### Main

Sono state utilizzate le librerie Glut per gestire l'inizializzazione, l'input e il ciclio principale dell'applicazione.



<!-- ### Librerie utilizzate -->
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
Il cielo è ottenuto con una SkyBox, cioè un cubo che ha sulle facce texture del cielo in distanza. Ad esso si è deciso di non applicare l'illuminazione direzionale.

#### Campo
Il campo è un rettangolo su cui è applicata la texture di un campo da calcio. È stato applicato il tesseling al campo, per ottenere un maggior realismo dalle luci frontali dell'auto.

### Camera
È possibile scegliere tre differenti camere con il tasto `F1`:
1. Camera fissa sulla palla
2. Camera nella stessa direzione dell'auto
3. Camera controllabile da mouse

### HUD
L'HUD è costituito da 3 elementi:
1. Mappa
2. Score
3. Messaggio di goal avvenuto
4. Keymap alla pressione del tasto `TAB`

Per il testo a schermo è stata utilizzata la funzione *glutBitmapCharacter* messa a disposizione dalle librerie Glut, che renderizza un carattere bitmap.

La mappa è stata creata renderizzando a schermo un rettangolo semi-trasparente, utilizzando un alpha value <1.0 per il colore. I punti su mappa per identificare palla e veicolo sono `GL_POINTS` ottenuti abilitando il parametro `GL_POINT_SMOOTH` durante l'inizializzazione.

### Collisioni
Le collissioni vengono rilevate e calcolate considerando sia l'auto che la palla come due sfere.
Le collissioni vengono in questo modo approssimate utilizzando un algoritmo semplifcato, ma che mantiene comunque un certo realismo.


### Punteggio

Ad ogni istante viene controllato se la palla attraversa una delle porte. Quando questo avviene il goal viene rilevato per una delle due squadre ed il punteggio aggiornato. Dopo di che, allo scadere di un timeout, la scena viene reinizializzata, con la palla al centro del campo e l'auto nella metà campo della squadra che ha subito goal.

## Conclusioni
Il progetto *Soccer car* è un videogioco 3D interattivo con un gameplay semplice e un discreto livello di realismo, considerando gli standard odierni.

In futuro è possibile effettuare miglioramenti sia a livello grafico che di gameplay. Per esempio potrebbe essere interessante rendere *Soccer Car* un gioco multiplayer.

Per quanto riguarda le possibili migliorie grafiche, certamente è da considerare l'introduzione di ombre proiettate dagli oggetti.
È inoltre da prendere in considerazione lo sviluppo di shaders per un miglior tesseling del campo.
