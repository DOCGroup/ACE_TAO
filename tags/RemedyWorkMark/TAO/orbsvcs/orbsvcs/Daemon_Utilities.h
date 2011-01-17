// -*- C++ -*-
// $Id$

#ifndef _DAEMON_UTILS_H
#define _DAEMON_UTILS_H
#include /**/ "ace/pre.h"

#include "orbsvcs/svc_utils_export.h"

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Svc_Utils_Export TAO_Daemon_Utility
{
public:
  static void check_for_daemon (int &argc, ACE_TCHAR* argv[]);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif
