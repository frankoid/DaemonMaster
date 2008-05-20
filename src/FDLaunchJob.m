//
//  FDLaunchJob.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDLaunchJob.h"


@implementation FDLaunchJob

- (id)initWithLabel:(NSString *)theLabel andPid:(NSNumber *)thePid
{
    if (self = [super init])
    {
        label = [theLabel retain];
        pid = [thePid retain];
    }
    return self;
}

- (void)dealloc
{
    [label release];
    [pid release];
    [super dealloc];
}

- (NSString *)description
{
	return [NSString stringWithFormat:@"%s %@", label, pid];
}

- (NSString *)label
{
	return label;
}

- (NSNumber *)pid
{
	return pid;
}

@end
