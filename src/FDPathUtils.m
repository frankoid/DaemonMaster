//
//  FDPathUtils.m
//  DaemonMaster
//
//  Created by Francis Devereux on 21/05/2008.
//  Copyright 2008 Francis Devereux. All rights reserved.
//

#import "FDPathUtils.h"

NSString *FDFileInPath(NSSearchPathDirectory directory,
					   NSSearchPathDomainMask domainMask,
					   NSString *filename)
{
	NSFileManager *fm = [NSFileManager defaultManager];
	
	NSArray *searchPath = NSSearchPathForDirectoriesInDomains(directory, domainMask, YES);
	for (NSString *pathEntry in searchPath)
	{
		NSArray *pathComponents = [[[NSArray alloc] initWithObjects:pathEntry, filename, nil] autorelease];
		NSString *fileInPath = [NSString pathWithComponents:pathComponents];
		if ([fm fileExistsAtPath:fileInPath])
		{
			return fileInPath;
		}
	}
	// not found
	return nil;
}


NSString *FDFileInPathSubdirectories(NSSearchPathDirectory directory,
									 NSSearchPathDomainMask domainMask,
									 NSArray *subdirs,
									 NSString *filename)
{
	NSFileManager *fm = [NSFileManager defaultManager];
	
	NSArray *searchPath = NSSearchPathForDirectoriesInDomains(directory, domainMask, YES);
	for (NSString *pathEntry in searchPath)
	{
		for (NSString *subdir in subdirs)
		{
			NSArray *pathComponents = [[[NSArray alloc] initWithObjects:pathEntry, subdir, filename, nil] autorelease];
			NSString *fileInPath = [NSString pathWithComponents:pathComponents];
			if ([fm fileExistsAtPath:fileInPath])
			{
				return fileInPath;
			}
		}
	}
	// not found
	return nil;
}