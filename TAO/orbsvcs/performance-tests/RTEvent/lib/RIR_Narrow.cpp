/**
 * @file RIR_Narrow.cpp
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_RIR_NARROW_CPP
#define TAO_PERF_RTEC_RIR_NARROW_CPP

#include "RIR_Narrow.h"
#include "tao/Environment.h"

template<class Interface> ACE_TYPENAME RIR_Narrow<Interface>::Interface_ptr
RIR_Narrow<Interface>::resolve (CORBA::ORB_ptr orb,
                                const char *object_id
                                ACE_ENV_ARG_DECL)
{
  Interface_var interface;

  ACE_TRY
    {
      CORBA::Object_var object =
        orb->resolve_initial_references (object_id
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      interface = Interface::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (interface.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Panic - error while narrowing <%s>\n",
                      object_id));
          ACE_OS::exit (1);
        }
    }ACE_CATCHANY{
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Caught an exception \n");
    }
  ACE_ENDTRY;
  return interface._retn ();
}

#endif /* TAO_PERF_RTEC_RIR_NARROW_CPP */
