//
//  FDProcessInfoReader.h
//  DaemonMaster
//
//  Created by Francis Devereux on 21/10/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

// Reads information about processes from the system
@interface FDProcessInfoReader : NSObject {

}

+ (void)initialize;
- (NSArray *)processList;

@end
