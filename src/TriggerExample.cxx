#include <caen.h>



int main() {


  // Load Registers
  string fname = "VT1730Registers.csv";
  registerMap *rMap = buildRegisterMap(fname);

  if (rMap == nullptr) {
    std::cout << "Bad Register Map! Quitting." << std::endl;
    return -1;
  }

  std::cout << "Hello CAEN" << std::endl;

  auto connectionType = CAENComm_PCIE_OpticalLink;

  int handle = 0;
  auto err = CAENComm_OpenDevice(connectionType, 0,0,0,&handle);
  std::cout << "Caen Comm ConnectionType Result: " << err << std::endl;

  if (err) {
    std::cout << "Could not open connection to V1730...quitting." << std::endl;
    return -1;
  }

  int bConfig = readRegister(handle, rMap->at("BoardConfigurationRead").sAddress);

  std::cout << std::hex << "Board Configuration:" << bConfig << std::endl; 

  int bInfo = readRegister(handle, rMap->at("Board_Info").sAddress);
  
  std::cout << std::hex << "Board Info:" << bInfo << std::endl;


  std::cout << "Samples per buffer:" << std::dec << readRegister(handle, rMap->at("Custom_Size").sAddress) << std::endl;

  //Enable Acquisition Control
  auto acqAddress = rMap->at("Acquisition_Control").sAddress; 
  uint16_t acq = 0xc; // SW Trigger, All triggers count
  err = CAENComm_Write16(handle, acqAddress, acq);
  if (err)
    std::cout << "Acquisition Control Write Failed" << std::endl; 


  //Software Trigger -- value written doesn't matter
  auto triggerAddress = rMap->at("Software_Trigger").sAddress; 
  err = CAENComm_Write32(handle, triggerAddress, 0xF);
  if (err)
    std::cout << "Software Trigger Failed " << std::endl;

  //Disable Acquisitions
  acq = 0x0; // Disable SW Trigger
  err = CAENComm_Write16(handle, acqAddress, acq);
  if (err)
    std::cout << "Acquisition Control Write Failed" << std::endl; 

  auto storedEventsReg = rMap->at("Event_Stored").sAddress;
  std::cout << "Current Stored Events:" << std::dec << readRegister(handle, storedEventsReg) << std::endl; 

  

  std::cout << "Done! Closing Device." << std::endl; 
    
  CAENComm_CloseDevice(handle); 




  return 0;

}
