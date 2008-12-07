//
//  FDProcessListController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 27/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDProcessListController.h"

#import "FDProcessInfoReader.h"
#import "FDDMPreferences.h"

//-- Private interface --------------------------------------------------------
@interface FDProcessListController ()
- (NSArray *)processListAsDicts;
- (void)scheduleRefreshTimer;
@end

//-- Implementation -----------------------------------------------------------
@implementation FDProcessListController

- (id)init
{
//    NSLog(@"FDProcessListController init");
    if (self = [super initWithWindowNibName:@"ProcessList"])
    {
        processInfoReader = [[FDProcessInfoReader alloc] init];
        refreshCount = 0;
    }
    return self;
}

- (void)windowDidLoad
{
//    NSLog(@"FDProcessListController windowDidLoad");
    [self refresh:self];
    NSTimeInterval refreshInterval = [[FDDMPreferences sharedPreferences] processListRefreshInterval];

    [refreshIntervalTextField setDoubleValue:refreshInterval];
    [refreshIntervalStepper setDoubleValue:refreshInterval];
    [self scheduleRefreshTimer];
}

- (void)dealloc
{
    [processInfoReader release];
    [refreshTimer invalidate];
    [super dealloc];
}

- (NSArray *)processListAsDicts
{
    return [processInfoReader processList];
}

- (void)scheduleRefreshTimer
{
    [refreshTimer invalidate];
    refreshTimer = [NSTimer scheduledTimerWithTimeInterval:[refreshIntervalTextField doubleValue] target:self selector:@selector(refresh:) userInfo:self repeats:YES];
}

//-- Actions ------------------------------------------------------------------

- (IBAction)refresh:(id)sender
{
    [refreshProgressIndicator startAnimation:self];
    [processListArrayController setContent:[self processListAsDicts]];
    [refreshProgressIndicator stopAnimation:self];
    [refreshCountLabel setIntValue:(++refreshCount)];
}

- (IBAction)takeRefreshIntervalFrom:(id)sender
{
    NSTimeInterval newInterval = [sender doubleValue];
    NSLog(@"takeRefreshIntervalFrom: new interval is %f", newInterval);
    [refreshIntervalTextField setDoubleValue:newInterval];
    [refreshIntervalStepper setDoubleValue:newInterval];
    [self scheduleRefreshTimer];
    [[FDDMPreferences sharedPreferences] setProcessListRefreshInterval:newInterval];
}

@end
