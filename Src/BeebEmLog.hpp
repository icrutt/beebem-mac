//
//  BeebEmLog.hpp
//  BeebEm4
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BeebEmLog_hpp
#define BeebEmLog_hpp

#include <stdio.h>

class BeebEmLog {
private:
    static FILE *tlog;
public:
    static void writeLog(const char *fmt, ...);
    static void closeLog();
};

#endif /* BeebEmLog_hpp */
