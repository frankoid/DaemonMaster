#import "SSYAuthorizedTaskmaster+DaemonMaster.h"
#import "AuthorizedTasks_DaemonMaster.h"
#import "NSError+Easy.h"
#include <unistd.h>     // for close() in -openSomeLowPortNumbers_p:error_p
#include <netinet/in.h> // for sock... stuff in -openSomeLowPortNumbers_p:error_p

@implementation SSYAuthorizedTaskMaster (Tasks)

- (BOOL)getProcessList:(NSArray**)processList_p
                 error:(NSError**)error_p
{
	SSYInitErrorP

    assert (processList_p != NULL) ;
    
    // Create the request.  The request always contains the kBASCommandKey that 
    // describes the command to do. 
	NSDictionary* request = [NSDictionary dictionaryWithObject:@kGetProcessListCommand
														forKey:@kBASCommandKey] ;
	
	NSDictionary* response ;
	BOOL ok = [self executeRequest:request
						response_p:&response
						   error_p:error_p] ;
    
	if (!ok) {goto end;}
	
	// Process response to get desired data
    *processList_p = [response objectForKey:@kFDDMProcesses];
	
end:
	return ok ;
}

@end