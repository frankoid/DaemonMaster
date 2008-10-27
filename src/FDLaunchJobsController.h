//
//  FDLaunchJobsController.h
//  DaemonMaster
//
//  Created by Francis Devereux on 27/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FDLaunchJobsController : NSWindowController {
    IBOutlet NSTableView *launchJobsView;
    IBOutlet NSProgressIndicator *refreshProgressIndicator;
    NSTimer *refreshTimer;
    NSMutableArray *launchJobs;
    
    BOOL runAsRoot;
}

- (id)init;

// - (void)windowDidLoad;

- (IBAction)refresh:(id)sender;
- (IBAction)load:(id)sender;
- (IBAction)unload:(id)sender;

// Properties
- (BOOL)runAsRoot;
- (void)setRunAsRoot:(BOOL)newRunAsRoot;

// NSTableView data source methods
- (int)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)rowIndex;

@end
