/*
 *  TestHarnessGetProcessList.c
 *  AuthorizedTasks
 *
 *  Created by Francis Devereux on 28/10/2008.
 *  Copyright 2008 Francis Devereux. All rights reserved.
 *
 */
#include "BetterAuthorizationSampleLib.h"
#include "AuthorizedTasks_DaemonMaster.h"
#include "MoreCFQ.h"

extern const BASCommandProc kCommandProcs[] ;
extern const BASCommandSpec kCommandSpecs[] ;


int main(int argc, char *argv[])
{
    aslclient                   asl = NULL;
    aslmsg                      aslMsg = NULL;
    asl     = asl_open(NULL, "HelperTools", ASL_OPT_STDERR);
    assert(asl != NULL);
    
    aslMsg = asl_new(ASL_TYPE_MSG);
    assert(aslMsg != NULL);

    CFMutableDictionaryRef      response    = NULL;
    response = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    assert(response);
   
    CFDictionaryRef             request     = NULL;
    request = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    assert(request);

    DoGetProcessList(NULL, NULL, request, response, asl, aslMsg);
    
    return 0;
}

