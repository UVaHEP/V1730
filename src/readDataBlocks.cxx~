#include <CAENComm.h>
#include <iostream>
#include <fstream> 
#include <map>
#include <string> 

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>


using std::vector; 
using std::string; 
using std::nullptr_t; 




typedef struct Register {
  std::string name;
  uint32_t sAddress; // start address
  uint32_t eAddress; // end address
  uint32_t bAddress; // broadcast


} Register; 



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

typedef std::map<std::string, Register> registerMap;
typedef vector< boost::iterator_range<string::iterator> > find_vector_type;
typedef vector< string > split_vector_type;


void printEventStructure(CAEN_DGTZ_EventInfo_t e) { 
			 
  std::cout << std::hex << "Event Size:" << e.EventSize << std::endl;
  std::cout << "Board ID:" << e.BoardId << std::endl;
  std::cout << "Board Trigger Pattern:" << e.Pattern << std::endl;
  std::cout << "Channel Mask:" << e.ChannelMask << std::endl;
  std::cout << "Event Counter (hex):" << e.EventCounter << std::endl;
  std::cout << "Trigger Time Tag:" << e.TriggerTimeTag << std::endl;

}



registerMap* buildRegisterMap(string &fname) {
  std::cout << "Reading registers from: " << fname << std::endl;
  registerMap *m = new registerMap();
  split_vector_type splitVec;

  const unsigned int bfsz = 256; 
  char buf[bfsz]; 
  
  std::ifstream fin(fname);
  while (  fin.getline(buf, bfsz) ) {
    std::string l(buf);
    boost::split(splitVec, l, boost::is_any_of(","), boost::token_compress_on);


    
    Register r;
    switch(splitVec.size()) {
    case 1:
      break;
    case 2: {
      //Name and Address only
      r.name = splitVec[0];
      r.sAddress = std::stoi(splitVec[1], nullptr, 16);

      break;
    }
    case 3: {
      //Name, Start Address and End Address
      r.name = splitVec[0];
      r.sAddress = std::stoi(splitVec[1], nullptr, 16);
      r.eAddress = std::stoi(splitVec[2], nullptr, 16);
      break;
    }
    case 4: {
      //Name, Start, End, Broadcast
      r.name = splitVec[0];
      r.sAddress = std::stoi(splitVec[1], nullptr, 16);
      r.eAddress = std::stoi(splitVec[2], nullptr, 16);
      r.bAddress = std::stoi(splitVec[3], nullptr, 16); 
      break;
    }
    default:
      r.name = "Bad";
      break;
    }

    if (r.name != "Bad") {
      m->insert(std::pair<std::string, Register>(r.name, r)); 
    }
    

    if (fin.eof())
      break; 

  }
  
  fin.close(); 
  

  return m; 

}; 

uint32_t readRegister(int handle, unsigned int reg) {
  uint32_t data = 0; 
  auto err = CAENComm_Read32(handle, reg, &data);
  if (err) {
    std::cout << "Failed to read register error: " << err << " register:" << std::hex << reg << std::endl;
    return 0;
  }
  return data;

}





int main() {


  
  std::cout << "Hello CAEN" << std::endl;
  string fname = "VT1730Registers.csv";
  registerMap *rMap = buildRegisterMap(fname);

  if (rMap == nullptr) {
    std::cout << "Bad Register Map! Quitting." << std::endl;
    return -1;
  }

  /*for (auto& kv : (*rMap)) {
    std::cout << "Name:" << kv.first << ", Register:" << std::hex << kv.second.sAddress << std::endl;
    }*/
 
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

  int evStored = readRegister(handle, rMap->at("Event_Stored").sAddress);

    std::cout << std::dec << "Events Stored:" << evStored << std::endl;  


  if (evStored > 0) {
    
    std::cout << "Reading a data block" << std::endl;
    int evSize = readRegister(handle, rMap->at("Custom_Size").sAddress);
    int bOrganization = readRegister(handle, rMap->at("Buffer_Organization").sAddress); 
    int sampleCount = 0;

    std::cout << "Current Buffer Setting:" << std::hex << bOrganization << std::endl; 
    if (evSize) {
      std::cout << std::dec << "Custom Event Size" << evSize << std::endl;
      sampleCount = evSize*10;
    }
    else {
      sampleCount = 5000-10; //Hard coded for now, as this comes from the Buffer Organization value

    }
    std::cout << "We should get: " << std::dec << sampleCount << " samples." << std::endl; 

    /*
      Read samples, the VT1730 works like this when reading:
      the Digitizer puts samples into the Event ReadOut Buffer (0x0000 - 0x0FFC)
      as you read out samples it updates the buffer with new samples 
     */


    //Write to a file for testing 
    std::ofstream f;
    f.open("SampleData.dat");
    //First let's read the header
    auto *header = new uint32_t[4];
    header[0] = readRegister(handle, 0x0000);
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
    int evWords = event.EventSize-4; // # of words in this event, subtract 4 for the header
    std::vector < uint32_t > samples; //2 samples per word 
    samples.reserve(evWords*2);
    
    int address = 0x0004; 
    for (int i = 0; i < evWords-1; i++) { 
      uint32_t word = readRegister(handle, address);
      auto first = (word&0x03fff);
      auto second = (word&0x003fff0000)>>16;
      samples.push_back(first);
      samples.push_back(second); 
      f << std::hex << first <<","<<second << std::endl; 

      // When we reach the end of the readout buffer wrap around to the start 
      if (address == 0xFFC) 
	address = 0x0;
      else
	address++; 
      
    }
  }

  std::cout << "Done! Closing Device." << std::endl; 
    
  CAENComm_CloseDevice(handle); 


  return 0;
}
