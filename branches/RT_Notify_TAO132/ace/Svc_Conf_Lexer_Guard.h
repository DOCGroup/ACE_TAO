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

/// Forward declarations
class ACE_Svc_Conf_Param;

struct ace_yy_buffer_state;

/**
 * @class ACE_Svc_Conf_Lexer_Guard
 *
 * @brief "Guard" that ensures lexer buffer switching is
 *        exception-safe.
 *
 * Buffers are switched, if necessary, each time a token is
 * parsed/scanned.  The buffer switching must be synchronized
 * externally.  This class performs no synchronization.
 *
 * @note Note that allocation/deallocation is done once during the
 *       processing of a service configurator directive.  Memory
 *       managements is done at a higher level, not in this class.
 *       This is necessary to prevent an allocation/deallocation from
 *       occurring when parsing/scanning each token.
 */
class ACE_Svc_Conf_Lexer_Guard
{
public:

  /// Constructor
  /**
   * Switches buffers, if necessary, when token scanning first
   * begins.  Allocation of the buffer will also occur if one has not
   * already been allocated.  This operation effectively pushes a
   * buffer on to a stack.
   */
  ACE_Svc_Conf_Lexer_Guard (ACE_Svc_Conf_Param *param);

  /// Destructor
  /**
   * Switches buffers, if necessary when token scanning completes.  No
   * buffer deallocation occurs here.  Buffers are deallocated when
   * parsing of the entire directive is done, not when scanning of a
   * single token is done.  This operation effective pops a buffer off
   * of a stack.
   */
  ~ACE_Svc_Conf_Lexer_Guard (void);

private:

  /// Lexer buffer that corresponds to the current Service
  /// Configurator file/direct scan.
  ace_yy_buffer_state *buffer_;

};


#include "ace/post.h"

#endif  /* ACE_SVC_CONF_LEXER_GUARD_H */
