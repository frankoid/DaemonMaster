#import "NSError+Easy.h"

@implementation NSError (Easy) 

+ (NSError*)errorWithLocalizedDetails:(NSString*)localizedDetails
								 code:(int)code
							   sender:(id)sender
							 selector:(SEL)selector {
	if (localizedDetails == nil) {
		localizedDetails = @"unspecified" ;
	}
	if (sender != nil) {
		localizedDetails = [localizedDetails stringByAppendingFormat:@"\n   Object Class: %@",
							NSStringFromClass([sender class])] ;

	}
	if (selector != NULL) {
		localizedDetails = [localizedDetails stringByAppendingFormat:@"\n   Method: %@",
							NSStringFromSelector(selector)] ;
	}
	NSDictionary* userInfo = [NSDictionary dictionaryWithObject:localizedDetails
														 forKey:NSLocalizedDescriptionKey] ;
	NSString* domain = [[NSBundle mainBundle] bundleIdentifier] ;
	return [NSError errorWithDomain:domain
							   code:code
						   userInfo:userInfo] ;
}

+ (NSError*)errorWithHTTPStatusCode:(int)code
							 sender:(id)sender 
						   selector:(SEL)selector {
	NSString* localizedDetails = [NSString stringWithFormat:@"HTTP Status Code: %d %@",
								  code,
								  [NSHTTPURLResponse localizedStringForStatusCode:code]] ;
	return [self errorWithLocalizedDetails:localizedDetails
									  code:code
									sender:sender
								  selector:selector] ;
}

- (NSError*)errorByAddingUserInfoObject:(id)object
								 forKey:(NSString*)key {
	NSMutableDictionary* userInfo = [[self userInfo] mutableCopy] ;
	if (object != nil) {
		if (userInfo) {
			// Make sure that key does not already exist in userInfo,
			// as ^it will^ if -errorByAddingMoreInfo: is invoked on the
			// same error more than once by a programmer who is too
			// lazy to use -errorByAddingUserInfoObject:forKey:.
			int i = 2 ;
			while ([[userInfo allKeys] indexOfObject:key] != NSNotFound) {
				key = [NSString stringWithFormat:@"More Info %d", i++] ;
			}
		}
		else {
			userInfo = [[NSMutableDictionary alloc] initWithCapacity:1] ;
		}
		[userInfo setObject:object forKey:key] ;
	}
	int code = [self code] ;
	NSString* domain = [self domain] ;
	NSError* newError = [NSError errorWithDomain:domain
											code:code
										userInfo:userInfo] ;
	[userInfo release] ;
	return newError ;
}

- (NSError*)errorByAddingLocalizedFailureReason:(NSString*)newText {
	return [self errorByAddingUserInfoObject:newText
									  forKey:NSLocalizedFailureReasonErrorKey] ;
}

- (NSError*)errorByAddingLocalizedRecoverySuggestion:(NSString*)newText {
	return [self errorByAddingUserInfoObject:newText
									  forKey:NSLocalizedRecoverySuggestionErrorKey] ;
}

- (NSError*)errorByAddingUnderlyingError:(NSError*)underlyingError {
	return [self errorByAddingUserInfoObject:underlyingError
									  forKey:NSUnderlyingErrorKey] ;
}

- (NSError*)errorByAddingMoreInfo:(NSString*)newText {
	return [self errorByAddingUserInfoObject:newText
									  forKey:@"More Info"] ;
}

@end