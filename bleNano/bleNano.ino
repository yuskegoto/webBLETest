#include<Arduino.h>
#include <nRF5x_BLE_API.h>


#define DEVICE_NAME            "Simulated RFduino"
#define LOCAL_NAME             "BLE_DEV"
#define TXRX_BUF_LEN           20

BLE                            ble;
Timeout                        timeout;
Ticker                         ticker_task1;

/*********** Pinconfig ***************/
#define ANALOG_IN A3
#define ADJUST_PIN D1

/*********** Parameters ***************/
#define TICKER_INTERVAL 1 // interval 1sec for update streaming
#define PACKET_LENGTH 4

uint16_t readVal = 0;

static const uint8_t service_uuid[]        = { 0x00, 0x00, 0x22, 0x20, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb } ;
static const uint8_t service_tx_uuid[]     = { 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb } ;
static const uint8_t service_rx_uuid[]     = { 0x00, 0x00, 0x22, 0x21, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb } ;
static const uint8_t service_uuid_rev[]    = { 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x20, 0x22, 0x00, 0x00 } ;

uint8_t tx_value[TXRX_BUF_LEN] = {0,};
uint8_t rx_value[TXRX_BUF_LEN] = {0,};

//TXRX seen from Central/Master side
GattCharacteristic  txCharacteristic(service_tx_uuid, tx_value, 1, TXRX_BUF_LEN, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE );
GattCharacteristic  rxCharacteristic(service_rx_uuid, rx_value, 1, TXRX_BUF_LEN, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
GattCharacteristic *uartChars[] = {&txCharacteristic, &rxCharacteristic};
GattService         uartService(service_uuid, uartChars, sizeof(uartChars) / sizeof(GattCharacteristic *));

//connect task
void connectionCallBack( const Gap::ConnectionCallbackParams_t *params ) {
  delay(10);
  digitalWrite(LED, HIGH);
}

//disconect tack
void disconnectionCallBack(const Gap::DisconnectionCallbackParams_t *params) {
  digitalWrite(LED, LOW);
  ble.startAdvertising();
}

void periodicCallback() 
{
    char packetChar[PACKET_LENGTH];   //char array to out put packet
    uint8_t packetInt[PACKET_LENGTH];  //int array of the equivalent

    sprintf(packetChar, "%04i", readVal);
    for(int8_t i = 0; i < PACKET_LENGTH; i++){
        packetInt[i] = packetChar[i];
    }

  ble.updateCharacteristicValue(rxCharacteristic.getValueAttribute().getHandle(), (uint8_t *)&packetInt, sizeof(packetInt));
}

void update(){
    readVal = analogRead(ANALOG_IN);
}

void setup() 
{
  pinMode(LED, OUTPUT);
  pinMode(ADJUST_PIN, INPUT_PULLUP);
  ticker_task1.attach(periodicCallback, TICKER_INTERVAL); // attach Ticker for writing value event

  ble.init();
  ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
  ble.onConnection(connectionCallBack);
  ble.onDisconnection(disconnectionCallBack);
  ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                   (const uint8_t *)LOCAL_NAME, sizeof(LOCAL_NAME) - 1);
  ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                   (const uint8_t *)service_uuid_rev, sizeof(service_uuid_rev));

  ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
  ble.addService(uartService);
  ble.setDeviceName((const uint8_t *) DEVICE_NAME);
  ble.setTxPower(4);
  ble.setAdvertisingInterval(160);
  ble.setAdvertisingTimeout(0);
  ble.startAdvertising();

  digitalWrite(LED, HIGH);  // on board led as power indicator
}

void loop() {
    update();
    ble.waitForEvent();
}
