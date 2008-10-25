#import "SSYAuthorizedTaskMaster.h"
//#import "AuthorizedTasksCommon.h"
#import "NSError+Easy.h"
#include "CommandProcArguments.h"
#include "BetterAuthorizationSampleLib.h"
#import <Cocoa/Cocoa.h>

static SSYAuthorizedTaskMaster* sharedWorker = nil ;

@implementation SSYAuthorizedTaskMaster

+ (SSYAuthorizedTaskMaster*)sharedWorker {
	@synchronized(self) {
        if (sharedWorker == nil) {
            sharedWorker = [[self alloc] init] ; 
        }
	}
    return sharedWorker ;
}

- (void)setHelperToolName:(NSString*)helperToolName {
    if (_helperToolName != helperToolName) {
        [_helperToolName release];
        _helperToolName = [helperToolName retain];
    }
}

- (void)setInstallerToolName:(NSString*)installerToolName {
	if (_installerToolName != installerToolName) {
        [_installerToolName release];
        _installerToolName = [installerToolName retain];
    }
}

- (void)configureWithCommandSpecs:(const BASCommandSpec *)commandSpecs
				   helperToolName:(NSString *)helperToolName
				installerToolName:(NSString *)installerToolName
				  stringsFilename:(NSString*)stringsFilename
				 bundleIdentifier:(NSString*)bundleIdentifier {
	_commandSpecs = commandSpecs ;

	if (bundleIdentifier == nil) {
		bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier] ;
	}
	
	BASSetDefaultRules(
					   _authRef, 
					   _commandSpecs, 
					   (CFStringRef)bundleIdentifier, 
					   (CFStringRef)stringsFilename
					   );
	[self setHelperToolName:helperToolName] ;
	
    if (installerToolName == nil) {
		installerToolName = @"AuthorizedTaskHelperToolInstaller" ;
	}
	[self setInstallerToolName:installerToolName] ;
}

- (const BASCommandSpec *)commandSpecs {
    return _commandSpecs ;
}

- (NSString *)helperToolName {
    return _helperToolName ;
}

- (NSString *)installerToolName {
    return _installerToolName ;
}

- init {
	self = [super init];
	
	if (self != 0)
	{
		OSStatus    err;
		
		// Create the AuthorizationRef that we'll use through this application.  We ignore 
		// any error from this.  A failure from AuthorizationCreate is very unusual, and if it 
		// happens there's no way to recover; Authorization Services just won't work.
		
		// If we don't do this, to assign _authRef before attempting a privileged task,
		// an assertion will burp out of BetterAuthorizationSampleLib.
		
		err = AuthorizationCreate(NULL, NULL, kAuthorizationFlagDefaults, &_authRef);
		assert(err == noErr);
		assert( (err == noErr) == (_authRef != NULL) );
		
	}
	
	return self ;
}

- (void)dealloc {
	[self setHelperToolName:nil] ;
	[self setInstallerToolName:nil] ;
	
	[super dealloc] ;
}

- (void)removeRight:(char*)rightName {
	if (rightName != NULL) {
		assert(AuthorizationRightRemove(
								_authRef,
								rightName
								   )  == noErr);
	}
}



- (void)recreateAuthorization {
    // Called when the user chooses the "Recreate" button.  This is just a testing 
    // convenience; it allows you to destroy the credentials that are stored in the cache 
    // associated with _authRef, so you can force the system to ask you for a password again.  
    // However, this isn't as convenient as you might think because the credentials might 
    // be cached globally.  See DTS Q&A 1277 "Security Credentials" for the gory details.
    //
    // <http://developer.apple.com/qa/qa2001/qa1277.html>
	OSStatus    err;
	
	// Free _authRef, destroying any credentials that it has acquired along the way. 
	
	err = AuthorizationFree(_authRef, kAuthorizationFlagDestroyRights);
	assert(err == noErr);
	_authRef = NULL;
	
	
	// Recreate it from scratch.
	
	err = AuthorizationCreate(NULL, NULL, kAuthorizationFlagDefaults, &_authRef);
	assert(err == noErr);
	assert( (err == noErr) == (_authRef != NULL) );    
}

- (BOOL)executeRequest:(NSDictionary*)request
			response_p:(NSDictionary**)response_p
			   error_p:(NSError**)error_p {
	SSYInitErrorP
	BOOL ok = YES ;
		
	int errCode ;
	NSString* errMsg ;
	
	if (request == nil) {
		errMsg = @"Nil request" ;
		errCode = 29875 ;
		SSYMakeAssignGeekyErrorP(errCode, errMsg) ;
		ok = NO ;
		goto end ;
	}
		
	if ([self commandSpecs] == NULL) {
		errMsg = @"Attempt to execute before commandSpecs have been set." ;
		errCode = 29257 ;
		SSYMakeAssignGeekyErrorP(errCode, errMsg) ;
		ok = NO ;
		goto end ;
	}
	
	if ([self helperToolName] == nil) {
		errMsg = @"Attempt to execute before helperToolName has been set." ;
		errCode = 29977 ;
		SSYMakeAssignGeekyErrorP(errCode, errMsg) ;
		ok = NO ;
		goto end ;
	}
	
	
	// Although our invoker may not need a response, BASExecuteRequestInHelperTool
	// requires one, so we make one if we weren't given one.
	if (response_p == nil) {
		NSMutableDictionary* response ;
		response_p = &response ;
	}
	
	NSString* bundleID = [[NSBundle mainBundle] bundleIdentifier] ;	
	assert(bundleID != nil) ;
	
	*response_p = nil ;
	OSStatus err = noErr - 1 ;
	int nTries = 0 ;
	
	NSString* msg = nil ;
	BASFailCode failCode = 999 ; // initialized to suppress compiler warning
	
	while ((err != noErr) && (nTries < 2)) {
		err = BASExecuteRequestInHelperTool(
											_authRef, 
											[self commandSpecs], 
											(CFStringRef)bundleID, 
											(CFDictionaryRef)request, 
											(CFDictionaryRef*)response_p
											);
		if (err == userCanceledErr) {
			break ;
		}
		
		nTries++ ;
		if (nTries > 1) {
			break ;
		}
		
		// If it failed, try to reinstall the helper tool.
		if (err != noErr) {
			// BASFixFailure needs the failCode
			failCode = BASDiagnoseFailure(_authRef, (CFStringRef) bundleID);
			
			// BASFixFailure tries to reinstall the helper tool
			OSStatus fixErr = BASFixFailure(
								_authRef,
								(CFStringRef) bundleID,
								(CFStringRef)[self installerToolName],
								(CFStringRef)[self helperToolName],
								failCode
								) ;
			if (fixErr == userCanceledErr) {
				break ;
			}
			
			if (fixErr != noErr) {
				// The helper tool could not be installed or reinstalled.
				// So we go back to what the original failure was and try to inform the user
				// It is left as a homework assignment if anyone wants to localize this:
				switch (failCode) {
					case kBASFailUnknown:
						msg = @"failed for an unknown reason." ;
						break ;
					case kBASFailDisabled:
						msg = @"is installed but disabled." ;
						break ;
					case kBASFailPartiallyInstalled:
						msg = @"is only partially installed." ;
						break ;
					case kBASFailNotInstalled: 
						msg = @"is not installed at all." ;
						break ;
					case kBASFailNeedsUpdate:
						msg = @"is installed but out of date." ;
						break ;
				}
				msg = [NSString stringWithFormat:
					   @"%@ %@  Tried to reinstall it using %@ but that failed too.",
					   [self helperToolName],
					   msg,
					   [self installerToolName]] ;
			}			
		}
	}
	
	// If all of the above went OK, it means that the IPC (inter-process communication)
	// to the helper tool worked.  But that doesn't mean all is OK yet...
	// We now have to check the response to see if the command's execution within 
	// the helper tool was successful.
	if ((err == noErr) && (*response_p != nil)) {
		err = BASGetErrorFromResponse((CFDictionaryRef)*response_p) ;
	}
	
	// According to BASExecuteRequestInHelperTool, we must release *response_p.
	// Also, it will be nil if the request fails.
	// But the invoker needs it.  So, we autorelease it.
	[*response_p autorelease] ;
	
	// If error, assign ok and *error_p	
	if (err != noErr) {
		if (msg == nil) {
			msg = [NSString stringWithFormat:@"AuthorizedTaskHelperTool returned OSStatus %d", err] ;
			err = failCode ;
		}
		SSYMakeAssignErrorP(err, msg) ;
		ok = NO ;
	}
	else if (*response_p == nil) {
		ok = NO ;
		msg = @"No data received from AuthorizedTaskHelperTool.\n" ;
		err = 39875 ;
		SSYMakeAssignErrorP(err, msg) ;
	}

end:
	return ok ;
}


@end