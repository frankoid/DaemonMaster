//
//  FDDMController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDDMController.h"
#import "FDAppListController.h"
#import "FDLaunchJobsController.h"

@implementation FDDMController

//-- Actions ------------------------------------------------------------------

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

@end
