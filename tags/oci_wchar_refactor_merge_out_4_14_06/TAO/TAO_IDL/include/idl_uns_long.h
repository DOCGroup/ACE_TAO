// $Id$
//
// idl type usigned long
//
// Either an 'unsigned long long' or an 'unsigned long'.
//

#ifndef _IDL_UNS_LONG_H
#define _IDL_UNS_LONG_H

#include "ace/Basic_Types.h"

#if defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
  // Platform has "long long" but no unsigned long long.  Use a long long
  // for the container.
  typedef long long idl_uns_long;
#elif defined (ACE_LACKS_LONGLONG_T)
  typedef unsigned long idl_uns_long;
#else
  typedef ACE_UINT64    idl_uns_long;
#endif /* ACE_LACKS_LONGLONG_T */

#endif /* _IDL_UNS_LONG_H */
