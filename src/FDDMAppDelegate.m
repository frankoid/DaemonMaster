//
//  FDDMAppDelegate.m
//  DaemonMaster
//
//  Created by Francis Devereux on 21/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDDMAppDelegate.h"
#import "FDArrayToSpaceSeparatedStringTransformer.h"

@implementation FDDMAppDelegate

+ (void)initialize
{
    [FDArrayToSpaceSeparatedStringTransformer registerTransformer];
}

// - (void)applicationDidFinishLaunching:(NSNotification *)notification
// {
// }

@end
