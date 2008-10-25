//
//  FDProcessInfoReader.m
//  DaemonMaster
//
//  Created by Francis Devereux on 21/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDProcessInfoReader.h"

#import "SSYAuthorizedTaskmaster/SSYAuthorizedTaskmaster.h"
#import "SSYAuthorizedTaskmaster+DaemonMaster.h"

#import <sys/sysctl.h>

@implementation FDProcessInfoReader

#if 0
- (NSArray *)processes
{
    NSMutableArray *processes = [[[NSMutableArray alloc] init] autorelease];

    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
//     int what = KERN_PROC_ALL;
//     int flag = 0;
    struct kinfo_proc *kps;
    int nentries = -1;
    size_t bufSize = 5589;
    size_t orig_bufSize;
    int retry_count = 0;
    int local_error = 0;
//     mib[0] = CTL_KERN;
//     mib[1] = KERN_PROC;
//     mib[2] = what;
//     mib[3] = flag;

    if (sysctl(mib, 4, NULL, &bufSize, NULL, 0) < 0)
    {
        // FJDTODO error handling - throw exception?
        // FJDTODO is perror thread safe?
        perror("Failure calling sysctl");
        return nil;
    }
    NSLog(@"bufSize = %d", bufSize);

    kps = (struct kinfo_proc *)malloc(bufSize);
    if (kps == 0)
    {
        perror("malloc failed");
        return nil;
    }

    /* retry for transient errors due to load in the system */
    orig_bufSize = bufSize;
    for (retry_count = 0; ; retry_count++)
    {
        local_error = 0;
        bufSize = orig_bufSize;
        if ((local_error = sysctl(mib, 4, kps, &bufSize, NULL, 0)) < 0) {
            if (retry_count < 1) { // FJDTODO 1 is for debugging, change back to 1000
                /* 1 sec back off */
                sleep(1);
                continue;
            }
            perror("Failure calling sysctl");
            goto ERROR;
        } else if (local_error == 0) {
            break;
        }
    }
    
    /* This has to be after the second sysctl since the bufSize
     may have changed.  */
    nentries = bufSize / sizeof(struct kinfo_proc);

    int entry;
    NSLog(@"bufSize = %d, nentries = %d", bufSize, nentries);
    for (entry = 0; entry < nentries; entry++)
    {
        char *command = 0;
        int argvlen;
        int argv0len;
        getproclline(&kps[entry], &command, &argvlen, &argv0len, 1);
        [processes addObject:[NSString stringWithCString:command]];
        free(command);
    }

    return processes;

  ERROR:
    free(kps);
    return nil;
}
#endif

// FJDTODO contains code from ps.c - check license
- (IBAction)dumpProcesses:(id)sender
{
//    NSArray * processes = [self processes];
//    NSLog(@"processes returned");
//    NSLog(@"processes = %@", processes);
    NSError *error = nil;
    NSArray *processList = nil;
    BOOL ok = [[SSYAuthorizedTaskMaster sharedWorker] getProcessList:&processList 
                                                               error:&error];
    if (!ok)
    {
        // FJDTODO proper error handling
        NSLog(@"getProcessList error: %@", [error localizedDescription]);
    }
    else
    {
        NSLog(@"processList = %@", processList);
    }
}

@end
