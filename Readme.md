<<<<<<< HEAD
# Codes sources de la conférence : Optimiser son code sur µControleur

Conférence présentée par [Nao974] pour [Robotic974] chez [EPITECH Réunion] le 25/03/2022 en présence des étudiants de 1ere à 3eme année.

Ce dépôt reprend les grandes lignes de la conférence et les différents codes sources présentés.


## Introduction

Les MCU (Micro Controller Unit) sont principalement utilisés dans les systèmes autonomes. Dans la suite de cette présentation, nous simplifierons un système embarqué comme un système autonome mais en déplacement.

Cela implique des contraintes physiques différentes en fonction du système :
- Taille
- Poids
- Energie
- Robustesse
- Etc...

 Mais également des contraintes techniques :
 - Sureté de fonctionnement
 - Sécurité des données
 - Et des contraintes très fortes sur :
   - La taille Mémoire
   - La Vitesse de traitement
   - La Réactivité du système

Par exemple le MCU Atmel 328P de la carte Arduino est cadencé à 16Mhz et n'a que 2Ko de RAM.

 Afin d'imager la contrainte de Réactivité, prenons l'exemple d'un véhicule autonome avec le châssis Maqueen et une carte Micro:bit.

Nous le programmons dans un 1er temps avec un code simple avec MakeCode, en fonction de 2 capteurs infra-rouge placés dessous à l'avant, nous commandons les moteurs afin de suivre une piste noire : 

<p >
    <img src="assets/img_video_suiveur_ligne_simple.jpg"  width="75%" alt="Code Suivi ligne avec capteur distance US">
</p>

**Le véhicule autonome suit la route sans aucun problème :**

<p>
    <img src="assets/video_suiveur_ligne_simple.gif"  alt="Video Suivi ligne avec capteur distance US">
</p>

Nous rajoutons maintenant la lecture de distance d'un objet par le capteur ultrason, si un obstacle est devant à moins de 10cm alors le véhicule s'arrête, sinon il continu à suivre la piste :

<p>
    <img src="assets/video_suiveur_ligne+capteur_distance_us.jpg"  width="75%" alt="Code Suivi ligne simple">
</p>

**Le véhicule NE SUIT PLUS LA ROUTE :**
<p>
    <img src="assets/video_suiveur_ligne+capteur_distance_us.gif"  alt="Video Suivi ligne avec capteur distance US">
</p>

### Pourquoi ?

Les librairies standards utilisent une fonction bloquante de type PulseIn() afin de mesurer le temps écoulé entre l'émission des trames ultra-son et leurs retours une fois qu'elles ont rebondi sur un éventuel obstacle.

Ces capteurs peuvent en moyenne mesurer une distance max de 3 mètres, la vitesse du son étend de 347 m/s (à 25°), le MCU pourra donc être bloqué peandant 173 ms (sans obstacle devant) à chaque lecture de distance. 

*Cela est suffisant pour laisser dévier notre véhicule autonome en dehors de la route.*

***Il est donc important de faire la chasse aux fonctions bloquantes !!!***

Ressources additionnelles : 
- Mesure de distance par interruption : https://github.com/Robotic974/atmega328p-registers-and-interrupts
- Mise en œuvre  de 2x capteurs Ultra-son : https://youtu.be/Ve35dOHQbLI

___

## Notre projet de démonstration

### K2000

<p> <img src="assets/K2000.gif"  alt="Video K2000"> </p>

Ce qui caractérise ce véhicule autonome est ..... le jeux de lumière à l'avant !!!

**Notre  Objectif :**

Ecrire le code d'un chenillard avec 8 Leds. Ce code devra être léger et ne devra pas bloquer les autres fonctions de notre véhicule autonome.

Afin de simplifier le code, l'animation ne se fera que dans un sens (pas d'aller-retour).

**Schéma de Câblage**
<p> <img src="assets/cablage.png"  alt="Câblage" width="75%"> </p>
Aucune difficulté particulière dans le câblage, chaque LED étend allumée l'une après l'autre, nous pouvons prendre le 5V directement à chaque broche du MCU.

___

## 01_BASIC_BOUCLE_FOR

*(code dans le dossier /src)*

Rien de compliqué pour commencer, 
- Une boucle `for` dans le `setup()` afin d'initialiser les broches du MCU en `Sortie` où sont branchées les LEDs.

- Puis dans la boucle principale, une autre boucle `for` : 
   - Nous allumons une LED, 
   - Nous attendons 75ms afin que note œil puisse capter la lumière
   - Nous éteignons la LED
   - La boucle `for` passe à la LED suivante 
- Une fois la dernière LED traitée, nous sortons de la boucle `for`
- La boucle principale relance une nouvelle boucle `for`

*Le code semble propre, fonctionnel, correspondre aux différents tutoriels que nous pouvons trouver sur internet, mais ...*

### **Quelques Règles de déclaration**

### Type des variables

La taille mémoire minimale allouable est 1 octet, donc déclarer un `boolean` est trompeur car en mémoire la variable ne prendra pas 1 bit mais 8 bits (1 octet).

Les différents types d'entier `int` ne sont pas parlant sur leur taille réelle en mémoire, de plus, en fonction du µControleur, il ne prendra pas forcément la même taille en mémoire :

<p> <img src="assets/taille_int.png"  width="75%" alt="Taille d'un INT"> </p>

Il faut donc privilégier les types décrivant dans leur nom, le nombre de bits utilisés :

- `int8_t`   au lieu du `char`
- `uint8_t`  au lieu du `byte` (char non signé)
- `int16_t`  au lieu du `int`
- `uint32_t` ... 

Nous voyons tout de suite la place utilisée en mémoire. 

### Déclaration des constantes

Il convient de ne pas utiliser  `#define` car le type de la constante n'est pas défini.

Privilégier la déclaration :
```java
const uint8_t NB_LED = 8;
```  

### Variable Globale

Notre code est un petit exemple, les variables globales déclarées sont utilisées dans le `setup()` et le `loop()`, nous pouvons donc les laisser en globale mais il convient de bien comprendre le fonctionnement de la mémoire : https://quai-lab.com/arduino-ses-memoires


<p><font color="red"> <u> <h1>LE GROS PROBLEME</h1> </u> </font></p>

L'utilisation de la fonction :
```cpp
delay(75);
```
C'est une fonction **BLOQUANTE !!!**, cela va bloquer toutes les autres fonctions pendant   8 * 75 ms = 600 ms à chaque boucle, notre véhicule autonome ira droit dans le mur... le code 02 va résoudre ce problème.
___

## 02_MILLIS

la fonction `millis()` retourne le nombre de millisecondes écoulées depuis le démarrage/reset de la carte, il suffit de le comparer avec celui de notre derniere action sur le chenillard, si l'intervalle est >75ms alors nous passons à la LED suivante, nous remplaçons donc notre fonction bloquante par une condition.

C'est beaucoup mieux,

- Plus de fonction bloquante 
- Code portable sur différents MPU

*Mais nous pouvons l'optimiser afin de prendre beaucoup moins de cycle d'horloge pour notre chenillard.*

___

## 03_MILLIS+PORTD

**Attention**, nous entamons notre descente au cœur  du MCU, le code va devenir spécialisé pour notre µcontroleur Atmel 328P, et ne sera donc plus portable avec d'autres cartes. Nous allons également nous éloigner du framework `Arduino`.

Vous aurez besoin pour cela des documents suivants :

- Le schéma [PINOUT] de la carte Arduino Uno 
- La DataSheet du µControleur [Atmel 328P]


### Le PortD
Les différentes broches digitales du 328P sont regroupées dans des `PORT`, pilotés par des registres.

Notre montage utilise les broches 0 à 7, ce n'est pas un hasard car celles-ci sont toutes rattachées au PORT D. C'est le seul port de l'Arduino UNO regroupant 8 broches (voir le schéma [PINOUT]).

*Par contre les broches 0 et 1 sont également branchées en interne de la carte sur le port série. Il convient donc à chaque téléversement de votre programme de débrancher ces broches de votre montage.*

Nous allons dans 1er temps initialiser ce PORT digital en `Sortie` grâce au registre DDR :

```cpp
DDRD  = B11111111; // Paramétrage des broches du port D en mode OUTPUT
```
Cela remplace la boucle `for` dans le `setup();`

Le registre PORTD permet d'appliquer la valeur 0 (0V) ou 1 (5V) à l'ensemble des broches en 1 seule instruction :
```cpp
PORTD = B00000000; // On éteint toutes les Leds
```

Il nous reste plus qu'à modifier notre ancien code afin de remplacer la fonction `digitalWrite()` par l'affectation de ce registre avec un décalage à gauche pour chaque changement de LED :
```cpp
PORTD = B00000001  << led++; 
led %=  NB_LED; // Permet de repasser de la Led 7++ à la Led 0
```

L'affectation direct d'un PORT est 6 à 8 fois plus rapide que la fonction `digitalWrite()`, sachant que nous affectons directement les 8 broches en une seule opération, je vous laisse calculer le temps gagné.

Je vous laisse découvrir ce qui se cache derrière la fonction [digitalWrite()] afin de mieux comprendre sa lenteur.
___

## 04_PORTD+TIMER
C'est super, notre code est 40 fois plus rapide qu'au début, sans fonction bloquante.... Mais que se passera-t-il si mes différentes fonctions de ma boucle principale (lecture des capteurs, machine à état, etc...) durent plus 75 ms...
Vous pouvez faire le test en rajoutant un `delay(1500);` en fin du `loop()` dans le précèdent exercice.

Mon chenillard super fluide a pris un sacré coup dans l'aile, l'animation rame...

Bon là, va falloir s'accrocher est commencer par bien comprendre comment fonctionne un TIMER en travaillant les tutoriels I à III : https://www.locoduino.org/spip.php?article84

OK, je suis d'accord ce n’est pas simple, mais je vous conseille de créer une calculette avec votre tableur préféré afin de trouver le bon prédiviseur, la valeur de débordement, et nombre de Tac pour votre variable *compteurTimer* .

Donc, vous l'aurez compris, un TIMER est un compteur que nous allons paramétrer afin de diviser la vitesse de l'horloge jusqu'a avoir un tic toutes les 75ms.
A ce moment le TIMER lèvera une interruption de débordement et le µControleur arrêtera sa boucle principale afin d'exécuter notre fonction `ISR(TIMER2_COMPA_vect)` comprenant l'affectation du PORTD vu précédemment.

Le code source est largement commenté mais pour bien comprendre le fonctionnement il convient de travailler les tutoriels cités plus haut.

A noter que les variables utilisées dans la routine d'interruption (ISR) doivent être déclarées avec le qualificatif `volatile` afin de prévenir le compilateur quelles peuvent être modifiée par un moyen extérieur au programme en cours (*par interruption donc*).

___

## En Conclusion

Voilà, nous avons maintenant un code très performant, sans fonction bloquante, dont le déclenchement n'est plus lié à la boucle principale.

Nous n'utilisons plus aucune fonction du framework Arduino, par contre ce code est maintenant spécifique à un seul micro contrôleur.

**Bon code à tous !**
___

## Organisation des codes sources

Chaque exercice proposé correspond à un programme Arduino indépendant des autres exercices. Néanmoins, pour ne pas avoir à gérer autant de projets PlatformIO qu'il y'a d'exercices, on peut s'arranger pour tous les faire coexister au sein d'un même projet. 

Chaque exercice est traité dans un fichier source portant l'extension `.cpp` et stocké dans le dossier `src`. Par défaut, PlatformIO se charge de compiler tous les fichiers sources qu'il trouve dans le dossier `src`, et notamment le traditionnel `main.cpp`. Aussi, pour modifier ce comportement, il existe une directive très pratique à insérer dans le fichier `platformio.ini` qui va nous permettre d'indiquer précisément ce qu'il faut compiler ou non. Par exemple, si l'on souhaite compiler le programme décrit dans le fichier `01_Basic_Boucle-For.cpp`, et uniquement celui-là, il suffira de le spécifier à l'aide de la directive `src_filter` de la manière suivante :

```ini
[env:uno]
platform   = atmelavr
board 	   = uno
framework  = arduino
src_filter = -<*> +<01_Basic_Boucle-For.cpp>
```

Ici on construit une liste de fichiers à compiler, en commençant par exclure tous les fichiers se trouvant dans le dossier `src` avec la balise `-<*>`, puis en insérant le seul fichier `01_Basic_Boucle-For.cpp` que l'on souhaite compiler avec la balise `+<01_Basic_Boucle-For.cpp>`.
=======
# Contrôle d'un chenillard à 8 LEDs par un bouton poussoir

L'objectif de cet atelier est de poursuivre le travail que nous avons commencé sur le [chenillard à 8 LEDs][led-chaser]. Nous allons ajouter un bouton poussoir dans notre petit circuit électronique pour commander la rampe de LEDs. J'utilise ici une carte [Arduino Nano][nano], mais vous pouvez tout à fait réaliser le même montage avec une carte [Arduino Uno][uno]. Le raccordement des LEDs et du bouton poussoir sur les broches de commandes numériques de la carte Arduino est exactement le même.

<table>
    <tbody>
        <tr>
            <td><img src="https://store-cdn.arduino.cc/uni/catalog/product/cache/1/image/1040x660/604a3538c15e081937dbfbd20aa60aad/a/0/a000066_featured_1_2.jpg" alt="Arduino Uno"></td>
            <td><img src="https://store-cdn.arduino.cc/uni/catalog/product/cache/1/image/1040x660/604a3538c15e081937dbfbd20aa60aad/A/0/A000005_featured_2.jpg" alt="Arduino Nano"></td>
        </tr>
    </tbody>
</table>


## Schéma de câblage du prototype

Les anodes (+) de chaque LED sont respectivement reliées, dans le même ordre, aux broches **D5** à **D12** (de droite à gauche) de la carte Arduino. Les cathodes (-), quant à elles, sont toutes reliées à la masse, c'est-à-dire à la broche **GND** de la carte Arduino, au travers d'une résistance de **220 Ω** pour limiter l'intensité du courant qui traversera les LEDs, et les protéger d'une dégradation qui leur serait fatale.

On utilisera ici deux platines de prototypage pour que le câblage ne soit pas trop étriqué :

<table>
    <tbody>
        <tr>
            <td><img src="assets/wiring.png" alt="Circuit"></td>
            <td><img src="assets/breadboard.jpg" alt="Circuit"></td>
        </tr>
    </tbody>
</table>

Le bouton poussoir, quant à lui, est relié à la broche **D2** de la carte Arduino pour qu'on puisse lire son état (enfoncé / relâché), ainsi qu'à la broche d'alimentation **5V** de la carte Arduino, qui permettra de faire circuler un courant électrique lorsque le bouton sera enfoncé et, dans le même temps, de basculer la broche de lecture **D2** à l'état `HIGH`. Lorsque le bouton est relâché, pour éviter que la broche de lecture **D2** ne se trouve dans un état flottant et oscille intempestivement entre les états logiques `HIGH` et `LOW`, il convient de la relier à la masse (**GND**) au travers d'une résistance de rappel de **10 kΩ**. Cette résistance est ainsi caractérisée comme une résistance **pull-down** et permet de maintenir l'état de la broche de lecture **D2** au niveau logique `LOW` lorsque le bouton est relâché. La valeur élevée de la résistance permet de limiter l'intensité du courant à une valeur très faible lorsque le bouton est enfoncé et que le circuit est fermé entre la broche d'alimentation **5V** et la masse.

<p align="center">
    <img src="assets/pull-down-button.png" width="551" height="241" alt="Câblage d'une résistance de rappel pull-down">
</p>


## Effet rebond du bouton poussoir

Le bouton est un dispositif mécanique dans lequel les éléments de contacts s'entrechoquent lorsqu'on enfonce ou relâche le poussoir. À l'échelle temporelle de traitement de l'information, induite par l'activité électronique au niveau de la carte Arduino, on observe donc une phase de transition pendant laquelle apparaissent des effets rebonds indésirables. Le bouton ne change pas instantanément d'état lorsqu'on agit dessus, et la broche de lecture de la carte Arduino va donc détecter une succession de changements d'états logiques `HIGH` / `LOW` avant de se stabiliser. Voilà par exemple ce que l'on peut observer à l'oscilloscope lorsqu'on presse le bouton :

<p align="center">
    <img src="assets/bouncing.png" width="522" height="302" alt="Rebonds mécaniques observés sur un bouton poussoir">
</p>

Un phénomène similaire apparaît également lorsqu'on relâche le bouton et que la broche de lecture repasse à l'état `LOW`. Le profil des changements d'états qui surviennent pendant la phase transitoire où l'on observe les effets rebonds dépend de plusieurs facteurs comme la structure du bouton lui-même, ou la manière dont l'utilisateur enfonce ou relâche le poussoir. La durée de cette phase peut également être variable d'un bouton à l'autre.

Nous allons voir, avec les exercices proposés, comment mettre en évidence ce phénomène, et surtout comment en tenir compte pour éliminer ses effets indésirables et gérer proprement la réponse du micro-contrôleur aux signaux perturbés induits par l'action de l'utilisateur sur le bouton.


## Configuration du projet PlatformIO

La configuration du projet est définie par les directives inscrites dans le fichier `platformio.ini` :

```ini
[env:led-chaser]
platform  = atmelavr
board     = nanoatmega328
framework = arduino
```

Le projet est ici configuré pour être téléversé sur une carte Arduino **Nano**. Si, de votre côté, vous utilisez une carte Arduino **Uno**, vous devez remplacer la désignation de la carte `board` par la valeur `uno` :

```ini
[env:led-chaser]
platform  = atmelavr
board     = uno
framework = arduino
```


## Organisation des codes sources

Chaque exercice proposé correspond à un programme Arduino indépendant des autres exercices. Néanmoins, pour ne pas avoir à gérer autant de projets PlatformIO qu'il y'a d'exercices, on peut s'arranger pour tous les faire coexister au sein d'un même projet. Il existe différentes façons de mettre cela en oeuvre avec PlatformIO, et nous avons déjà appliqué l'une d'entre elles dans [l'atelier précédent][led-chaser]. Cette fois, nous allons procéder autrement.

Chaque exercice est traité dans un fichier source portant l'extension `.cpp` et stocké dans le dossier `src`. Par défaut, PlatformIO se charge de compiler tous les fichiers sources qu'il trouve dans le dossier `src`, et notamment le traditionnel `main.cpp`. Aussi, pour modifier ce comportement, il existe une directive très pratique à insérer dans le fichier `platformio.ini` qui va nous permettre d'indiquer précisément ce qu'il faut compiler ou non. Par exemple, si l'on souhaite compiler le programme décrit dans le fichier `01-basic-button.cpp`, et uniquement celui-là, il suffira de le spécifier à l'aide de la directive `src_filter` de la manière suivante :

```ini
[env:led-chaser]
platform   = atmelavr
board      = uno
framework  = arduino
src_filter = -<*> +<01-basic-button.cpp>
```

Ici on construit une liste de fichiers à compiler, en commençant par exclure tous les fichiers se trouvant dans le dossier `src` avec la balise `-<*>`, puis en insérant le seul fichier `01-basic-button.cpp` que l'on souhaite compiler avec la balise `+<01-basic-button.cpp>`.
>>>>>>> 693c0fd (Add LICENCE and first Readme.md)

Par conséquent, pour compiler un autre programme, vous devrez préciser le nom du fichier correspondant avec la balise appropriée. Chaque fichier solution est spécifique et indépendant des autres. Vous ne pouvez donc en compiler qu'un seul à la fois.

*Reportez-vous à la documentation officielle de PlatformIO pour plus de détails sur [la directive `src_filter`][src-filter].*


<<<<<<< HEAD
[EPITECH Réunion]: https://www.epitech.eu/fr/ecole-informatique-la-reunion
[Robotic974]:      https://www.facebook.com/robotic974
[Nao974]:          https://www.youtube.com/channel/UC9U0Txo279cyaHdZIZnFn_g

[Atmel 328P]:                https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
[PINOUT]:          assets/pinout_328P.pdf
[digitalWrite()]:  https://garretlab.web.fc2.com/en/arduino/inside/hardware/arduino/avr/cores/arduino/wiring_digital.c/digitalWrite.html
=======
**Bon code !**


[led-chaser]: https://github.com/Robotic974/led-chaser
[nano]:       https://store.arduino.cc/arduino-nano
[uno]:        https://store.arduino.cc/arduino-uno-rev3
>>>>>>> 693c0fd (Add LICENCE and first Readme.md)
[src-filter]: https://docs.platformio.org/en/latest/projectconf/section_env_build.html#src-filter