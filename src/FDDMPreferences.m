//
//  FDDMPreferences.m
//  DaemonMaster
//
//  Created by Francis Devereux on 07/12/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDDMPreferences.h"

NSString * const FDProcessListRefreshIntervalKey = @"FDProcessListRefreshInterval";

static FDDMPreferences *sharedPreferences;

@implementation FDDMPreferences

+ (void)initialize
{
    // Set up and register the default user preferences
    NSMutableDictionary *defaultValues = [NSMutableDictionary dictionary];
    
    NSData *intervalAsData = [NSKeyedArchiver archivedDataWithRootObject:
                              [NSNumber numberWithDouble:3.0]];
    [defaultValues setObject:intervalAsData forKey:FDProcessListRefreshIntervalKey];
    
    [[NSUserDefaults standardUserDefaults] registerDefaults: defaultValues];
    
    // Create shared instance
    sharedPreferences = [[FDDMPreferences alloc] init];
}

+ (FDDMPreferences *)sharedPreferences
{
    return sharedPreferences;
}

- (NSTimeInterval)processListRefreshInterval
{
    NSData *intervalAsData = [[NSUserDefaults standardUserDefaults] 
                              objectForKey:FDProcessListRefreshIntervalKey];
    NSNumber *intervalAsNumber = [NSKeyedUnarchiver unarchiveObjectWithData:intervalAsData];
    return [intervalAsNumber doubleValue];
}

- (void)setProcessListRefreshInterval:(NSTimeInterval)interval
{
    NSNumber *intervalAsNumber = [NSNumber numberWithDouble:interval];
    NSData *intervalAsData = [NSKeyedArchiver archivedDataWithRootObject:intervalAsNumber];
    [[NSUserDefaults standardUserDefaults] 
     setObject:intervalAsData forKey:FDProcessListRefreshIntervalKey];
}

@end
