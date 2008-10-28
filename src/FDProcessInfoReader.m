//
//  FDProcessInfoReader.m
//  DaemonMaster
//
//  Created by Francis Devereux on 21/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDProcessInfoReader.h"

#import "SSYAuthorizedTaskmaster/SSYAuthorizedTaskmaster.h"
#import "SSYAuthorizedTaskmaster+DaemonMaster.h"
extern const BASCommandSpec kCommandSpecs[];

#import <sys/sysctl.h>

@implementation FDProcessInfoReader

+ (void)initialize
{
	//NSLog(@"FDProcessInfoReader initialize: about to configure SSYAuthorizedTaskMaster");
    [[SSYAuthorizedTaskMaster sharedWorker] configureWithCommandSpecs:kCommandSpecs
                                                       helperToolName:@"AuthorizedTaskHelperTool_DaemonMaster"
                                                    installerToolName:nil    // Use default
                                                      stringsFilename:nil    // Use default
                                                     bundleIdentifier:nil] ; // Use default
    
}

- (NSArray *)processList;
{
    NSError *error = nil;
    NSArray *processList = nil;
    BOOL ok = [[SSYAuthorizedTaskMaster sharedWorker] getProcessList:&processList 
                                                               error:&error];
    if (!ok)
    {
        // FJDTODO proper error handling
        NSLog(@"getProcessList error: %@", [error localizedDescription]);
        return nil;
    }
    return processList;
}

@end
