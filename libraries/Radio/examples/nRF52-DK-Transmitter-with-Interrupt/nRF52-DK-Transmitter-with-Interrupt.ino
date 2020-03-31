#include "radio_config.h"

static uint32_t packet;
volatile byte state = LOW;

/**@brief Function for initialization oscillators.
 */
void clock_initialization()
{
  /* Start 16 MHz crystal oscillator */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART    = 1;

  /* Wait for the external oscillator to start up */
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
  {
    // Do nothing.
  }

  /* Start low frequency crystal oscillator for app_timer(used by bsp)*/
  NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_LFCLKSTART    = 1;

  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
  {
    // Do nothing.
  }
}

/**@brief Function for sending packet.
 */
void send_packet()
{
  // send the packet:
  NRF_RADIO->EVENTS_READY = 0U;
  NRF_RADIO->TASKS_TXEN   = 1;

  while (NRF_RADIO->EVENTS_READY == 0U)
  {
    // wait
  }
  NRF_RADIO->EVENTS_END  = 0U;
  NRF_RADIO->TASKS_START = 1U;

  while (NRF_RADIO->EVENTS_END == 0U)
  {
    // wait
  }
  
  Serial.println("The packet was sent\n\r");

  NRF_RADIO->EVENTS_DISABLED = 0U;
  // Disable radio
  NRF_RADIO->TASKS_DISABLE = 1U;

  while (NRF_RADIO->EVENTS_DISABLED == 0U)
  {
    // wait
  }
}

void setup() {
  // put your setup code here, to run once:
  clock_initialization();
  radio_configure();
  NRF_RADIO->PACKETPTR = (uint32_t)&packet;

  Serial.begin(115200);
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(2), interruptHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), interruptHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(4), interruptHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(5), interruptHandler, FALLING);
  
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

void interruptHandler() {
  if (digitalRead(2) == LOW) packet = 1;
  if (digitalRead(3) == LOW) packet = 2;
  if (digitalRead(4) == LOW) packet = 3;
  if (digitalRead(5) == LOW) packet = 4;

  digitalWrite(6, LOW);
  send_packet();
  digitalWrite(6, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
}
