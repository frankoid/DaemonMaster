//
//  FDDMController.h
//  DaemonMaster - Main controller
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class FDAppListController;
@class FDLaunchJobsController;
@class FDProcessListController;

@interface FDDMController : NSObject {
	FDProcessListController *processListController;
    FDAppListController *appListController;
    FDLaunchJobsController *launchJobsController;
}

- (void)awakeFromNib;

- (IBAction)showProcessListWindow:(id)sender;
- (IBAction)showAppListWindow:(id)sender;
- (IBAction)showLaunchJobsWindow:(id)sender;

@end
