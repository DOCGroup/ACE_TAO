// -*- C++ -*-
//=============================================================================
/**
 *  @file   Stack_Trace.h
 *
 *  $Id$
 *
 *  @brief  Encapsulate string representation of stack trace.
 *
 *  @author Chris Cleeland <cleeland at ociweb dot com>
 */
//=============================================================================

#ifndef ACE_STACK_TRACE_H
#define ACE_STACK_TRACE_H

#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"
#include "ace/Basic_Types.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#  ifndef ACE_STACK_TRACE_SYMBUFSIZ
#    define ACE_STACK_TRACE_SYMBUFSIZ 4096
#  endif

/**
 *  @class ACE_Stack_Trace
 *
 *  @brief Encapsulate a string representation of a stack trace on supported platforms.
 *  Stack traces for code built with optimize=1 (or "Release" configs on Visual
 *  Studio) may be misleading (missng frames) due to inlining performed by the
 *  compiler, which is indepenent of the inline=0 / inline=1 build option and
 *  the __ACE_INLINE__ / ACE_NO_INLINE macros.
 */
class ACE_Export ACE_Stack_Trace
{
public:
  /**
   * @brief Grab a snapshot of the current stack trace and hold it for later use.
   *
   * @param starting_frame_offset offset into the array of frames to start printing; 0 is the platform-specific offset for the first frame, positive numbers give less frames, negative give more frames
   * @param num_frames the number of stack frames to include (0 indicates platform-specific maximum)
   *
   */
  explicit ACE_Stack_Trace (ssize_t starting_frame_offset = 0, size_t num_frames = 0);

  /**
   * @brief Return buffer as a C-style string.
   * @return C-style string with string representation of stack trace.
   * @note Lifecycle of string follows lifecycle of ACE_Stack_Trace instance.
   */
  const char* c_str() const;

  static const size_t SYMBUFSIZ = ACE_STACK_TRACE_SYMBUFSIZ;

private:
  char buf_[SYMBUFSIZ];
  size_t buflen_;

  static const char UNSUPPORTED[];
  static const char UNABLE_TO_GET_TRACE[];

  void generate_trace (ssize_t starting_frame_offset, size_t num_frames);
};

#include "ace/post.h"
#endif /* ACE_STACK_TRACE_H */

