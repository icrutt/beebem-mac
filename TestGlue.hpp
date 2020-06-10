//
//  TestGlue.hpp
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef TestGlue_hpp
#define TestGlue_hpp

#include <stdio.h>
#include "GlueInterface.h"

class TestGlue : public GlueInterface {
private:
  int val;
  bool newVal;
public:
  TestGlue();
  void sendValue(int);
  bool isValueWaiting();
  int getValue();
};

#endif /* TestGlue_hpp */
