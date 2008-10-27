//
//  FDDMController.h
//  DaemonMaster - Main controller
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class FDAppListController;

@interface FDDMController : NSObject {
    FDAppListController *appListController;
    
	IBOutlet NSTableView *launchJobsView;
	IBOutlet NSProgressIndicator *refreshProgressIndicator;
	NSTimer *refreshTimer;
	NSMutableArray *launchJobs;
	
	BOOL runAsRoot;
}

- (IBAction)refresh:(id)sender;
- (IBAction)load:(id)sender;
- (IBAction)unload:(id)sender;

- (IBAction)showAppListWindow:(id)sender;

// Properties
- (BOOL)runAsRoot;
- (void)setRunAsRoot:(BOOL)newRunAsRoot;

// NSTableView data source methods
- (int)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)rowIndex;


@end
