//
//  FDAppListController.m
//  DaemonMaster
//
//  Created by Francis Devereux on 16/07/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDAppListController.h"

//#import <LSInfo.h>

// Not public Apple API - see http://www.cocoadev.com/index.pl?AllApplications and http://developer.apple.com/technotes/tn/tn2029.html
extern void _LSCopyAllApplicationURLs(NSArray**);

//-- Private interface --------------------------------------------------------
@interface FDAppListController ()
- (NSArray *)allAppsAsDicts;
@end

//-- Implementation -----------------------------------------------------------
@implementation FDAppListController

- (id)init
{
    self = [super initWithWindowNibName:@"AppList"];
    return self;
}

- (void)awakeFromNib
{
    NSLog(@"In FDAppListController awakeFromNib");
}

- (void)windowDidLoad
{
    NSLog(@"In FDAppListController windowDidLoad, %@", appListArrayController);
    [appListArrayController setContent:[self allAppsAsDicts]];
}

- (NSArray *)allAppURLs
{
    NSArray *allAppURLs;
    _LSCopyAllApplicationURLs(&allAppURLs);
    [allAppURLs autorelease];
    return allAppURLs;
}

//- (NSArray *)findAllApplications
- (void)dumpAllApps:(id)sender
{
    NSLog(@"allAppURLs = %@", [self allAppURLs]);
}

static NSString *archNameFromInt(int appArch)
{
    NSString *appArchName;
    switch(appArch)
    {
        case NSBundleExecutableArchitectureI386:
            appArchName = @"i386";
            break;
        case NSBundleExecutableArchitecturePPC:
            appArchName = @"PPC";
            break;
        case NSBundleExecutableArchitectureX86_64:
            appArchName = @"x86_64";
            break;
        case NSBundleExecutableArchitecturePPC64:
            appArchName = @"PPC64";
            break;
        default:
            appArchName = [NSString stringWithFormat:@"Unknown architecture %x", appArch];
    }
    return appArchName;
}

// FJDTODO move to FJDStringUtil
static NSString *commaSeparatedStringFromArray(NSArray *array)
{
    NSMutableString *result = [[[NSMutableString alloc] init] autorelease];
    BOOL first = YES;
    for (id element in array)
    {
        if (first)
        {
            first = NO;
            [result appendFormat:@"%@", element];
        }
        else
        {
            [result appendFormat:@", %@", element];
        }
    }
    return result;
}

- (NSArray *)allAppsAsDicts
{
    NSSortDescriptor *archSortDesc = [[[NSSortDescriptor alloc] initWithKey:@"intValue" ascending:YES] autorelease];
    NSArray *archSortDescs = [NSArray arrayWithObject:archSortDesc];
    
    NSArray *allAppURLs = [self allAppURLs];
    NSMutableArray *allAppDicts = [[[NSMutableArray alloc] init] autorelease];
    for (NSURL *appURL in allAppURLs) {
        if (![appURL isFileURL])
        {
            NSLog(@"Application URL \"%@\" is not a file URL; skipping this application", appURL);
            continue;
        }
        
        NSString *appPath = [appURL path];
        NSBundle *appBundle = [NSBundle bundleWithPath:appPath];
        NSArray *appArchs = [appBundle executableArchitectures];
        appArchs = [appArchs sortedArrayUsingDescriptors:archSortDescs];
//      NSMutableString *sAppArchs = [[NSMutableString alloc] init];
        NSMutableArray *appArchNames = [[NSMutableArray alloc] init];
        for (NSNumber *appArch in appArchs)
        {
            int iAppArch = [appArch intValue];
            [appArchNames addObject:archNameFromInt(iAppArch)];
        }
        
        NSString *sAppArchNames = commaSeparatedStringFromArray(appArchNames);
        
        NSMutableDictionary *appDict = [[[NSMutableDictionary alloc] init] autorelease];
        [appDict setObject:appPath forKey:@"path"];
        [appDict setObject:sAppArchNames forKey:@"archs"];
        [allAppDicts addObject:appDict];
    }
    
    return allAppDicts;
}

@end
