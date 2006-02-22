// $Id$

#ifndef CONFIG_LYNXOS_PTHREAD_H
#define CONFIG_LYNXOS_PTHREAD_H
#include /**/ "ace/pre.h"

#ifdef pthread_cancel
# undef pthread_cancel
# define pthread_cancel(t) st_cancel_no_macro(t)
inline int st_cancel_no_macro(pthread_t t)
{
  return (st_cancel(t) < 0 ? -1 : 0);
}
#endif /* pthread_cancel */

#include /**/ "ace/post.h"
#endif /* CONFIG_LYNXOS_PTHREAD_H */
