// $Id$

#if !defined (ACE_BOOL_H)
#define ACE_BOOL_H

#if defined (_MSC_VER)
  // Define bool out of the std namespace.
# if _MSC_VER > 1010
#   include /**/ <yvals.h>
# else  /* _MSC_VER <= 1010 */
#   if !defined (bool)
#     define bool int
#   endif /* bool */

#   if !defined (true)
#     define true 1
#   endif /* true */

#   if !defined (false)
#     define false 0
#   endif /* false */
# endif /* _MSC_VER <= 1010 */
#endif /* _MSC_VER */


 /*
 *Added by d:\\convert.pl --begin--
 */
namespace std {
/*
 *Added by d:\\convert.pl --end--
 */

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company and Microsoft
 * Corporation make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 */

/*
 *Added by d:\\convert.pl --begin--
 */
}
/*
 *Added by d:\\convert.pl --end--
 */

#endif /* ACE_BOOL_H */
