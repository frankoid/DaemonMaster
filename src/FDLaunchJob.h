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
	NSNumber *pid;
	NSString *plistPath;
}

+ (void)initialize;

- (id)initWithLabel:(NSString *)theLabel andPid:(NSNumber *)thePid;

- (BOOL)load;
- (BOOL)loadWithFinishNotificationObserver:(id)notificationObserver selector:(SEL)notificationSelector;

- (BOOL)unload;
- (BOOL)unloadWithFinishNotificationObserver:(id)notificationObserver selector:(SEL)notificationSelector;

- (NSString *)label;
- (NSNumber *)pid;
- (NSString *)plistPath;
@end
