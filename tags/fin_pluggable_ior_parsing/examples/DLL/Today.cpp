// $Id$

#define ACE_BUILD_SVC_DLL

#include "Today.h"
#include "ace/Log_Msg.h"

// Implementation of the abstract class method which describes the
// magazine.

void 
Today::title (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Today: Information Technology Special Nov98\n"));
}

// Returns the pointer to the Today class.
// The ACE_BUILD_SVC_DLL and ACE_Svc_Export directives are necessary as take care
// of exporting the function for Win32 platforms.
extern "C" ACE_Svc_Export Magazine *create_magazine (void);

Magazine *create_magazine (void)
{
 Magazine *mag;
 ACE_NEW_RETURN (mag,
                 Today,
                 0);
 return mag;
}
