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

#define CIAO_REGISTER_VALUE_FACTORY(ORB,FACTORY,VALUETYPE)       {\
        CORBA::ValueFactory factory = new FACTORY; \
        CORBA::ValueFactory prev_factory = \
          ORB->register_value_factory \
          (VALUETYPE::_tao_obv_static_repository_id (), \
           factory); \
        if (prev_factory) prev_factory->_remove_ref (); \
        factory->_remove_ref ();      }

#include /**/ "ace/post.h"
#endif /* CIAO_COMMON_H */
