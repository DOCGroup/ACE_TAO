// $Id$

#include "orbsvcs/AV/default_resource.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"

TAO_AV_Default_Resource_Factory::TAO_AV_Default_Resource_Factory (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"TAO_AV_Default_Resource_Factory::TAO_AV_Default_Resource_Factory %d\n"));
}

TAO_AV_Default_Resource_Factory::~TAO_AV_Default_Resource_Factory (void)
{
  TAO_AV_TransportFactorySetItor t_end = TAO_AV_CORE::instance ()->transport_factories ()->end (); 
  for (TAO_AV_TransportFactorySetItor tfiterator =
         TAO_AV_CORE::instance ()->transport_factories ()->begin ();
       tfiterator != t_end;
       ++tfiterator)
    delete *tfiterator;
  
  TAO_AV_CORE::instance ()->transport_factories ()->reset ();

  TAO_AV_Flow_ProtocolFactorySetItor fp_end = TAO_AV_CORE::instance ()->flow_protocol_factories ()->end (); 
  for (TAO_AV_Flow_ProtocolFactorySetItor pfiterator =
         TAO_AV_CORE::instance ()->flow_protocol_factories ()->begin ();
       pfiterator != fp_end;
       ++pfiterator)
    delete *pfiterator;
  
  TAO_AV_CORE::instance ()->flow_protocol_factories ()->reset ();

  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
	      "TAO_AV_Default_Resource_Factory::~TAO_AV_Default_Resource_Factory\n"));
}

int
TAO_AV_Default_Resource_Factory::init (int argc, char *argv[])
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
	      "TAO_AV_Default_Resource_Factory::init\n"));
  
  int curarg = 0;
  
  for (curarg = 0; curarg < argc; ++curarg)
    {
      if (ACE_OS::strcasecmp (argv[curarg],
			      "-AVTransportFactory") == 0)
	{
	  TAO_AV_TransportFactorySet *tset = TAO_AV_CORE::instance ()->transport_factories ();
	  curarg++;
	  if (curarg < argc)
	    {
	      TAO_AV_Transport_Item *item = 0;
	      ACE_NEW_RETURN (item,
			      TAO_AV_Transport_Item (argv[curarg]),
			      -1);
	      if (tset->insert (item) == -1)
		ACE_ERROR ((LM_ERROR,
			    ACE_TEXT ("(%P|%t) Unable to add transport factories for %s: %p\n"),
			    argv[curarg]));
	    }
	}
      else if (ACE_OS::strcasecmp (argv[curarg],
				   "-AVFlowProtocolFactory") == 0)
	{
	  TAO_AV_Flow_ProtocolFactorySet *fpset = TAO_AV_CORE::instance ()->flow_protocol_factories ();
	  curarg++;
	  if (curarg < argc)
	    {
	      TAO_AV_Flow_Protocol_Item *item = 0;
	      ACE_NEW_RETURN (item,
			      TAO_AV_Flow_Protocol_Item (argv[curarg]),
			      -1);
	      if (fpset->insert (item) == -1)
		ACE_ERROR ((LM_ERROR,
			    ACE_TEXT ("(%P|%t) Unable to add flow protocol factories for %s: %p\n"),
			    argv[curarg]));
	    }
	}
      
    }
  return 0;
}


// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_AV_Default_Resource_Factory,
                       ACE_TEXT ("AV_Default_Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_Default_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_Default_Resource_Factory)

// ****************************************************************








