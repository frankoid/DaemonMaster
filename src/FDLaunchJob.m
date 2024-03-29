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

// private: run a launchctl command that takes a configuration plist as an argumen
- (BOOL)doLaunchctlPlistCommand:(NSString *)command withFinishNotificationObserver:(id)notificationObserver selector:(SEL)notificationSelector
{
	if (plistPath)
	{
		NSArray *args = [[NSArray alloc] initWithObjects:command, plistPath, nil];
		NSTask *launchctlTask = [NSTask launchedTaskWithLaunchPath:@"/bin/launchctl" arguments:args];
		[args release];
		args = nil;
		
		if (notificationObserver)
		{
			[[NSNotificationCenter defaultCenter] addObserver:notificationObserver
													 selector:notificationSelector
														 name:NSTaskDidTerminateNotification 
													   object:launchctlTask];
		}
		return YES;
	}
	else
	{
		return NO;
	}
}

- (BOOL)load
{
	return [self loadWithFinishNotificationObserver:nil selector:nil];
}

- (BOOL)loadWithFinishNotificationObserver:(id)notificationObserver selector:(SEL)notificationSelector
{
    return [self doLaunchctlPlistCommand:@"load"
          withFinishNotificationObserver:notificationObserver
                                selector:notificationSelector];
}

- (BOOL)unload
{
	return [self unloadWithFinishNotificationObserver:nil selector:nil];
}

- (BOOL)unloadWithFinishNotificationObserver:(id)notificationObserver selector:(SEL)notificationSelector
{
    return [self doLaunchctlPlistCommand:@"unload"
          withFinishNotificationObserver:notificationObserver
                                selector:notificationSelector];
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
