//
//  FDLaunchJob.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDLaunchJob.h"
#import "FDPathUtils.h"

@implementation FDLaunchJob

static NSArray *launchJobLibrarySubdirs;

+ (void)initialize
{
	launchJobLibrarySubdirs = [[NSArray alloc] initWithObjects:@"LaunchAgents", @"LaunchDaemons", nil];
}

- (id)initWithLabel:(NSString *)theLabel andPid:(NSNumber *)thePid
{
    if (self = [super init])
    {
        label = [theLabel retain];
        pid = [thePid retain];
		plistPath = FDFileInPathSubdirectories(NSLibraryDirectory, NSAllDomainsMask,
											   launchJobLibrarySubdirs,
											   [NSString stringWithFormat:@"%@.plist", label]);
		[plistPath retain];
    }
    return self;
}

- (void)dealloc
{
    [label release];
    [pid release];
	[plistPath release];
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

- (NSString *)plistPath
{
	return plistPath;
}

@end
