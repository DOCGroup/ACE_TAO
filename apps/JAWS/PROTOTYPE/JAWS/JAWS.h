// $Id$

#if (JAWS_NTRACE == 1)
# define JAWS_TRACE(X)
#else
# define JAWS_TRACE(X) ACE_Trace ____ (ASYS_TEXT (X), \
                                       __LINE__, \
                                       ASYS_TEXT (__FILE__))
#endif /* JAWS_NTRACE */

#ifndef JAWS_JAWS_H
#define JAWS_JAWS_H

#endif /* JAWS_JAWS_H */
