//
//  FDBenchmarksController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 04/09/2009.
//  Copyright 2009 Francis Devereux. All rights reserved.
//

#import "FDBenchmarksController.h"

#import "FDProcessInfoReader.h"

#include <mach/mach_time.h>

//Raw mach_absolute_times going in, difference in seconds out
// from http://macresearch.org/tutorial_performance_and_time
static double subtractTimes( uint64_t endTime, uint64_t startTime )
{
    uint64_t difference = endTime - startTime;
    static double conversion = 0.0;
    
    if( conversion == 0.0 )
    {
        mach_timebase_info_data_t info;
        kern_return_t err = mach_timebase_info( &info );
        
	//Convert the timebase into seconds
        if( err == 0  )
		conversion = 1e-9 * (double) info.numer / (double) info.denom;
    }
    
    return conversion * (double) difference;
}

@implementation FDBenchmarksController

- (id)init
{
    if (self = [super initWithWindowNibName:@"Benchmarks"])
    {
        processInfoReader = [[FDProcessInfoReader alloc] init];
    }
    return self;
}

- (void)dealloc
{
    [processInfoReader release];
    [super dealloc];
}

static const int ITERATIONS = 1000;

- (double)benchmarkPHT
{
    int i;
    
    uint64_t start = mach_absolute_time();
    for (i = 0; i < ITERATIONS; i++)
    {
        [processInfoReader processList];
    }
    uint64_t stop = mach_absolute_time();
    
    return subtractTimes(stop, start);
}

- (double)benchmarkSubprocess
{
    int i;
    
    uint64_t start = mach_absolute_time();
    NSFileHandle *fhDevNull = [NSFileHandle fileHandleWithNullDevice];
    for (i = 0; i < ITERATIONS; i++)
    {
        NSArray *args = [[NSArray alloc] initWithObjects:@"-ax", @"-o", @"command,args", nil];
        NSTask *psTask = [[NSTask alloc] init];
        [psTask setLaunchPath:@"/bin/ps"];
        [psTask setArguments:args];
        [psTask setStandardOutput:fhDevNull];
        [psTask launch];
        [psTask waitUntilExit];
        [args release];
        [psTask release];
            
    }
    uint64_t stop = mach_absolute_time();
    
    return subtractTimes(stop, start);
}

- (IBAction)benchmark:(id)sender
{
    double pht = [self benchmarkPHT];
    [phtResultTextField setDoubleValue:pht];

    double subprocess = [self benchmarkSubprocess];
    [subprocessResultTextField setDoubleValue:subprocess];
}

@end
