# SenseHatC
Utilisation de la carte SenseHat avec un Raspberry PI via le Bus I2C.
Sources initiales https://github.com/moshegottlieb et https://github.com/davebm1 thanks you both for your job....
### SenseHat for the Raspberry PI not in Python but in C
L'utilisation de la SenseHat ce fait en C en utilisant les fichiers et les commandes i2c de la librairie libi2c-dev
Ici nous utiliserons uniquement la matrice de leds (frame buffer) et les capteurs de pression, d'humidité et de température.
## Install
Vous devez installer la librairie de developpement i2c:
`sudo apt-get install libi2c-dev`
et aussi activé le bus i2c
`sudo raspi-config` --> advanced options --> enable i2
Il peut être intéressant d'installer les outils i2c
`sudo apt-get install i2c-tools`
Ensuite dans le répertoire contenant ce projet:
`make`
`sudo make install`

Ensuite dans les fichiers C :
`#include <sense/fb.h>`<
Lier avec  `-lsense`
exemple pour l'exemple du projet qui est dans le répertoire courant du projet:
gcc -o testAffichageValeurDesCapteurs MaSenseHat.c affichagevaleurdescapteurs.c -lsense
ou
gcc -o testAffichageValeurDesCapteurs MaSenseHat.c affichagevaleurdescapteurs.c fb.c
mais make install à créé la librairie sense que vous pouvez utilisez avec gcc -l


