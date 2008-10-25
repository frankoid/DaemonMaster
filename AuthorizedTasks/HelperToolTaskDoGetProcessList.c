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

/*
 * Get command and arguments.
 *
 * on return argvlen is the length of the extracted string, argv0len is
 * the length of the command (same as argvlen if show_args is true)
 *
 * Based on code from ps.tproj/print.c
 */
static void
getproclline(const struct kinfo_proc *kp, char **command_name, int *argvlen, int *argv0len,
  int show_args)
{
    int     mib[3], argmax, nargs, c = 0;
    size_t      size;
    char        *procargs, *sp, *np, *cp;
//    extern int  eflg;

    /* Get the maximum process arguments size. */
    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;

    size = sizeof(argmax);
    if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1) {
        goto ERROR_A;
    }

    /* Allocate space for the arguments. */
    procargs = (char *)malloc(argmax);
    if (procargs == NULL) {
        goto ERROR_A;
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
        goto ERROR_B;
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
        goto ERROR_B;
    }

    /* Skip trailing '\0' characters. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp != '\0') {
            /* Beginning of first argument reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        goto ERROR_B;
    }
    /* Save where the argv[0] string starts. */
    sp = cp;

    /*
     * Iterate through the '\0'-terminated strings and convert '\0' to ' '
     * until a string is found that has a '=' character in it (or there are
     * no more strings in procargs).  There is no way to deterministically
     * know where the command arguments end and the environment strings
     * start, which is why the '=' character is searched for as a heuristic.
     */
    for (np = NULL; c < nargs && cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            c++;
            if (np != NULL) {
                /* Convert previous '\0'. */
                *np = ' ';
            } else {
                *argv0len = cp - sp;
            }
            /* Note location of current '\0'. */
            np = cp;

            if (!show_args) {
                /*
                 * Don't convert '\0' characters to ' '.
                 * However, we needed to know that the
                 * command name was terminated, which we
                 * now know.
                 */
                break;
            }
        }
    }

#if 0
    /*
     * If eflg is non-zero, continue converting '\0' characters to ' '
     * characters until no more strings that look like environment settings
     * follow.
     */
    if ( show_args && (eflg != 0) && ( (getuid() == 0) || (KI_EPROC(k)->e_pcred.p_ruid == getuid()) ) ) {
        for (; cp < &procargs[size]; cp++) {
            if (*cp == '\0') {
                if (np != NULL) {
                    if (&np[1] == cp) {
                        /*
                         * Two '\0' characters in a row.
                         * This should normally only
                         * happen after all the strings
                         * have been seen, but in any
                         * case, stop parsing.
                         */
                        break;
                    }
                    /* Convert previous '\0'. */
                    *np = ' ';
                }
                /* Note location of current '\0'. */
                np = cp;
            }
        }
    }
#endif
        
    /*
     * sp points to the beginning of the arguments/environment string, and
     * np should point to the '\0' terminator for the string.
     */
    if (np == NULL || np == sp) {
        /* Empty or unterminated string. */
        goto ERROR_B;
    }

    /* Make a copy of the string. */
    *argvlen = asprintf(command_name, "%s", sp);

    /* Clean up. */
    free(procargs);
    return;

    ERROR_B:
    free(procargs);
    ERROR_A:
    *argv0len = *argvlen 
      = asprintf(command_name, "(%s)", kp->kp_proc.p_comm);
}



OSStatus DoGetProcessList(COMMAND_PROC_ARGUMENTS) {
    OSStatus retval = noErr;

    CFMutableArrayRef processes = NULL;
    CFMutableDictionaryRef processInfoDict = NULL;
    CFNumberRef cfPid = NULL;
    CFStringRef cfCommand = NULL;

    // Pre-conditions
    
    // userData may be NULL
    assert(request != NULL);
    assert(response != NULL);
    // asl may be NULL
    // aslMsg may be NULL

    // Get the process list
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
//     int what = KERN_PROC_ALL;
//     int flag = 0;
    struct kinfo_proc *kps = NULL;
    int nentries = -1;
    size_t bufSize = 0;
    size_t orig_bufSize;
    int retry_count = 0;
    int local_error = 0;
//     mib[0] = CTL_KERN;
//     mib[1] = KERN_PROC;
//     mib[2] = what;
//     mib[3] = flag;

    char *command = NULL;

    if (sysctl(mib, 4, NULL, &bufSize, NULL, 0) < 0)
    {
        perror("Failure calling sysctl");
        retval = coreFoundationUnknownErr;
        goto EXIT;
    }

    kps = (struct kinfo_proc *)malloc(bufSize);
    if (kps == 0)
    {
        perror("malloc failed");
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
            if (retry_count < 1000) {
                /* 1 sec back off */
                sleep(1);
                continue;
            }
            perror("Failure calling sysctl");
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
/*     NSLog(@"bufSize = %d, nentries = %d", bufSize, nentries); */
    for (entry = 0; entry < nentries; entry++)
    {
        int argvlen;
        int argv0len;
        getproclline(&kps[entry], &command, &argvlen, &argv0len, 1);

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
    
        // add the command to processInfoDict
        cfCommand = CFStringCreateWithCString(NULL, command, kCFStringEncodingUTF8);
        if (!cfCommand)
        {
            retval = coreFoundationUnknownErr;
            goto EXIT;
        }
        CFDictionaryAddValue(processInfoDict, CFSTR(kFDDMCommand), cfCommand);
        CFRelease(cfCommand);
        cfCommand = NULL;
    
        // add processInfoDict to the processes array
        CFArrayAppendValue(processes, processInfoDict);
    
        CFRelease(processInfoDict);
        processInfoDict = NULL;

        free(command);
        command = NULL;
    }
    
        
    // Add the data to the response

    // for each process {
    
    // } end for each process
    
    CFDictionaryAddValue(response, CFSTR(kFDDMProcesses), processes);
    
EXIT:
    if (kps != NULL)
    {
        free(kps);
    }
    if (command != NULL)
    {
        free(command);
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
    if (cfCommand != NULL)
    {
        CFRelease(cfCommand);
    }
    
    return retval;
}   
