/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_stropts.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_STROPTS_H
# define ACE_ACE_STROPTS_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_STROPTS_H)
#   if defined (ACE_HAS_STREAMS)
#     if defined (AIX)
#       if !defined (_XOPEN_EXTENDED_SOURCE)
#         define _XOPEN_EXTENDED_SOURCE
#       endif /* !_XOPEN_EXTENDED_SOURCE */
#       include /**/ <stropts.h>
#       undef _XOPEN_EXTENDED_SOURCE
#     endif /* AIX */
#   else 
#     include /**/ <stropts.h>
#   endif /* ACE_HAS_STREAMS */
# endif /* ACE_LACKS_STROPTS_H */

# if defined (ACE_LACKS_STRRECVFD)
struct strrecvfd {};
# endif /* ACE_LACKS_STRRECVFD */

# if !defined (ACE_HAS_STRBUF_T)
struct strbuf
{
  int maxlen; // no. of bytes in buffer.
  int len;    // no. of bytes returned.
  void *buf;  // pointer to data.
};
# endif /* ACE_HAS_STRBUF_T */

extern "C" int isastream (int);

// should find a better place for this, but not
// OS.h!!!
# include "ace/OS_Export.h" // and this is why!
/**
 * @class ACE_Str_Buf
 *
 * @brief Simple wrapper for STREAM pipes strbuf.
 */
class ACE_OS_Export ACE_Str_Buf : public strbuf
{
public:
  // = Initialization method
  /// Constructor.
  ACE_Str_Buf (void *b = 0, int l = 0, int max = 0)//;
  {
    this->maxlen = max;
    this->len = l;
    this->buf = (char *) b;
  }

  /// Constructor.
  ACE_Str_Buf (strbuf &sb)//;
  {
    this->maxlen = sb.maxlen;
    this->len = sb.len;
    this->buf = sb.buf;
  }
};

#endif /* ACE_ACE_STROPTS_H */
