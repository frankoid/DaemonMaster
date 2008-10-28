/* Copyright notice for code copied from adv_cmds-119/ps.tproj: */
/*-
 * Copyright (c) 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 * Copyright (c) 2008 Francis Devereux
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


// Needed for any key constants used in the request/response dics:
#include "AuthorizedTasks_DaemonMaster.h"

// Needed for COMMAND_PROC_ARGUMENTS:
#include "CommandProcArguments.h"

// Provides some handy utilities for dealing with CF (CoreFoundation).
// It may not be used in all tasks but is included so you don't lose it.
#include "MoreCFQ.h"

// Needed for sysctl()
#include <sys/sysctl.h>

// Needed for sleep()
#include <unistd.h>

static aslclient sAsl = NULL;
static aslmsg sAslMsg = NULL;

/*
 * Get a process's argv as a CFArray.
 *
 * argv is implicitly retained by the caller (i.e. the caller must CFRelease
 * it).
 *
 * Based on getproclline from ps.tproj/print.c
 */
static OSStatus
CreateArgArrayForProcess(const struct kinfo_proc *kp, CFMutableArrayRef *argsp)
{
    OSStatus retval = noErr;
    int      mib[3], argmax, nargs, c = 0;
    size_t   size;
    char     *procargs, *np, *cp;

    /* Get the maximum process arguments size. */
    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;

    size = sizeof(argmax);
    if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1) {
        asl_log(sAsl, sAslMsg, ASL_LEVEL_ERR, "KERN_ARGMAX sysctl failed: %m");
        goto ERROR_A_COMM;
    }

    /* Allocate space for the arguments. */
    procargs = (char *)malloc(argmax);
    if (procargs == NULL) {
        goto ERROR_A_COMM;
    }

    /*
     * Make a sysctl() call to get the raw argument space of the process.
     * The layout is documented in start.s, which is part of the Csu
     * project.  In summary, it looks like:
     *
     * /---------------\ 0x00000000
     * :               :
     * :               :
     * |---------------|
     * | argc          |
     * |---------------|
     * | arg[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | arg[argc - 1] |
     * |---------------|
     * | 0             |
     * |---------------|
     * | env[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | env[n]        |
     * |---------------|
     * | 0             |
     * |---------------| <-- Beginning of data returned by sysctl() is here.
     * | argc          |
     * |---------------|
     * | exec_path     |
     * |:::::::::::::::|
     * |               |
     * | String area.  |
     * |               |
     * |---------------| <-- Top of stack.
     * :               :
     * :               :
     * \---------------/ 0xffffffff
     */
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROCARGS2;
    mib[2] = kp->kp_proc.p_pid;
    
    size = (size_t)argmax;
    if (sysctl(mib, 3, procargs, &size, NULL, 0) == -1) {
        goto ERROR_B_COMM;
    }

    memcpy(&nargs, procargs, sizeof(nargs));
    cp = procargs + sizeof(nargs);

    /* Skip the saved exec_path. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            /* End of exec_path reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        goto ERROR_B_COMM;
    }

    /* Skip trailing '\0' characters. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp != '\0') {
            /* Beginning of first argument reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        goto ERROR_B_COMM;
    }

    if ((retval = CFQArrayCreateMutable(argsp)) != noErr) goto ERROR_B;

    /*
     * Iterate through the '\0'-terminated strings until desired number of
     * args is reached or the end of procargs is reached.
     */
    for (np = cp - 1; c < nargs && cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            CFStringRef arg;
            c++;
            /* Get previous arg */
            arg = CFStringCreateWithCString(NULL, np + 1, kCFStringEncodingUTF8);
            if (!arg)
            {
                retval = coreFoundationUnknownErr;
                goto ERROR_C_COMM;
            }
            CFArrayAppendValue(*argsp, arg);
            CFRelease(arg);

            /* Note location of current '\0'. */
            np = cp;
        }
    }

  ERROR_B:
    free(procargs);
//  ERROR_A:
    return retval;

  ERROR_C_COMM:
    CFRelease(*argsp);
    *argsp = NULL;
  ERROR_B_COMM:
    free(procargs);
  ERROR_A_COMM:
    /* Getting all the args failed. Fall back to getting just the command
     * from the kerninfo_proc. */
    retval = noErr;
    if ((retval = CFQArrayCreateMutable(argsp)) != noErr) goto ERROR_X;
    CFStringRef command = CFStringCreateWithFormat(NULL, NULL, CFSTR("(%s)"), kp->kp_proc.p_comm);
    CFArrayAppendValue(*argsp, command);
    CFRelease(command);
    
  ERROR_X:
/*     if (retval) asl_log(sAsl, sAslMsg, ASL_LEVEL_ERR, "Error in CreateArgArrayForProcess %d: %m", retval); */
    return retval;
}



OSStatus DoGetProcessList(COMMAND_PROC_ARGUMENTS) {
    OSStatus retval = noErr;

    CFMutableArrayRef processes = NULL;
    CFMutableDictionaryRef processInfoDict = NULL;
    CFNumberRef cfPid = NULL;
    CFStringRef command = NULL;
    CFMutableArrayRef args = NULL;

    // Pre-conditions

    // userData may be NULL
    assert(request != NULL);
    assert(response != NULL);
    // asl may be NULL
    // aslMsg may be NULL

    sAsl = asl;
    sAslMsg = aslMsg;

    // Get the process list
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    struct kinfo_proc *kps = NULL;
    int nentries = -1;
    size_t bufSize = 0;
    size_t orig_bufSize;
    int retry_count = 0;
    int local_error = 0;

    if (sysctl(mib, 4, NULL, &bufSize, NULL, 0) < 0)
    {
        asl_log(asl, aslMsg, ASL_LEVEL_ERR, "KERN_PROC sysctl failed: %m");
        retval = coreFoundationUnknownErr;
        goto EXIT;
    }

    kps = (struct kinfo_proc *)malloc(bufSize);
    if (kps == 0)
    {
        asl_log(asl, aslMsg, ASL_LEVEL_ERR, "malloc failed: %m");
        retval = coreFoundationUnknownErr;
        goto EXIT;
    }

    /* retry for transient errors due to load in the system */
    orig_bufSize = bufSize;
    for (retry_count = 0; ; retry_count++)
    {
        local_error = 0;
        bufSize = orig_bufSize;
        if ((local_error = sysctl(mib, 4, kps, &bufSize, NULL, 0)) < 0) {
            if (retry_count < 10) {
                /* 1 sec back off */
                sleep(1);
                continue;
            }
            asl_log(asl, aslMsg, ASL_LEVEL_ERR, "KERN_PROC sysctl failed despite retries: %m");
            retval = coreFoundationUnknownErr;
            goto EXIT;
        } else if (local_error == 0) {
            break;
        }
    }

    /* This has to be after the second sysctl since the bufSize
     may have changed.  */
    nentries = bufSize / sizeof(struct kinfo_proc);


    if ((retval = CFQArrayCreateMutable(&processes)) != noErr) goto EXIT;

    int entry;
    for (entry = 0; entry < nentries; entry++)
    {
        if ((retval = CFQDictionaryCreateMutable(&processInfoDict)) != noErr) goto EXIT;

        // add the PID to processInfoDict
        cfPid = CFNumberCreate(NULL, kCFNumberIntType, &(kps[entry].kp_proc.p_pid));
        if (!cfPid)
        {
            retval = coreFoundationUnknownErr;
            goto EXIT;
        }
        CFDictionaryAddValue(processInfoDict, CFSTR(kFDDMPID), cfPid);
        CFRelease(cfPid);
        cfPid = NULL;

        // add the command name to processInfoDict
        command = CFStringCreateWithCString(NULL, kps[entry].kp_proc.p_comm, kCFStringEncodingUTF8);
        CFDictionaryAddValue(processInfoDict, CFSTR(kFDDMCommand), command);
        CFRelease(command);
        command = NULL;
        
        // add the args to processInfoDict
        if (CreateArgArrayForProcess(&kps[entry], &args)) goto EXIT;
        CFDictionaryAddValue(processInfoDict, CFSTR(kFDDMArgs), args);
        CFRelease(args);
        args = NULL;

        // add processInfoDict to the processes array
        CFArrayAppendValue(processes, processInfoDict);

        CFRelease(processInfoDict);
        processInfoDict = NULL;
    }

    CFDictionaryAddValue(response, CFSTR(kFDDMProcesses), processes);

EXIT:
    if (kps != NULL)
    {
        free(kps);
    }
    if (processes != NULL)
    {
        CFRelease(processes);
    }
    if (processInfoDict != NULL)
    {
        CFRelease(processInfoDict);
    }
    if (cfPid != NULL)
    {
        CFRelease(cfPid);
    }
    if (command != NULL)
    {
        CFRelease(command);
    }
    if (args != NULL)
    {
        CFRelease(args);
    }

    if (retval)
    {
        asl_log(asl, aslMsg, ASL_LEVEL_ERR, "DoGetProcessList returning error %d: %m", retval);
    }
    return retval;
}
