// $Id$

#include "Today.h"

// This declaration is necessary to get this working on NT.
#if defined (ACE_HAS_WIN32)

#ifdef __cplusplus
// @@ Kirthika, this program will ALWAYS be compiled with a C++
// compiler!  Therefore, you can omit this __cplusplus #ifdef stuff...
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif
  // @@ Kirthika, please check to see which of the ACE macros, e.g.,
  // ACE_SVC_EXPORT, or ACE_EXPORT should be used here, rather than
  // having to explicitly #ifdef your code you should use the ACE
  // macros for this.
	__declspec( dllexport ) Magazine *create_magazine (void);

#ifdef __cplusplus
}
#endif

// @@ Kirthika, please make sure that you always do something like this
// #endif /* ACE_HAS_WIN32 */ when you close a #ifdef block.  However,
// in this case, I think you can remove the #if defined
// (ACE_HAS_WIN32) altogether since that'll be handled by the
// ACE_*_EXPORT macro!

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
