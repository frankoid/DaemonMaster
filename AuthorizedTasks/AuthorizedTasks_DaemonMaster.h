#include "BetterAuthorizationSampleLib.h"
#include "CommandProcArguments.h"



// *******************
// Helper Tool Version
// *******************
extern const int kFDDMCurrentHelperVersion;

// ********************************
// Constants for Each of your Tasks
// ********************************

// Descriptions of each task:

// "GetHelperVersion" returns the version of the helper tool that is running.
// If this does not match the version compiled into the main app then the
// helper is out of date and needs to be updated.

// "GetProcessList" gets a list of all processes running on the system.


// Declare a "command proc" (function) for each task.  (These are defined in the Helper Tool.)

OSStatus DoGetHelperVersion(COMMAND_PROC_ARGUMENTS);
OSStatus DoGetProcessList(COMMAND_PROC_ARGUMENTS);

// Declare a "command name" for each task.

#define kGetHelperVersionCommand    "GetHelperVersion"
#define kGetProcessListCommand      "GetProcessList"

// Declare a "right name" for each task.

#define kGetHelperVersionRightName  "org.devrx.DaemonMaster.GetHelperVersion"
#define kGetProcessListRightName    "org.devrx.DaemonMaster.GetProcessList"

// ***********************************************************
// Optional Keys used to Pass Data to and from the Helper Tool
// ***********************************************************

/*
 These keys appear in the request and response dictionaries.
 You can re-use these in different tasks, or define your own as needed.
 These are defined without the "@" prefix so that they can be used in CFSTR()
 To use create NSStrings in Cocoa methods, add the "@" prefix.
 You may pass any ^serializable^ [1] CoreFoundation object
 (CFString, CFNumber, CFBoolean, CFDate, CFData, CFArray, and CFDictionary).
 If you wish to pass nonserializable objects you must archive them into a CFData.
 Wrapping nonserializable objects into a dictionary or array is not good enough.
 [1] http://developer.apple.com/documentation/CoreFoundation/Conceptual/CFPropertyLists/CFPropertyLists.html
*/ 
#define kFDDMHelperVersion  "helperVersion"     // CFNumber
#define kFDDMProcesses      "processes"         // CFArray
#define kFDDMPID            "pid"               // CFNumber
#define kFDDMArgs           "args"              // CFArray of CFStrings process's
