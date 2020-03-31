#include "radio_config.h"

static uint32_t packet;

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

/**@brief Function for reading packet.
 */
uint32_t read_packet()
{
  uint32_t result = 0;

  NRF_RADIO->EVENTS_READY = 0U;
  // Enable radio and wait for ready
  NRF_RADIO->TASKS_RXEN = 1U;

  while (NRF_RADIO->EVENTS_READY == 0U)
  {
    // wait
  }
  NRF_RADIO->EVENTS_END = 0U;
  // Start listening and wait for address received event
  NRF_RADIO->TASKS_START = 1U;

  // Wait for end of packet or buttons state changed
  while (NRF_RADIO->EVENTS_END == 0U)
  {
    // wait
  }

  if (NRF_RADIO->CRCSTATUS == 1U)
  {
    result = packet;
  }
  NRF_RADIO->EVENTS_DISABLED = 0U;
  // Disable radio
  NRF_RADIO->TASKS_DISABLE = 1U;

  while (NRF_RADIO->EVENTS_DISABLED == 0U)
  {
    // wait
  }
  return result;
}

void setup() {
  // put your setup code here, to run once:
  clock_initialization();
  radio_configure();
  NRF_RADIO->PACKETPTR = (uint32_t)&packet;
  
  Serial.begin(115200);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  
  uint32_t received = read_packet();
  
  if (received == 1) digitalWrite(6, LOW);
  if (received == 2) digitalWrite(7, LOW);
  if (received == 3) digitalWrite(8, LOW);
  if (received == 4) digitalWrite(9, LOW);

  delay(10);
}
