//
//  FDLaunchctlWrapper.h
//  DaemonMaster
//
//  Created by Francis Devereux on 28/04/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

// Wrapper for the launchctl command
@interface FDLaunchctlWrapper : NSObject {
	NSNumberFormatter *integerFormatter;
}

+ (FDLaunchctlWrapper *)sharedLaunchctlWrapper;

- (id)init;
- (void)dealloc;
- (NSArray *)list;

@end
