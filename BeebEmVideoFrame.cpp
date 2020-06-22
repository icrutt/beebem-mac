//
//  BeebVideoFrame.cpp
//  BeebEm4
//
//  Created by Ian Rutt on 22/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#include "BeebEmVideoFrame.hpp"

BeebEmVideoFrame::BeebEmVideoFrame(char* theData, int xSizePixels, int ySizePixels)
{
    frameData = theData;
    this->xSizePixels = xSizePixels;
    this->ySizePixels = ySizePixels;
}

char* BeebEmVideoFrame::getPointer() {
    return frameData;
}

char BeebEmVideoFrame::getVal(int i) {
    return frameData[i];
}
