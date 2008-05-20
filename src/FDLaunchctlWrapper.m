//
//  FDLaunchctlWrapper.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDLaunchctlWrapper.h"
#import "FDLaunchJob.h"
#import "FDExceptionNames.h"

#import <stdio.h>

#define PIPE_BUF_SIZE 4096

static FDLaunchctlWrapper *sharedLaunchctlWrapper;

@implementation FDLaunchctlWrapper

+ (FDLaunchctlWrapper *)sharedLaunchctlWrapper
{
	if (sharedLaunchctlWrapper == nil)
	{
		sharedLaunchctlWrapper = [[FDLaunchctlWrapper alloc] init];
	}
	return sharedLaunchctlWrapper;
}

- (id)init
{
	if (self = [super init])
	{
		integerFormatter = [[NSNumberFormatter alloc] init];
		[integerFormatter setFormat:@"#"];
	}
	return self;
}

- (void) dealloc
{
	[integerFormatter release];
	[super dealloc];
}


- (NSArray *)list
{
	void *buf = malloc(PIPE_BUF_SIZE);
	if (buf == 0) [NSException raise:FDOutOfMemoryException format:FDOutOfMemoryExceptionMessage];
	
	NSMutableArray *list = [[[NSMutableArray alloc] init] autorelease];
	
	//FJDNOTE: could use an NSTask here instead of popen (I didn't know about NSTask when I wrote this code)
	FILE *launchctlPipe = popen("/bin/launchctl list", "r");
//	FILE *launchctlPipe = popen("sleep 3", "r");
    NSFileHandle *launchctlFH = [[NSFileHandle alloc] initWithFileDescriptor:fileno(launchctlPipe)];

    NSData *launchctlOutput = [launchctlFH readDataToEndOfFile];
	NSString *sLaunchctlOutput = [[NSString alloc] initWithData:launchctlOutput encoding:NSUTF8StringEncoding]; //FJDTODO encoding 
//    NSLog(@"launchctlOutput = %@", sLaunchctlOutput);
    [launchctlFH release];
	pclose(launchctlPipe);
	
	NSArray *lines = [sLaunchctlOutput componentsSeparatedByString:@"\n"];
	[sLaunchctlOutput release];
	NSEnumerator *eLines = [lines objectEnumerator];
	// skip the first line which is a header
	[eLines nextObject];
	NSCharacterSet *whiteSpace = [NSCharacterSet characterSetWithCharactersInString:@"\t "];
	for (NSString *line in eLines)
	{
		NSArray *components = [line componentsSeparatedByCharactersInSet:whiteSpace];
		if ([components count] < 3)
		{
			NSLog(@"not enough components in line %@", line);
		}
		else
		{
			NSString *sPid = [components objectAtIndex:0];
			NSNumber *pid = [sPid isEqualToString:@"-"] ? nil : [integerFormatter numberFromString:sPid];

//            NSString *sStatus = [components objectAtIndex:1];
//            NSNumber *status = [sStatus isEqualToString:@"-"] ? nil : [integerFormatter numberFromString:sStatus];

            NSString *label = [components objectAtIndex:2];

//            NSLog(@"| %@ | %@ | %@ |", pid, status, label);
			FDLaunchJob *job = [[FDLaunchJob alloc] initWithLabel:label andPid:pid];
			[list addObject:job];
			[job release];
		}
	}
	
	return list;
}

@end
