#import <Cocoa/Cocoa.h>
#import "SSYAuthorizedTaskMaster/SSYAuthorizedTaskMaster.h"


@interface SSYAuthorizedTaskMaster (Tasks) 

- (BOOL)getProcessList:(NSArray**)processList_p
                 error:(NSError**)error_p;

@end
