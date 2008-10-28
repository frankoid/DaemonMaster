//
//  FDSpaceSeparateArrayValueTransformer.h
//  DaemonMaster
//
//  Created by Francis Devereux on 28/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

// A value transformer that converts an array into a space-separated string
@interface FDArrayToSpaceSeparatedStringTransformer : NSValueTransformer {

}

+ (void)registerTransformer;
@end
