#import <Cocoa/Cocoa.h>
#import <Security/Authorization.h>
#include "BetterAuthorizationSampleLib.h"



/*!
 @class       SSYAuthorizedTaskMaster 
 @superclass  NSObject
 @abstract    A class for accessing your privileged Helper Tool to perform authorized tasks
 @discussion  Designed to instantiated as a singleton, but I suppose you could have more than
 one if you really wanted to.
 */
@interface SSYAuthorizedTaskMaster : NSObject {
	AuthorizationRef _authRef ;
	// We can't retain the C array _commandSpecs, but by
	// requiring it to be const we insure that it won't go away.
	const BASCommandSpec* _commandSpecs ;
	NSString* _helperToolName ;
	NSString* _installerToolName ;
}

/*!
    @method     sharedWorker 
    @abstract   To get and/or create a SSYAuthorizedTaskmaster singleton.
    @discussion Recommended unless you have > 1 authorized helper tool in an application.
    @result     The SSYAuthorizedTaskmaster singleton.
*/
+ (SSYAuthorizedTaskMaster*)sharedWorker ;

/*!
 @method     configureWithCommandSpecs:helperToolName:installerToolName:stringsFilename:bundleIdentifier:
 @abstract   Configures the instance
 @discussion Configurator which must be run before executeRequest:respons_p:error_p is invoked.
 The framework needs to know what its commands are, what additional prompt if any to
 present in the authentication dialog, and the name of the tool that executes the tasks.
 @param      commandSpecs a C array of BASCommandSpec structs.  Because we can't retain a C array,
 we require it to be const we insure that it won't go away.  commandSpecs must not be NULL.
 @param      helperToolName The name of the authorized helper tool which must be found in Contents/MacOS
 @param      installerToolName The name of the tool used to install the helper tool which must be
 found in Contents/MacOS.  If you pass nil, defaults to @"AuthorizedTaskHelperToolInstaller".
 @param      stringsFilename The name of a .strings file containing the prompt(s) specified in the
 rightDescriptionKey field(s) of the BASCommandSpec structs in commandSpecs.  If nil, defaults
 to the usual "Localizable".  This argument is only used for prepending a string to the " MyApp.app
 requires that you type your/administrator password" prompt in the authentication dialogs.  It may be nil.
 @param      bundleIdentifier The reverse-DNS ("com.MyCompany.MyApp") identifier of the bundle
 containing the stringsFilename.strings file in its Resources.  If nil, defaults to 
 [[NSBundle mainBundle] bundleIdentifier].  This argument is only used for prepending a string to the
 " MyApp.app requires that you type your/administrator password" prompt in the authentication dialogs.  
 It may be nil.
 */
- (void)configureWithCommandSpecs:(const BASCommandSpec *)commandSpecs
				   helperToolName:(NSString *)helperToolName
				installerToolName:(NSString *)installerToolName
				  stringsFilename:(NSString*)stringsFilename
				 bundleIdentifier:(NSString*)bundleIdentifier ;

/*!
 @method     executeRequest:response_p:error_p:
 @abstract   Executes request in the Helper Tool
 @discussion 
 @param      request request to be executed
 @param      response_p Pointer to which the response NSDictionary* will be assigned.  *response_p will be autoreleased, but you must close any file descriptors
 that you get in it.
 @param      #error_p Pointer to an NSError* to which any error will be assigned
 @result     YES if successful, NO otherwise
 */
- (BOOL)executeRequest:(NSDictionary*)request
			response_p:(NSDictionary**)response_p
			   error_p:(NSError**)error_p ;

/*!
 @method     removeRight:
 @abstract   Removes a right from the system using AuthorizationRightRemove()
 @discussion A utility method, useful in debugging
 @param      rightName The name of the right to be removed
 */
- (void)removeRight:(char*)rightName ;

/*!
 @method     recreateAuthorization:
 @abstract   Destroys the instance's authorization (_authRef) using AuthorizationFree() and then
 creates a new one using AuthorizationCreate().
 @discussion A utility method, useful in debugging
 @param      rightName The name of the right to be removed
 */
- (void)recreateAuthorization ;

@end
