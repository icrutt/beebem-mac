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

struct ScreenRect {
    int top;
    int bottom;
    int left;
    int right;
};

class BeebEmVideoFrame {
private:
    char* frameData;
    ScreenRect screen;
    int xDataSize, yDataSize;
public:
    BeebEmVideoFrame(char* data, int xDataSize, int yDataSize, ScreenRect scrR);
    char* getPointer();
    char getVal(int i);
    int getWidth();
    int getHeight();
    int getVOffset();
};

#endif /* BeebVideoFrame_hpp */
