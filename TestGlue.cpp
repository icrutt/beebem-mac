//
//  TestGlue.cpp
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#include "TestGlue.hpp"

TestGlue::TestGlue(){
  newVal = false;
}
void TestGlue::sendValue(int inVal) {
  val = inVal;
  newVal = true;
}
bool TestGlue::isValueWaiting() {
  return newVal;
}
int TestGlue::getValue() {
  newVal = false;
  return val;
}
