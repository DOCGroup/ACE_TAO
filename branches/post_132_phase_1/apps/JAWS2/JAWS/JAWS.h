// $Id$

#if (JAWS_NTRACE == 1)
# define JAWS_TRACE(X)
#else
# define JAWS_TRACE(X) ACE_Trace ____ (ACE_TEXT (X), \
                                       __LINE__, \
                                       ACE_TEXT (__FILE__))
#endif /* JAWS_NTRACE */

#ifndef JAWS_JAWS_H
#define JAWS_JAWS_H

#endif /* JAWS_JAWS_H */
