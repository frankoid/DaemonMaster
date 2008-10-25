//
//  FDDMAppDelegate.m
//  DaemonMaster
//
//  Created by Francis Devereux on 21/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDDMAppDelegate.h"

#import "SSYAuthorizedTaskmaster/SSYAuthorizedTaskmaster.h"
#import "SSYAuthorizedTaskmaster+DaemonMaster.h"
extern const BASCommandSpec kCommandSpecs[];

@implementation FDDMAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    [[SSYAuthorizedTaskMaster sharedWorker] configureWithCommandSpecs:kCommandSpecs
                                                       helperToolName:@"AuthorizedTaskHelperTool_DaemonMaster"
                                                    installerToolName:nil    // Use default
                                                      stringsFilename:nil    // Use default
                                                     bundleIdentifier:nil] ; // Use default
    
}

@end
