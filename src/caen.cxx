#include <caen.h>




uint32_t readRegister(int handle, unsigned int reg) {
  uint32_t data = 0; 
  auto err = CAENComm_Read32(handle, reg, &data);
  if (err) {
    std::cout << "Failed to read register error: " << err << " register:" << std::hex << reg << std::endl;
    return 0;
  }
  return data;
}

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
