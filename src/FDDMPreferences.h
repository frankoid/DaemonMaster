//
//  FDDMPreferences.h
//  DaemonMaster
//
//  Created by Francis Devereux on 07/12/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

// Loads and saves user preferences.  Other classes should use FDDMPrefences 
// and not call NSUserDefaults directly.
@interface FDDMPreferences : NSObject {

}

+ (void)initialize;
+ (FDDMPreferences *)sharedPreferences;
- (NSTimeInterval)processListRefreshInterval;
- (void)setProcessListRefreshInterval:(NSTimeInterval)interval;

@end
