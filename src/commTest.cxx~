#include <CAENComm.h>
#include <iostream>
#include <fstream> 
#include <chrono>
#include <thread> 


//Pulled from Caen's Digitizer Library
#define MAX_UINT16_CHANNEL_SIZE     (8)

typedef struct 
{
    uint32_t             EventSize;
    uint32_t             BoardId;
    uint32_t             Pattern;
    uint32_t             ChannelMask;
    uint32_t             EventCounter;
    uint32_t             TriggerTimeTag;
} CAEN_DGTZ_EventInfo_t;

typedef struct 
{
    uint32_t            ChSize[MAX_UINT16_CHANNEL_SIZE]; // the number of samples stored in DataChannel array  
    uint16_t            *DataChannel[MAX_UINT16_CHANNEL_SIZE]; // the array of ChSize samples
} CAEN_DGTZ_UINT16_EVENT_t;


uint32_t readRegister(int handle, unsigned int reg) {
  uint32_t data = 0; 
  auto err = CAENComm_Read32(handle, reg, &data);
  if (err) {
    std::cout << "Failed to read register error: " << err << " register:" << std::hex << reg << std::endl;
    return 0;
  }
  return data;

}

void printChStatus(int handle) {

  auto chStatus = { 0x1088, 0x1188, 0x1288, 0x1388, 0x1488, 0x1588,
		    0x1688, 0x1788 };


  uint32_t data = 0; 
  for (auto ch : chStatus) {
    auto err = CAENComm_Read32(handle, ch, &data);
    if (err) {
      std::cout << "Failed to read channel status: " << ch << " error:" << err << std::endl;
      return;
    }
    std::cout << "Ch: " << ch << " Status:" << std::hex << data << std::endl;
  }
}


void printEventStructure(CAEN_DGTZ_EventInfo_t e) { 
			 
  std::cout << std::hex << "Event Size:" << e.EventSize << std::endl;
  std::cout << "Board ID:" << e.BoardId << std::endl;
  std::cout << "Board Trigger Pattern:" << e.Pattern << std::endl;
  std::cout << "Channel Mask:" << e.ChannelMask << std::endl;
  std::cout << "Event Counter (hex):" << e.EventCounter << std::endl;
  std::cout << "Trigger Time Tag:" << e.TriggerTimeTag << std::endl;

}

int main() {

  std::cout << "Hello CAEN" << std::endl;

  auto connectionType = CAENComm_PCIE_OpticalLink;

  int handle = 0;
  auto err = CAENComm_OpenDevice(connectionType, 0,0,0,&handle);
  std::cout << "Caen Comm ConnectionType Result: " << err << std::endl;


  auto resetAdd = 0xEF24; 
  auto bInfoAdd = 0x8140;
  auto bIDAdd = 0xEF08; 
  auto adcCalAdd = 0x809C;
  
  uint32_t data = 0; 


  /*  //Reset the Digitizer
  err = CAENComm_Write32(handle, resetAdd, 0x1);
  if (err)
    std::cout << "Failed to Reset the Digitizer" << std::endl;

  //Wait for digitizer to stabilize
  std::this_thread::sleep_for(std::chrono::milliseconds(300)); 

  std::cout << "Calibrating ADCs" << std::endl;
  err = CAENComm_Write32(handle, adcCalAdd, 0x1);

  std::this_thread::sleep_for(std::chrono::milliseconds(600)); 
  */
  err = CAENComm_Read32(handle, bInfoAdd, &data);
  std::cout << "Board Info Error: " << err << std::endl <<
    "Result:" << std::hex << data << std::endl; 
  
  err = CAENComm_Read32(handle, bIDAdd, &data);
  std::cout << "Board ID Error: " << err << std::endl <<
    "Result:" << std::hex << data << std::endl; 


  auto maxEvent = 0xEF1C;
  


  //Dunno if this uses 0 offset or 1
  //0 offset! Note it may cause a system crash if you read an unused register
  auto channels = {0x10A8, 0x11A8, 0x12A8, 0x13A8, 0x14A8, 0x15A8,
		  0x16A8, 0x17A8 }; 


  for (auto ch : channels) {
    uint32_t chData = 0; 
    err = CAENComm_Read32(handle, ch, &chData);
    if (err) {
      std::cout << "Channel Temp Read Error: " << err << " Channel:" << ch << std::endl;
    }
    int temp = chData & 0x0FF; 
    std::cout << "Channel: " << ch << " Int Temperature:" << std::dec << temp   << std::endl;
      
    }


  

  auto boardConfig = 0x8000;
  std::cout << "Board Config: " << std::hex << readRegister(handle, boardConfig) << std::endl;

  auto boardOrganization = 0x800C;
  std::cout << "Board Organization:" << std::hex << readRegister(handle, boardOrganization) << std::endl;


  auto customSizeReg = 0x8020;
  std::cout << "Samples per buffer:" << std::dec << readRegister(handle, customSizeReg) << std::endl;


  uint32_t myConfig = 0x18;
  /*  auto boardConfigWrite = 0x8004;

  err = CAENComm_Write32(handle, boardConfigWrite, myConfig);
  if (err)
    std::cout << "Failed to write new board configuration" << std::endl;
  */
  std::cout << "Board Config: " << std::hex << readRegister(handle, boardConfig) << std::endl; 
  
  std::cout << "Channel 0 Dynamic Range: " << std::hex << readRegister(handle, 0x1028) << std::endl; 

  //Enable Acquisition Control
  auto acqAddress = 0x8100;
  uint16_t acq = 0xc; // SW Trigger, All triggers count
  err = CAENComm_Write16(handle, acqAddress, acq);
  if (err)
    std::cout << "Acquisition Control Write Failed" << std::endl; 
  
  //Software Trigger -- value written doesn't matter
  auto triggerAddress = 0x8108; 
  err = CAENComm_Write32(handle, triggerAddress, 0xF);
  if (err)
    std::cout << "Software Trigger Failed " << std::endl;

  //Disable Acquisitions
  acq = 0x0; // SW Trigger, All triggers count
  err = CAENComm_Write16(handle, acqAddress, acq);
  if (err)
    std::cout << "Acquisition Control Write Failed" << std::endl; 

  
  auto readoutStatus = 0xEF04;

  std::cout << "Readout Status: " << std::hex << readRegister(handle, readoutStatus) << std::endl;

  printChStatus(handle);


  auto channelEnabledReg = 0x8120;
  std::cout << "Channel Enabled Mask:" << std::hex << readRegister(handle, channelEnabledReg) << std::endl; 


  auto storedEventsReg = 0x812C;
  std::cout << "Stored Events:" << std::dec << readRegister(handle, storedEventsReg) << std::endl; 


  /*
  //Let's try to read some data!
  auto startAddress = 0x0000;

  //Each header has a length of 4, 32-bit words
  auto *header = new uint32_t[4];
  header[0] = readRegister(handle, startAddress);
  header[1] = readRegister(handle, 0x0001);
  header[2] = readRegister(handle, 0x0002);
  header[3] = readRegister(handle, 0x0003);
  CAEN_DGTZ_EventInfo_t event;
  std::cout << "Header[0]:" << std::hex << header[0] << std::endl; 
  event.EventSize = header[0]&0x0FFFFFFF;
  event.BoardId = header[1]&0xF8000000;
  event.Pattern = header[1]&0x0FFFF00;
  event.ChannelMask = header[1]&0x0FF; // 0-7
  event.ChannelMask |= (header[2]&0xF0000000); //8-15
  event.EventCounter = header[2]&0x0FFFFFFF;
  event.TriggerTimeTag = header[3]; 

  printEventStructure(event); 
  
  std::ofstream f;
  f.open("SampleData.dat");
  
  //Grab some samples
  auto *samples = new uint32_t[0xFF8]; 
  for (int i = 0; i< 0xFF8; i++) {
    samples[i] = readRegister(handle, 0x0004+i);
    auto first = (samples[i]&0x03fff);
    auto second = (samples[i]&0x003fff0000)>>16;
    std::cout << std::hex << "1:"<<first<<std::endl <<
      "2:"<<second << std::endl;
    f << std::hex << first <<","<<second << std::endl; 

  }


  
  f << "\n\n\nStart Second Grab" << std::endl; 
   //Grab more samples
  delete samples; 
  samples = new uint32_t[0xFF8]; 
  for (int i = 0; i< 0xFF8; i++) {
    samples[i] = readRegister(handle, 0x0004+i);
    auto first = (samples[i]&0x03fff);
    auto second = (samples[i]&0x003fff0000)>>16;
    std::cout << std::hex << "1:"<<first<<std::endl <<
      "2:"<<second << std::endl;
    f << std::hex << first <<","<<second << std::endl; 

  }

  f << "\n\n\nStart Third Grab" << std::endl; 
   //Grab more samples
  delete samples; 
  samples = new uint32_t[0xFF8]; 
  for (int i = 0; i< 0xFF8; i++) {
    samples[i] = readRegister(handle, 0x0004+i);
    auto first = (samples[i]&0x03fff);
    auto second = (samples[i]&0x003fff0000)>>16;
    std::cout << std::hex << "1:"<<first<<std::endl <<
      "2:"<<second << std::endl;
    f << std::hex << first <<","<<second << std::endl; 

  }


  */
  
  CAENComm_CloseDevice(handle); 



  return 0; 
}


