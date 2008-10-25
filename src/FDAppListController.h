//
//  FDAppListController.h
//  DaemonMaster
//
//  Created by Francis Devereux on 16/07/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FDAppListController : NSObject {
	IBOutlet NSArrayController *appListArrayController;
}

- (void)awakeFromNib;

- (void)dumpAllApps:(id)sender;

@end
