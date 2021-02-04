#include <LBLE.h>
#include <LBLECentral.h>
#include <LBLEPeriphral.h>

LBLEAdvDataItem item;

byte seq = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("BLE begin");
  LBLE.begin();
  while (!LBLE.ready()) {
    delay(100);
  }
  Serial.println("BLE ready");
  
//  LBLEPeripheral.setName("NTUIOT-BLE");
//  Serial.println("setName: NTUIOT-BLE");
  
  Serial.print("Device Address = [");
  Serial.print(LBLE.getDeviceAddress());
  Serial.println("]");

  LBLEUuid uuid("E2C56DB5-DFFB-48D2-B060-D0F5A71096E0");
  //  beaconData.configAsIBeacon(uuid, 01, 02, -40);
  //  beaconData.configAsEddystoneURL(EDDY_HTTPS, "labs.mediatek", EDDY_DOT_COM);

  Serial.print("Start advertising iBeacon with uuid=");
  Serial.println(uuid);

  //  LBLEPeripheral.advertiseAsBeacon(beaconData);
}

void loop() {
  item.clear();
  item.adType = 0x00;  // BT_GAP_LE_AD_TYPE_SERVICE_DATA
  item.adData[0] = seq++;
  item.adDataLen = 1;
  //  strcpy((char *)item.adData, "hello");
  //  item.adDataLen = strlen("hello");

  LBLEPeripheral.stopAdvertise();
  LBLEAdvertisementData beaconData;
  beaconData.addName("NTUIOT-BLE");
  beaconData.addAdvertisementData(item);
  LBLEPeripheral.advertiseAsBeacon(beaconData, 700, -10);
//  LBLEPeripheral.advertise(beaconData);

  if (((int) (millis() / 1000)) % 3 == 0) {
    // list advertisements found.
    Serial.print("Total ");
    Serial.print(LBLECentral.getPeripheralCount());
    Serial.println(" devices found:");
    Serial.println("idx\taddress\t\t\tflag\tRSSI");
    for (int i = 0; i < LBLECentral.getPeripheralCount(); ++i) {
      printDeviceInfo(i);
    }
    LBLECentral.stopScan();
    Serial.println("------scan restart-------");
    LBLECentral.scan();
  }
    
  delay(1000);
}

void printDeviceInfo(int i) {
  Serial.print(i);
  Serial.print("\t");
  Serial.print(LBLECentral.getAddress(i));
  Serial.print("\t");
  Serial.print(LBLECentral.getAdvertisementFlag(i), HEX);
  Serial.print("\t");
  Serial.print(LBLECentral.getRSSI(i));
  Serial.print("\t");
  const String name = LBLECentral.getName(i);
  Serial.print(name);
  if(name.length() == 0)
  {
    Serial.print("(Unknown)");
  }
  Serial.print(" by ");
  const String manu = LBLECentral.getManufacturer(i);
  Serial.print(manu);
  Serial.print(", service: ");
  if (!LBLECentral.getServiceUuid(i).isEmpty()) {
    Serial.print(LBLECentral.getServiceUuid(i));
  } else {
    Serial.print("(no service info)");
  }

  if (LBLECentral.isIBeacon(i)) {
    LBLEUuid uuid;
    uint16_t major = 0, minor = 0;
    int8_t txPower = 0;
    LBLECentral.getIBeaconInfo(i, uuid, major, minor, txPower);

    Serial.print(" ");
    Serial.print("iBeacon->");
    Serial.print("  UUID: ");
    Serial.print(uuid);
    Serial.print("\tMajor:");
    Serial.print(major);
    Serial.print("\tMinor:");
    Serial.print(minor);
    Serial.print("\ttxPower:");
    Serial.print(txPower);
  }

  Serial.println();
}
