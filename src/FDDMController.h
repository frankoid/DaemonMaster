//
//  FDDMController.h
//  DaemonMaster - Main controller
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FDDMController : NSObject {
	IBOutlet NSTableView *launchJobsView;
	IBOutlet NSProgressIndicator *refreshProgressIndicator;
	NSTimer *refreshTimer;
	NSMutableArray *launchJobs;
}

- (IBAction)refresh:(id)sender;

// NSTableView data source methods
- (int)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)rowIndex;



@end
