#ifndef _CAEN_H
#define _CAEN_H



#include <CAENComm.h>
#include <iostream>
#include <fstream> 
#include <chrono>
#include <thread> 
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



uint32_t readRegister(int handle, unsigned int reg);
void printEventStructure(CAEN_DGTZ_EventInfo_t e);
registerMap* buildRegisterMap(string &fname); 

#endif
