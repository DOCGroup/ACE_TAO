// -*- C++ -*-

#ifndef TAO_INTERCEPTOR_TYPE_H
#define TAO_INTERCEPTOR_TYPE_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

/// The different type of interceptors have different functionality
/// to perform.
enum class Interceptor_Type
{
  /// Do not register an interceptor
  none,

  /// Does nothing on all interception points
  noop,

  /// Perform service context manipulation
  context,

  /// Invoke dynamic interface methods and does extraction from anys.
  dynamic
};

Interceptor_Type get_interceptor_type (int argc, ACE_TCHAR *argv[]);

#include /**/ "ace/post.h"
#endif /* TAO_INTERCEPTOR_TYPE_H */
