//
//  GlueInterface.h
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef GlueInterface_h
#define GlueInterface_h

class GlueInterface {
public:
  virtual void sendValue(int) = 0;
  virtual bool isValueWaiting() = 0;
  virtual int getValue() = 0;
  virtual void sendVideoFrame(char*) = 0;
  virtual char* getVideoFrame() = 0;
};

#endif /* GlueInterface_h */
