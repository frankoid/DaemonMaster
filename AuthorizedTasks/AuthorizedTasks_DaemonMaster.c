#include "BetterAuthorizationSampleLib.h"
#include "AuthorizedTasks_DaemonMaster.h"

/*
  Helper tool version - update when the helper tool is changed
*/
const int kFDDMCurrentHelperVersion = 1;

/*
 The kCommandSpecs array is used by both the app and the helper tool to define
 the set of supported commands to the BetterAuthorizationSampleLib module.
 
 The kCommandProcs array is used by the helper tool only.
 
 It is important that the two arrays kCommandSpecs and kCommandProcs
 have the same number of elements and that corresponding elements refer to the same tasks.
*/

const BASCommandSpec kCommandSpecs[] = {
    {   kGetHelperVersionCommand,           // commandName
        kGetHelperVersionRightName,         // rightName
        "allow",                            // rightDefaultRule    -- by default, anyone can acquire this right
        NULL,                               // rightDescriptionKey -- key for custom prompt in .strings resource
        NULL                                // userData
    },
    
    {   kGetProcessListCommand,             // commandName
        kGetProcessListRightName,           // rightName
        "allow",                            // rightDefaultRule    -- by default, anyone can acquire this right
        "getProcessListPrompt",             // rightDescriptionKey -- key for custom prompt in .strings resource
        NULL                                // userData
    },
    
    // Use null termination so that we can loop through this array without
    // having to know its count.
    {   NULL,
        NULL, 
        NULL, 
        NULL,
        NULL
    }
};
