#import <Cocoa/Cocoa.h>

// Macros for making NSErrors

/*
 Quick macros to make a simple error without much thinking
 First argument is int, second is NSString*.
 */
#define SSYMakeError(_code,_localizedDetails) [NSError errorWithLocalizedDetails:_localizedDetails code:_code sender:nil selector:NULL]
#define SSYMakeHTTPError(_code) [NSError errorWithHTTPStatusCode:_code sender:nil selector:NULL]

/*
 Adds sender class and method name to the localized description,
 as described in errorWithLocalizedDetails:code:sender:selector below.
 This is good for errors which are not often expected; if you have a 
 "Copy to Clipboard" button or "Email this error to Support"
 on your alert users may copy and send it to your support email.
 This macro will only compile within an Objective-C method because
 it needs the local variables _cmd and self.
 First argument is int, second is NSString*.
*/ 
#define SSYMakeGeekyError(_code,_localizedDetails) [NSError errorWithLocalizedDetails:_localizedDetails code:_code sender:self selector:_cmd]
#define SSYMakeGeekyHTTPError(_code) [NSError errorWithHTTPStatusCode:_code sender:self selector:_cmd]

// Macros for initializing and assigning an NSError** named error_p

/*
 These are useful within functions that get an argument (NSError**)error_p
 Use SSYInitErrorP to assign it to *error_p to nil at beginning of function.
 (This is optional.  Apple doesn't do it in their methods that take NSError**,
 but I don't like the idea of leaving it unassigned.)
 Then, use the other three to assign to *error_p if/when an error occurs.
 Benefit: All of these macros check that error_p != NULL before assigning.
*/
#define SSYAssignErrorP(_error) if (error_p != NULL) {*error_p = _error ;}
#define SSYInitErrorP SSYAssignErrorP(nil) ;
#define SSYMakeAssignErrorP(_code,_localizedDetails) SSYAssignErrorP(SSYMakeError(_code,_localizedDetails))
#define SSYMakeAssignGeekyErrorP(_code,_localizedDetails) SSYAssignErrorP(SSYMakeGeekyError(_code,_localizedDetails))



@interface NSError (Easy) 

/*
 If sender != nil, will add the following line to localized description:
     "   Object Class: <name of sender's class>"
 If selector != nil, will add the following line to localized description:
     "   Method: <name of method>"
 */ 
+ (NSError*)errorWithLocalizedDetails:(NSString*)localizedDetails
								 code:(int)code
							   sender:(id)sender 
							 selector:(SEL)selector ;

/*
 If sender != nil, will add the following line to localized description:
 "   Object Class: <name of sender's class>"
 If selector != nil, will add the following line to localized description:
 "   Method: <name of method>"
 */ 
+ (NSError*)errorWithHTTPStatusCode:(int)code
							 sender:(id)sender 
						   selector:(SEL)selector ;

/*
 Some methods for adding userInfo keys to errors already created
 */ 
/*!
 @method     errorByAddingLocalizedFailureReason:
 @abstract   Adds a string value for string key NSLocalizedFailureReasonErrorKey to userInfo of the receiver
 @param      newText The string value to be added for key NSLocalizedFailureReasonErrorKey
 @result     A new NSError object, identical to the receiver except for the additional key/value pair in userInfo
 */
- (NSError*)errorByAddingLocalizedFailureReason:(NSString*)newText ;

/*!
 @method     errorByAddingLocalizedRecoverySuggestion:
 @abstract   Adds a string value for string key NSLocalizedRecoverySuggestionErrorKey to userInfo of the receiver
 @param      newText The string value to be added for key NSLocalizedRecoverySuggestionErrorKey
 @result     A new NSError object, identical to the receiver except for the additional key/value pair in userInfo
 */
- (NSError*)errorByAddingLocalizedRecoverySuggestion:(NSString*)newText ;

/*!
 @method     errorByAddingUnderlyingError:
 @abstract   Adds an errror for string key NSUnderlyingErrorKey to userInfo of the receiver
 @param      underlyingError The error value to be added for key NSUnderlyingErrorKey
 @result     A new NSError object, identical to the receiver except for the additional key/value pair in userInfo
 */
- (NSError*)errorByAddingUnderlyingError:(NSError*)underlyingError ;

/*!
 @method     errorByAddingUserInfoObject:forKey:
 @abstract   Adds object for key into the userInfo of the receiver
 @param      object of the pair to be added
 @param      key of the pair to be added
 @result     A new NSError object, identical to the receiver except for the additional key/value pair in userInfo
 */
- (NSError*)errorByAddingUserInfoObject:(id)object
								 forKey:(NSString*)key ;

/*!
 @method     errorByAddingMoreInfo:
 @abstract   Adds a string value for string key @"More Info" to userInfo of the receiver,
             although if you invoke this method more than once the second newText will be
             in the key "More Info 2", etc.
 @discussion This is a cheesy method.  Especially if you might need to invoke this more than once
             on the same NSErrorObject, send -errorByAddingUserInfoObject instead and give each
             piece of info its own nice descriptive key.
 @param      newText The string value to be added for key @"More Info"
 @result     A new NSError object, identical to the receiver except for the additional key/value pair in userInfo
 */
- (NSError*)errorByAddingMoreInfo:(NSString*)newText ;

@end
