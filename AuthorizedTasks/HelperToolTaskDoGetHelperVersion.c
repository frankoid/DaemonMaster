/*
 *  HelperToolTaskDoGetHelperVersion.c
 *  AuthorizedTasks
 *
 *  Created by Francis Devereux on 28/10/2008.
 *  Copyright 2008 Francis Devereux. All rights reserved.
 *
 */

// Needed for any key constants used in the request/response dics:
#include "AuthorizedTasks_DaemonMaster.h"

// Needed for COMMAND_PROC_ARGUMENTS:
#include "CommandProcArguments.h"

// Provides some handy utilities for dealing with CF (CoreFoundation).
// It may not be used in all tasks but is included so you don't lose it.
#include "MoreCFQ.h"

OSStatus DoGetHelperVersion(COMMAND_PROC_ARGUMENTS) {
    OSStatus retval = noErr;
    CFNumberRef version = NULL;

    // Pre-conditions
    
    // userData may be NULL
    assert(request != NULL);
    assert(response != NULL);
    // asl may be NULL
    // aslMsg may be NULL

    version = CFNumberCreate(NULL, kCFNumberIntType, &kFDDMCurrentHelperVersion);
    if (version == NULL)
    {
        retval = coreFoundationUnknownErr;
        goto EXIT;
    }
    
    CFDictionaryAddValue(response, CFSTR(kFDDMHelperVersion), version);
    
EXIT:
    if (version != NULL)
    {
        CFRelease(version);
    }
    
    return retval;
}   
