//
//  FDDMController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDDMController.h"
#import "FDLaunchctlWrapper.h"
#import "FDLaunchJob.h"
#import "FDAppListController.h"

@implementation FDDMController

- (void)dealloc
{
	[refreshTimer invalidate];
	[launchJobs release];
	[super dealloc];
}

- (void)awakeFromNib
{
	//refreshTimer = [NSTimer scheduledTimerWithTimeInterval:5.0 target:self selector:@selector(refresh:) userInfo:self repeats:YES];
//	[refreshTimer retain];
	NSLog(@"In FDDMController awakeFromNib");
	runAsRoot = NO;
}

//-- Private Methods ----------------------------------------------------------
- (NSMutableArray *)launchJobs
{
	if (launchJobs == nil)
	{
		//NSLog(@"Refreshing launchJobs");
		launchJobs = [[[FDLaunchctlWrapper sharedLaunchctlWrapper] list] retain];
		[launchJobs sortUsingDescriptors:[launchJobsView sortDescriptors]];
	}
	return launchJobs;
}

- (NSArray *)selectedLaunchJobs
{
    return [launchJobs objectsAtIndexes:[launchJobsView selectedRowIndexes]];
    
}

//-- Actions ------------------------------------------------------------------

- (IBAction)refresh:(id)sender
{
	[refreshProgressIndicator startAnimation:self];
	[launchJobs release];
	launchJobs = nil;
	// I thought that the next line would be necessary necessary to keep the
	// progress indicator spinning whilst the job information is being
	// gathered, but it doesn't seem to be...
//	[self launchJobs];
	[launchJobsView reloadData];
	[refreshProgressIndicator stopAnimation:self];
}

- (void)taskDidFinish:(NSNotification *)notification
{
	NSTask *finishedTask = [notification object];
    NSLog(@"task \"%@\" finished with status: %d. args = ", [finishedTask launchPath], [finishedTask terminationStatus], [finishedTask arguments]);
}

- (IBAction)load:(id)sender
{
    for (FDLaunchJob *launchJob in [self selectedLaunchJobs])
    {
        [launchJob loadWithFinishNotificationObserver:self selector:@selector(taskDidFinish:)];
    }
}

- (IBAction)unload:(id)sender
{
    for (FDLaunchJob *launchJob in [self selectedLaunchJobs])
    {
        [launchJob unloadWithFinishNotificationObserver:self selector:@selector(taskDidFinish:)];
    }
}

- (IBAction)showAppListWindow:(id)sender
{
    if (!appListController)
    {
        appListController = [[FDAppListController alloc] init];
    }
    [appListController showWindow:self];
}

//-----------------------------------------------------------------------------

- (BOOL)runAsRoot
{
	return runAsRoot;
}

- (void)setRunAsRoot:(BOOL)newRunAsRoot
{
	runAsRoot = newRunAsRoot;
}

//-- NSTableView data source methods --
- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
	return [[self launchJobs] count];
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)rowIndex
{
	id identifier = [tableColumn identifier];
	NSArray *localLaunchJobs = [self launchJobs];
	if (localLaunchJobs == nil)
	{
		NSLog(@"localLaunchJobs is nil!");
		return nil;
	}
	else if ([localLaunchJobs count] > rowIndex)
	{
		FDLaunchJob *job = [localLaunchJobs objectAtIndex:rowIndex];
		id value = [job valueForKey:identifier];
//		NSLog(@"Value for row %d and column %@ is %@ (isa %@)", rowIndex, identifier, value, [value class]);
		return value;
	}
	else
	{
		NSLog(@"Row %d out of range (array has %d elements)", rowIndex, [localLaunchJobs count]);
		return nil;
	}
}

- (void)tableView:(NSTableView *)tableView sortDescriptorsDidChange:(NSArray *)oldDescriptors
{
    NSArray *newDescriptors = [tableView sortDescriptors];
    [[self launchJobs] sortUsingDescriptors:newDescriptors];
    [tableView reloadData];
}

//-- NSWindow delegate methods --
// This controller is the delegate of the main DaemonMaster window

- (void)windowWillClose:(NSNotification *)notification
{
	[[NSApplication sharedApplication] terminate:self];
}

@end
