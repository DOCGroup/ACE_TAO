// $Id$

#include "Today.h"

// This declaration is necessary to get this working on NT.
#if defined (ACE_HAS_WIN32)

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif
	__declspec( dllexport ) Magazine *create_magazine (void);

#ifdef __cplusplus
}
#endif

#endif

// Implementation of the abstract class method which describes the
// magazine.
void 
Today::title (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Today: Information Technology Special Nov98\n"));
}

// Returns the pointer to the Today class.

extern "C" 
Magazine *create_magazine (void)
{
 Magazine *mag;
 ACE_NEW_RETURN (mag,
                 Today,
                 0);
 return mag;
}
