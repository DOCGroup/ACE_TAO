// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/CIAO/ciao
//
// = FILENAME
//    CIAO_Component_i.cpp
//
// = DESCRIPTION
//    CIAO's implementation for basic component supports.
//
// = AUTHOR
//    Nanbor Wang
//
// ===========================================================

#include "ciao/CIAO_ComponentC.h"

#define CIAO_REGISTER_VALUE_FACTORY(ORB,FACTORY,VALUETYPE)       {\
        CORBA_ValueFactory factory = new FACTORY; \
        CORBA_ValueFactory prev_factory = \
          ORB->register_value_factory \
          (VALUETYPE::_tao_obv_static_repository_id (), \
           factory); \
        if (prev_factory) prev_factory->_remove_ref (); \
        factory->_remove_ref ();      }

namespace CIAO
{
  /**
   * The initialize routine for any clients that need to access
   * component.  This routine now simply registers various valuetype
   * factories defined in CIAO_Component.pidl.  Currently, this method
   * should be call right after ORB initialization but we should try
   * to register these stuff automatically.
   */
  CIAO_CLIENT_Export int CIAO_client_init (CORBA::ORB_ptr o);
};
