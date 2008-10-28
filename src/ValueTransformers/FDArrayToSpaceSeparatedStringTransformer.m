//
//  FDSpaceSeparateArrayValueTransformer.m
//  DaemonMaster
//
//  Created by Francis Devereux on 28/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDArrayToSpaceSeparatedStringTransformer.h"

@implementation FDArrayToSpaceSeparatedStringTransformer

+ (void)registerTransformer
{
    FDArrayToSpaceSeparatedStringTransformer *transformer =
        [[[FDArrayToSpaceSeparatedStringTransformer alloc] init] autorelease];
    
    [NSValueTransformer setValueTransformer:transformer
                                    forName:@"FDArrayToSpaceSeparatedStringTransformer"];
}

+ (Class)transformedValueClass
{
    return [NSString class];
}

+ (BOOL)allowsReverseTransformation
{
    return NO;
}

- (id)transformedValue:(id)value
{
    if (value == nil) return nil;
    
    
    NSArray *array = value;
    NSMutableString *result = [[[NSMutableString alloc] init] autorelease];
    
    BOOL first = YES;
    id object;
    for (object in array)
    {
        if (first)
        {
            first = NO;
        }
        else
        {
            [result appendString:@" "];
        }
        
        [result appendString:[object description]];
    }
    
    return result;
}
@end
