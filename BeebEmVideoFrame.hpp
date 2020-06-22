//
//  BeebVideoFrame.hpp
//  BeebEm4
//
//  Created by Ian Rutt on 22/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BeebEmVideoFrame_hpp
#define BeebEmVideoFrame_hpp

#include <stdio.h>

class BeebEmVideoFrame {
private:
    char* frameData;
    int xSizePixels, ySizePixels;
public:
    BeebEmVideoFrame(char* data, int xSizePixels, int ySizePixels);
    char* getPointer();
    char getVal(int i);
};

#endif /* BeebVideoFrame_hpp */
