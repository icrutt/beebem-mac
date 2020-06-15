//
//  BeebEmLog.cpp
//  BeebEm4
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#include "BeebEmLog.hpp"
#include <cstdarg>

// Data members
FILE* BeebEmLog::tlog=NULL;


// Methods
void BeebEmLog::writeLog(const char *fmt, ...)
{
char buff[512];
    
    va_list argptr;
    
    va_start(argptr, fmt);
    vsprintf(buff, fmt, argptr);
    va_end(argptr);

    if (BeebEmLog::tlog) fprintf(BeebEmLog::tlog, "%s", buff);
    fprintf(stderr, "%s", buff);
}

void BeebEmLog::closeLog()
{
    if (tlog) fclose(tlog);
}
