#include "BetterAuthorizationSampleLib.h"
#include "CommandProcArguments.h"


// ********************************
// Constants for Each of your Tasks
// ********************************

// Descriptions of each task:

// "GetProcessList" gets a list of all processes running on the system.


// Declare a "command proc" (function) for each task.  (These are defined in the Helper Tool.)

OSStatus DoGetProcessList(COMMAND_PROC_ARGUMENTS) ;

// Declare a "command name" for each task.

#define kGetProcessListCommand				"GetProcessList"

// Declare a "right name" for each task.

#define	kGetProcessListRightName	"org.devrx.DaemonMaster.GetProcessList"

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
#define kFDDMProcesses      "Processes"         // CFArray
#define kFDDMPID            "PID"               // CFNumber
#define kFDDMCommand        "Command"           // CFString complete command (i.e. all of the process's argv)

