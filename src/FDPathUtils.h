//
//  FDPathUtils.h
//  DaemonMaster
//
//  Created by Francis Devereux on 21/05/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NSString *FDFileInPath(NSSearchPathDirectory directory,
					   NSSearchPathDomainMask domainMask,
					   NSString *file);

NSString *FDFileInPathSubdirectories(NSSearchPathDirectory directory,
									 NSSearchPathDomainMask domainMask,
									 NSArray *subdirs,
									 NSString *filename);