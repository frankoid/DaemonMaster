//
//  FDLaunchJob.h
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <sys/types.h>

/*
 * Information about a launchd job (daemon or agent)
 */
@interface FDLaunchJob : NSObject {
	NSString *label;
	pid_t pid;
	int status;
}

- (id)initWithLabel:(NSString *)theLabel andStatus:(int)status andPid:(pid_t)thePid;

@end
