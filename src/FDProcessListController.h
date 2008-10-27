//
//  FDProcessListController.h
//  DaemonMaster
//
//  Created by Francis Devereux on 27/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class FDProcessInfoReader;

@interface FDProcessListController : NSWindowController {
    IBOutlet NSArrayController *processListArrayController;

    FDProcessInfoReader *processInfoReader;
}

- (id)init;
- (void)windowDidLoad;
- (void)dealloc;

@end
