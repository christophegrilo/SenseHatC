/***********************************************************************
 * Nom: GRILO				Fonction: Enseignant Informatique Indus.
 * Prénom: Christophe		Entreprise: Lycée les Eucalyptus - Nice (06)
 * Date de Création: 15/12/2017		Modification: 23/12/2017
 * ---------------------------------------------------------------------
 * 							MASENSEHAT.C
 * ---------------------------------------------------------------------
 * Contient les sous-programmes permettant d'accèder et d'interpréter 
 * les informations des capteurs de température, d'humidité et de 
 * pression de la SenseHat.
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
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <fcntl.h>
#include "MaSenseHat.h"

/*----------------------------------------------------------------------
 * Procédure delay
 * ---------------------------------------------------------------------
 * Entrée: la tempo en millisecondes
 * Sortie: Aucune
 * ---------------------------------------------------------------------
 * Permet de se mettre en attente.
 * -------------------------------------------------------------------*/
void delay(int t)
{
    usleep(t * 1000);
}

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
double pression()
{
	int fd = 0;
   
    uint8_t press_out_xl = 0;
    uint8_t press_out_l = 0;
    uint8_t press_out_h = 0;

    int32_t press_out = 0;
    double pressure = 0.0;

    uint8_t status = 0;

    /* Ouverture de la communication i2c */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
	perror("Unable to open i2c device");
	exit(1);
    }

    /* Configuration de l'esclave i2c, le capteur de pression  */
    if (ioctl(fd, I2C_SLAVE, PRESSION_ID) < 0) {
	perror("Unable to configure i2c slave device");
	close(fd);
	exit(1);
    }

    /* Verification que le registre WHO_AM_I définit bien que nous sommes
     * le capteur de pression */
    if (i2c_smbus_read_byte_data(fd, WHO_AM_I) != 0xBD) {
	printf("%s\n", "who_am_i error");
	close(fd);
	exit(1);
    }

    /* Arrêt du composant */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    /* Demarrage du capteur de pression en mode "single shot"  */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x84);

    /* Acquisition de la pression,  le "set bit" sera mis à 0 par le 
     * capteur lui même à la fin de l'exécution.(self-clearing bit) */
    i2c_smbus_write_byte_data(fd, CTRL_REG2, 0x01);

    /* Attendre que la mesure soit totalement réalisée */
    do 
    {
		delay(25);		/* 25 millisecondes */
		status = i2c_smbus_read_byte_data(fd, CTRL_REG2);
    }
    while (status != 0);

    /* Lecture de la mesure de la pression (3 octets à lire) */
    press_out_xl = i2c_smbus_read_byte_data(fd, PRESS_OUT_XL);
    press_out_l = i2c_smbus_read_byte_data(fd, PRESS_OUT_L);
    press_out_h = i2c_smbus_read_byte_data(fd, PRESS_OUT_H);

    /* Application de l'algorithme de calcul */
    press_out = press_out_h << 16 | press_out_l << 8 | press_out_xl;
    pressure = press_out / 4096.0;

    /* Affichage de sortie */
    printf("Pressure = %.0f hPa\n", pressure);

    /* Arrêter le composant */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);
    close(fd);
   
    return pressure;
}

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
double temperature()
{
	int fd = 0;
    uint8_t temp_out_l = 0, temp_out_h = 0;
    int16_t temp_out = 0;
    double t_c = 0.0;

    uint8_t status = 0;

    /* Ouverture de la communication i2c */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
	perror("Unable to open i2c device");
	exit(1);
    }

    /* Configuration de l'esclave i2c, le capteur de pression qui est 
     * aussi le capteur de température */
    if (ioctl(fd, I2C_SLAVE, PRESSION_ID) < 0) {
	perror("Unable to configure i2c slave device");
	close(fd);
	exit(1);
    }

    /* Verification que le registre WHO_AM_I définit bien que nous sommes
     * le capteur de pression qui est aussi le capteur de température */
    if (i2c_smbus_read_byte_data(fd, WHO_AM_I) != 0xBD) {
	printf("%s\n", "who_am_i error");
	close(fd);
	exit(1);
    }

     /* Arrêt du composant (clean start) */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

   /* Demarrage du capteur de pression en mode "single shot"  */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x84);

    
    /* Acquisition de la temperature,  le "set bit" sera mis à 0 par le 
     * capteur lui même à la fin de l'exécution.(self-clearing bit) */
    i2c_smbus_write_byte_data(fd, CTRL_REG2, 0x01);

    /* Attendre que la mesure soit totalement réalisée */
    do 
    {
		delay(25);		/* 25 millisecondes */
		status = i2c_smbus_read_byte_data(fd, CTRL_REG2);
    }
    while (status != 0);

    /* Lecture de la mesure de la temperature (2 octets à lire) */
    temp_out_l = i2c_smbus_read_byte_data(fd, TEMP_OUT_L);
    temp_out_h = i2c_smbus_read_byte_data(fd, TEMP_OUT_H);

    /* Application de l'algorithme de calcul */
    temp_out = temp_out_h << 8 | temp_out_l;
    t_c = 42.5 + (temp_out / 480.0);

    /* Affichage de la temperature */
    printf("Temp (from press) = %.2f°C\n", t_c);

    /* Arrêter le composant */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);
    close(fd);
    
    return t_c;
}

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
double humidite()
{
	int fd = 0;
    uint8_t status = 0;
	uint8_t h0_out_l = 0, h0_out_h = 0;
    uint8_t h1_out_l = 0, h1_out_h = 0;
    uint8_t h0_rh_x2 = 0, h1_rh_x2 = 0;
    uint8_t h_t_out_l = 0, h_t_out_h = 0;

    int16_t h_t_out = 0; 
    int16_t h0_t0_out = 0, h1_t0_out = 0;
    
    double H0_rH = 0.0, H1_rH = 0.0;
    double h_gradient_m = 0.0, h_intercept_c = 0.0;
    double H_rH = 0.0;
    
    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
	perror("Unable to open i2c device");
	exit(1);
    }

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, DEV_ID) < 0) {
	perror("Unable to configure i2c slave device");
	close(fd);
	exit(1);
    }

    /* check we are who we should be */
    if (i2c_smbus_read_byte_data(fd, WHO_AM_I) != 0xBC) {
	printf("%s\n", "who_am_i error");
	close(fd);
	exit(1);
    }

    /* Power down the device (clean start) */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    /* Turn on the humidity sensor analog front end in single shot mode  */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x84);

    /* Run one-shot measurement (temperature and humidity). The set bit will be reset by the
     * sensor itself after execution (self-clearing bit) */
    i2c_smbus_write_byte_data(fd, CTRL_REG2, 0x01);

    /* Wait until the measurement is completed */
    do {
	delay(25);		/* 25 milliseconds */
	status = i2c_smbus_read_byte_data(fd, CTRL_REG2);
    }
    while (status != 0);

    /* Read calibration relative humidity LSB (ADC) data
     * (humidity calibration x-data for two points)
     */
    h0_out_l = i2c_smbus_read_byte_data(fd, H0_T0_OUT_L);
    h0_out_h = i2c_smbus_read_byte_data(fd, H0_T0_OUT_H);
    h1_out_l = i2c_smbus_read_byte_data(fd, H1_T0_OUT_L);
    h1_out_h = i2c_smbus_read_byte_data(fd, H1_T0_OUT_H);

    /* Read relative humidity (% rH) data
     * (humidity calibration y-data for two points)
     */
    h0_rh_x2 = i2c_smbus_read_byte_data(fd, H0_rH_x2);
    h1_rh_x2 = i2c_smbus_read_byte_data(fd, H1_rH_x2);

    /* make 16 bit values (bit shift)
     * (humidity calibration x-values)
     */
    h0_t0_out = h0_out_h << 8 | h0_out_l;
    h1_t0_out = h1_out_h << 8 | h1_out_l;

    /* Humidity calibration values
     * (humidity calibration y-values)
     */
    H0_rH = h0_rh_x2 / 2.0;
    H1_rH = h1_rh_x2 / 2.0;

    /* Solve the linear equasions 'y = mx + c' to give the
     * calibration straight line graphs for humidity
     */
    h_gradient_m = (H1_rH - H0_rH) / (h1_t0_out - h0_t0_out);
    h_intercept_c = H1_rH - (h_gradient_m * h1_t0_out);

   /* Read the ambient humidity measurement (2 bytes to read) */
    h_t_out_l = i2c_smbus_read_byte_data(fd, H_T_OUT_L);
    h_t_out_h = i2c_smbus_read_byte_data(fd, H_T_OUT_H);

    /* make 16 bit value */
    h_t_out = h_t_out_h << 8 | h_t_out_l;

    /* Calculate ambient humidity */
    H_rH = (h_gradient_m * h_t_out) + h_intercept_c;

    /* Output */
    printf("Humidity = %.0f%% rH\n", H_rH);

    /* Power down the device */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);
    close(fd);

    return H_rH;
}

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
void temperatureEtPression(double *laTemperature,double *laPression)
{
	*laTemperature=temperature();
	*laPression=pression();
}
