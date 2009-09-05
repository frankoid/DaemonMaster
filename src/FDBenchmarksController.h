//
//  FDBenchmarksController.h
//  DaemonMaster
//
//  Created by Francis Devereux on 04/09/2009.
//  Copyright 2009 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class FDProcessInfoReader;

@interface FDBenchmarksController : NSWindowController {
    IBOutlet NSTextField *phtResultTextField;
    IBOutlet NSTextField *subprocessResultTextField;
    FDProcessInfoReader *processInfoReader;
}

- (IBAction)benchmark:(id)sender;

@end
