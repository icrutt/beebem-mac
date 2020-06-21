//
//  GlueInterface.h
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef GlueInterface_h
#define GlueInterface_h

#include "BeebEvent.hpp"

class GlueInterface {
public:
  virtual void sendVideoFrame(char*) = 0;
  virtual char* getVideoFrame() = 0;
  virtual void sendEvent(BeebEvent*) = 0;
  virtual BeebEvent* getEvent() = 0;
};

#endif /* GlueInterface_h */
