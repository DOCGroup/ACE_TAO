// $Id$

/**
 * @file CIAO_common.h
 *
 * This file collects common CIAO macro definitions.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_COMMON_H
#define CIAO_COMMON_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_Client_Export.h"

#define CIAO_REGISTER_VALUE_FACTORY(ORB,FACTORY,VALUETYPE)       {\
        CORBA::ValueFactory factory = new FACTORY; \
        CORBA::ValueFactory prev_factory = \
          ORB->register_value_factory \
          (VALUETYPE::_tao_obv_static_repository_id (), \
           factory); \
        if (prev_factory) prev_factory->_remove_ref (); \
        factory->_remove_ref ();      }

namespace CIAO
{
  /**
   * Return the debug level.  The debug level of CIAO is control by
   * an environment variable "CIAO_DEBUG_LEVEL".  It should be an int
   * value.  If it is not defined, the default debug level is 0.  The
   * value of debug value is evaluated on its first use and the value
   * is then cached.  The actual implementation of this function is in
   * Client_init.cpp.
   *
   * Some recommendation for using the debug_level
   *
   *    > 0 : For component users.  Component developers are encouraged
   *          to develop their own
   *   > 10 : For CIAO tools
   *   > 20 : For CIAO core.
   */
  CIAO_CLIENT_Export int debug_level (void);
}

#include /**/ "ace/post.h"
#endif /* CIAO_COMMON_H */
