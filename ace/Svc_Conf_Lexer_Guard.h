// -*- C++ -*-

//=============================================================================
/**
 *  @file    Svc_Conf_Lexer_Guard.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef ACE_SVC_CONF_LEXER_GUARD_H
#define ACE_SVC_CONF_LEXER_GUARD_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Svc_Conf_Lexer_Guard
 *
 * @brief "Guard" that ensures lexer buffer stack manipulation is
 *        exception-safe.
 *
 * Buffers are allocated and deallocated when scanning a file or a
 * string.  This class utilizes the "guard" idiom to perform stack
 * pushing and popping before and after parsing/scanning.
 * @par
 * The underlying stack allows nested scans to occur.  For example,
 * while scanning a `svc.conf' file, a Service Object's init() method
 * could invoke a Service Configurator directive, which would require
 * "moving" the current lexer state out of the way (pushing it onto
 * the stack implementation).
 */
class ACE_Svc_Conf_Lexer_Guard
{
public:

  /// Constructor
  /**
   * Create a new buffer to be used when scanning a new Service
   * Configurator file, push it onto the underlying buffer stack,
   * and make it the current buffer.
   */
  ACE_Svc_Conf_Lexer_Guard (FILE *file);

  /// Constructor
  /**
   * Create a new buffer to be used when scanning a new Service
   * Configurator directive, push it onto the underlying buffer stack,
   * and make it the current buffer.
   */
  ACE_Svc_Conf_Lexer_Guard (const char *directive);

  /// Destructor
  /**
   * Pop the current buffer off of the underlying buffer stack,
   * and make the previous buffer (i.e. the one on the top of the
   * stack), the current buffer.
   */
  ~ACE_Svc_Conf_Lexer_Guard (void);

};


#include "ace/post.h"

#endif  /* ACE_SVC_CONF_LEXER_GUARD_H */
