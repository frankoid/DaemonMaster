//
//  FDDMController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDDMController.h"
#import "FDLaunchctlWrapper.h"

@implementation FDDMController

- (IBAction)refresh:(id)sender
{
	FDLaunchctlWrapper *launchctl = [[FDLaunchctlWrapper alloc] init];
	[launchctl list];
}

@end
