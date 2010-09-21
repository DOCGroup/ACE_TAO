// -*- C++ -*-
//
// $Id$

/**
 * @file Client_init.h
 *
 * Initializing CIAO client side ORB, if they need to.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_CLIENT_INIT_H
#define CIAO_CLIENT_INIT_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Base/CIAO_Base_stub_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class ORB;
  typedef ORB *ORB_ptr;
}
TAO_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  /**
   * The initialize routine for any clients that need to access
   * components. This routine now simply registers various valuetype
   * factories defined in CIAO_Component.pidl.  Currently, this method
   * should be call right after ORB initialization but we should try
   * to register these stuff automatically.
   */
  CIAO_BASE_STUB_Export int Client_init (CORBA::ORB_ptr o);
}

#define CIAO_REGISTER_VALUE_FACTORY(ORB,FACTORY,VALUETYPE)       {\
        CORBA::ValueFactory factory = new FACTORY; \
        CORBA::ValueFactory prev_factory = \
          ORB->register_value_factory \
          (VALUETYPE::_tao_obv_static_repository_id (), \
           factory); \
        if (prev_factory) prev_factory->_remove_ref (); \
        factory->_remove_ref ();      }

#include /**/ "ace/post.h"

#endif /* CIAO_CLIENT_INIT_H */
