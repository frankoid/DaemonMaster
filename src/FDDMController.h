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

@interface FDDMController : NSObject {
    FDAppListController *appListController;
    FDLaunchJobsController *launchJobsController;
}

- (IBAction)showAppListWindow:(id)sender;
- (IBAction)showLaunchJobsWindow:(id)sender;

@end
