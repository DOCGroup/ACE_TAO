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

#include "ace/Log_Msg.h"

template<class Interface> typename RIR_Narrow<Interface>::Interface_ptr
RIR_Narrow<Interface>::resolve (CORBA::ORB_ptr orb,
                                const char *object_id)
{
  Interface_var intface;

  try
    {
      CORBA::Object_var object =
        orb->resolve_initial_references (object_id);

      intface = Interface::_narrow (object.in ());

      if (CORBA::is_nil (intface.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Panic - error while narrowing <%C>\n",
                      object_id));
          ACE_OS::exit (1);
        }
    } catch (const CORBA::Exception& ex) {
      ex._tao_print_exception ("Caught an exception\n");
    }
  return intface._retn ();
}

#endif /* TAO_PERF_RTEC_RIR_NARROW_CPP */
