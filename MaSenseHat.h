/***********************************************************************
 * Nom: GRILO				Fonction: Enseignant Informatique Indus.
 * Prénom: Christophe		Entreprise: Lycée les Eucalyptus - Nice (06)
 * Date de Création: 15/12/2017		Modification: 23/12/2017
 * ---------------------------------------------------------------------
 * 							MASENSEHAT.H
 * ---------------------------------------------------------------------
 * Contient les constantes et les déclaration des sous-programmes 
 * permettant d'accèder et d'interpréter les informations des capteurs 
 * de température, d'humidité et de pression de la SenseHat.
 * ---------------------------------------------------------------------
 * Avant de Compiler faire:
 * sudo raspi-config --> advanced options --> enable i2c
 * sudo apt-get install libi2c-dev i2c-tools
 * Puis compiler par exemple en utilisant:
 * gcc -o testfonction1 MaSenseHat.c affichagevaleurdescapteurs.c fb.c
 * si la librairie sense-master est installée: cd ....libsense-master
 * make
 * sudo make install
 * ********************************************************************/

#ifndef MASENSEHAT_H
#define MASENSEHAT_H

#define PRESSION_ID 0x5c
#define DEV_ID 0x5F
#define DEV_PATH "/dev/i2c-1"
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21

#define PRESS_OUT_XL 0x28
#define PRESS_OUT_L 0x29
#define PRESS_OUT_H 0x2A

#define TEMP_OUT_L 0x2B
#define TEMP_OUT_H 0x2C

#define H0_T0_OUT_L 0x36
#define H0_T0_OUT_H 0x37
#define H1_T0_OUT_L 0x3A
#define H1_T0_OUT_H 0x3B
#define H0_rH_x2 0x30
#define H1_rH_x2 0x31
#define H_T_OUT_L 0x28
#define H_T_OUT_H 0x29


/*----------------------------------------------------------------------
 * Procédure delay
 * ---------------------------------------------------------------------
 * Entrée: la tempo en millisecondes
 * Sortie: Aucune
 * ---------------------------------------------------------------------
 * Permet de se mettre en attente.
 * -------------------------------------------------------------------*/
void delay(int);

/*----------------------------------------------------------------------
 * Fonction pression
 * ---------------------------------------------------------------------
 * Entrée: Aucune
 * Sortie: Aucune
 * Retour: La pression atmosphérique en hPa.
 * ---------------------------------------------------------------------
 * Permet de connaître la pression atmosphérique fourni par le capteur
 * LPS25H via le Bus I2C.
 * -------------------------------------------------------------------*/
double pression();

/*----------------------------------------------------------------------
 * Fonction temperature
 * ---------------------------------------------------------------------
 * Entrée: Aucune
 * Sortie: Aucune
 * Retour: La température en °C.
 * ---------------------------------------------------------------------
 * Permet de connaître la temperature fourni par le capteur LPS25H via 
 * le Bus I2C.
 * -------------------------------------------------------------------*/
double temperature();

/*----------------------------------------------------------------------
 * Fonction humidite
 * ---------------------------------------------------------------------
 * Entrée: Aucune
 * Sortie: Aucune
 * Retour: L'humidité ambiante.
 * ---------------------------------------------------------------------
 * Permet de connaître l'humidité fourni par le capteur HTS221 via 
 * le Bus I2C.
 * -------------------------------------------------------------------*/
double humidite();

/*----------------------------------------------------------------------
 * Procédure  temperatureEtPression
 * ---------------------------------------------------------------------
 * Entrée: Aucune
 * Sortie: la temperature et la pression atmosphérique
 * ---------------------------------------------------------------------
 * Permet de connaître la temperature et al pression atmosphérique 
 * données par le capteur LPS25H via le Bus I2C. 
 * -------------------------------------------------------------------
 * Remarque: passage de paramètres par adresse nous sommes en C pur.
 * Le passage de paramètres par référence étant impossible.
 * -------------------------------------------------------------------*/
void temperatureEtPression(double *,double *);
#endif
