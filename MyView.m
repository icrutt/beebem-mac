//
//  MyView.m
//  BeebEm4
//
//  Created by Ian Rutt on 17/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import "MyView.h"

CGContextRef MyCreateBitmapContext (int pixelsWide, int pixelsHigh)
{
    CGContextRef    context = NULL;
    CGColorSpaceRef colorSpace;
    void *          bitmapData;
    int             bitmapByteCount;
    int             bitmapBytesPerRow;
 
    bitmapBytesPerRow   = (pixelsWide * 4);// 1
    bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);
 
    colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);// 2
    bitmapData = calloc( bitmapByteCount, sizeof(uint8_t) );// 3
    if (bitmapData == NULL)
    {
        fprintf (stderr, "Memory not allocated!");
        return NULL;
    }
    context = CGBitmapContextCreate (bitmapData,// 4
                                    pixelsWide,
                                    pixelsHigh,
                                    8,      // bits per component
                                    bitmapBytesPerRow,
                                    colorSpace,
                                    kCGImageAlphaNoneSkipLast);
    if (context== NULL)
    {
        free (bitmapData);// 5
        fprintf (stderr, "Context not created!");
        return NULL;
    }
    CGColorSpaceRelease( colorSpace );// 6
 
    return context;// 7
}

//=================================================================================================

@implementation MyView
 
- (id)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    return self;
}
 
- (void)drawRect:(NSRect)rect
{

    CGContextRef myBitmapContext = MyCreateBitmapContext (800, 512);
    char *bitmapData = (char*)CGBitmapContextGetData(myBitmapContext);
    int count=0;
    for (int i=0;i<(512*800);i++) {
        for (int j=0; j<4; j++){
        bitmapData[count] = 32*[currentData getVal:i];
            count++;
        }
    }
    CGImageRef myImage2 = CGBitmapContextCreateImage (myBitmapContext);
    NSImage* myImage3 = [[NSImage alloc] initWithCGImage:myImage2 size:NSMakeSize(800,512)];
    [myImage3 drawAtPoint:NSMakePoint(0.0, 0.0)
                fromRect:NSMakeRect(0.0, 0.0, 800.0, 512.0)
               operation:NSCompositeSourceOver
                fraction:1.0];

}
 

-(void) updateFrame:(BeebFrame* )newFrame
{
    currentData = newFrame;
}

-(BOOL)acceptsFirstResponder
{
    NSLog(@"Accepting");
    return YES;
}

-(void) keyDown:(NSEvent *)event
{
    [self.delegate newKeyboardEvent:self theEvent:event];
}

-(void) keyUp:(NSEvent *)event
{
    [self.delegate newKeyboardEvent:self theEvent:event];
}

-(void) flagsChanged:(NSEvent *)event
{
     [self.delegate newKeyboardEvent:self theEvent:event];
}


@end
