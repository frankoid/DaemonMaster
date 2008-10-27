//
//  FDProcessListController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 27/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDProcessListController.h"

#import "FDProcessInfoReader.h"

//-- Private interface --------------------------------------------------------
@interface FDProcessListController ()
- (NSArray *)processListAsDicts;
@end

//-- Implementation -----------------------------------------------------------
@implementation FDProcessListController

- (id)init
{
    if (self = [super initWithWindowNibName:@"ProcessList"])
    {
        processInfoReader = [[FDProcessInfoReader alloc] init];
    }
    return self;
}

- (void)windowDidLoad
{
    //NSLog(@"FDProcessListController windowDidLoad");
    [processListArrayController setContent:[self processListAsDicts]];
}

- (void)dealloc
{
    [processInfoReader release];
    [super dealloc];
}

- (NSArray *)processListAsDicts
{
    return [processInfoReader processList];
}

@end