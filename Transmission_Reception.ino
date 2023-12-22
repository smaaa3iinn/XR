  /*! \file *********************************************************************
 *
 * \brief
 *      LIAISON SERIE (Transmission)
 *
 * - Ce code configure la communication série USART sur un microcontrôleur AVR avec une fréquence d'horloge de 16 MHz et un taux de bauds de 9600 bps. 
 *
 * - La fonction USART_Transmit envoie continuellement le caractère ASCII 'A' (65) via la communication série. 
 *
 * - Ce programme peut être utilisé comme base pour tester la transmission série sur un microcontrôleur ou pour des applications simples nécessitant l'émission constante d'un caractère sur la ligne série.
 *

 *

 * \author
 * - Name: Ismail Khouna
 * - E-mail: ismail.khouna@etudiant.univ-reims.fr \n
 *
 * $Name: - RELEASE_2_0 $
 * $Revision: - 2.0 $
 * $RCSfile: - transmission.c,v $
 * $Date: - 30/11/2023 12:30:00 $  \n
 ******************************************************************************/

#define F_CPU 16000000UL              // Fréquence du CPU
#define BAUD 9600                     // Débit en bauds
#define MYUBRR F_CPU / 16 / BAUD - 1  // Calcul de la valeur d'UBRR

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int flag0 = 0;  // Variable pour indiquer la réception de données
volatile int flag3 = 0;
volatile char donnee;  // Variable pour stocker les données reçues

// Initialisation de l'USART0
void USART0_Init(unsigned int ubrr) {
  /* Configuration du taux de bauds */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Activation du récepteur et du transmetteur
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  /* Configuration du format de trame : 8 bits de données, 1 bit de stop */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Initialisation de l'USART3
void USART3_Init(unsigned int ubrr) {
  /* Configuration du taux de bauds */
  UBRR3H = (unsigned char)(ubrr >> 8);
  UBRR3L = (unsigned char)ubrr;
  // Activation du récepteur et du transmetteur
  UCSR3B = (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3);
  /* Configuration du format de trame : 8 bits de données, 1 bit de stop */
  UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
}

// Transmission via USART0
void USART0_Transmit(unsigned char data) {
  /* Attente du buffer de transmission vide */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Envoi des données dans le buffer */
  UDR0 = data;
}

// Transmission via USART3
void USART3_Transmit(unsigned char data) {
  /* Attente du buffer de transmission vide */
  while (!(UCSR3A & (1 << UDRE3)))
    ;
  /* Envoi des données dans le buffer */
  UDR3 = data;
}

// Réception via USART0
unsigned char USART0_Receive(void) {
  /* Attente des données à recevoir */
  while (!(UCSR0A & (1 << RXC0)))
    ;
  /* Récupération et renvoi des données reçues */
  return UDR0;
}

// Réception via USART3
unsigned char USART3_Receive(void) {
  /* Attente des données à recevoir */
  while (!(UCSR3A & (1 << RXC3)))
    ;
  /* Récupération et renvoi des données reçues */
  return UDR3;
}

// Interruption pour la réception sur USART0
ISR(USART0_RX_vect) {
  flag0 = 1;
}

// Interruption pour la réception sur USART3
ISR(USART3_RX_vect) {
  flag3 = 1;
}

int main(void) {
  USART0_Init(MYUBRR);  // Initialisation de l'USART0
  USART3_Init(MYUBRR);  // Initialisation de l'USART3
  sei();                // Activation des interruptions globales

  while (1) {
    if (flag3) {
      USART0_Transmit(UDR3);  // Transmission des données reçues par USART3 via USART0
      flag3 = 0;
    }
    if (flag0) {
      USART3_Transmit(UDR0);  // Transmission des données reçues par USART3 via USART0
      flag0 = 0;
    }
    _delay_ms(1);  // Délai d'attente
  }
}
