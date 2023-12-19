//Beck, TFT_eSPI_DriverSelect.cpp, 12/18/23a
#include <TFT_eSPI_DriverSelect.h>

void SelectDisplayDriver(enum eDisplayDriver eDisplay){
    switch (eDisplay){
      case eDisplayDriver::eSetup1_ILI9341:
        aucReceiverMACAddress[wMACByteNum]= aucBluePinMAC[wMACByteNum];
        break;
      case eDisplayDriver::eSetup21_ILI9488:
        aucReceiverMACAddress[wMACByteNum]= aucWhitePinMAC[wMACByteNum];
        break;
      case eDisplayDriver::eSetup25_TTGO_T_Display:
        aucReceiverMACAddress[wMACByteNum]= aucBlackPinMAC[wMACByteNum];
        break;
      case eDisplayDriver::eDriverNotSelected:
        //Do nothing and expect User_Setup_Select.h haks been hard-coded with driver to inclde
        break;
      default:
        Serial << "SelectDisplayDriver(): Bad switch= " << (int)eDisplay << endl;
    } //switch
    return;
} //
//Last line.
