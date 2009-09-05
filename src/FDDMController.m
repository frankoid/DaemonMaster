//
//  FDDMController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDDMController.h"

#import "FDProcessListController.h"
#import "FDAppListController.h"
#import "FDLaunchJobsController.h"
#import "FDBenchmarksController.h"

@implementation FDDMController

//-- Methods ------------------------------------------------------------------
- (void)awakeFromNib
{
    [self showProcessListWindow:self];
}

//-- Actions ------------------------------------------------------------------

- (IBAction)showProcessListWindow:(id)sender
{
    if (!processListController)
    {
        processListController = [[FDProcessListController alloc] init];
    }
    [processListController showWindow:self];
}

- (IBAction)showAppListWindow:(id)sender
{
    if (!appListController)
    {
        appListController = [[FDAppListController alloc] init];
    }
    [appListController showWindow:self];
}

- (IBAction)showLaunchJobsWindow:(id)sender
{
    if (!launchJobsController)
    {
        launchJobsController = [[FDLaunchJobsController alloc] init];
    }
    [launchJobsController showWindow:self];
}

- (IBAction)showBenchmarksWindow:(id)sender;
{
    if (!benchmarksController)
    {
        benchmarksController = [[FDBenchmarksController alloc] init];
    }
    [benchmarksController showWindow:self];
}


@end
