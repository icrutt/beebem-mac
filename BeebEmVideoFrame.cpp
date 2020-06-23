//
//  BeebVideoFrame.cpp
//  BeebEm4
//
//  Created by Ian Rutt on 22/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#include "BeebEmVideoFrame.hpp"

BeebEmVideoFrame::BeebEmVideoFrame(char* theData, int xDataSize, int yDataSize, ScreenRect scrR)
{
    frameData = theData;
    screen = scrR;
    this->xDataSize = xDataSize;
    this->yDataSize = yDataSize;
}

char* BeebEmVideoFrame::getPointer() {
    return frameData;
}

char BeebEmVideoFrame::getVal(int i) {
    return frameData[i];
}

int BeebEmVideoFrame::getWidth() {
    return screen.right - screen.left;
}

int BeebEmVideoFrame::getHeight() {
    return screen.bottom - screen.top;
}

int BeebEmVideoFrame::getVOffset() {
    return screen.top;
}
