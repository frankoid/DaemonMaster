//
//  FDDMController.h
//  DaemonMaster - Main controller
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class FDAppListController;
@class FDProcessListController;
@class FDLaunchJobsController;
@class FDBenchmarksController;

@interface FDDMController : NSObject {
    FDProcessListController *processListController;
    FDAppListController *appListController;
    FDLaunchJobsController *launchJobsController;
    FDBenchmarksController *benchmarksController;
}

- (void)awakeFromNib;

- (IBAction)showProcessListWindow:(id)sender;
- (IBAction)showAppListWindow:(id)sender;
- (IBAction)showLaunchJobsWindow:(id)sender;
- (IBAction)showBenchmarksWindow:(id)sender;

@end
