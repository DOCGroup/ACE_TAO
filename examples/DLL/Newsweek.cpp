// $Id$

#include "Newsweek.h"

// This declaration necessary to get thi sworking on NT.
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

// Implementation of the abstract class method which describes
// the magazine.
void Newsweek::title (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Newsweek: Vol 23 Dec87\n"));
}

// Returns the Newsweek class pointer.
extern "C"
Magazine *create_magazine (void)
{
  Magazine *mag;
  ACE_NEW_RETURN (mag,
                  Newsweek,
                  0);
  return mag;
}
