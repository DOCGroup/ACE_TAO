//==================================================================
/**
 *  @file  RUK_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "IAD_Handler.h"
#include "RUK_Handler.h"
#include "Utils.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    /*
     * Class ResourceUsageKind_Handler
     */

     void RUK_Handler::process_ResourceUsageKind
        (DOMNodeIterator *iter, ::Deployment::ResourceUsageKind &kind)
     {
       // -- ResourceUsageKind enum
       XStr none                    ("None");
       XStr instance_uses_resource  ("InstanceUsesResource");
       XStr resource_uses_instance  ("ResourceUsesInstance");
       XStr port_uses_resource      ("PortUsesResource");
       XStr resource_uses_port      ("ResourceUsesPort");
       
       XStr kind_str = XStr (Utils::parse_string (iter));
       if (kind_str == none)
	 kind = Deployment::None;
       else if (kind_str = instance_uses_resource)
	 kind = Deployment::InstanceUsesResource;
       else if (kind_str == resource_uses_instance)
	 kind = Deployment::ResourceUsesInstance;
       else if (kind_str == port_uses_resource)
	 kind = Deployment::PortUsesResource;
       else if (kind_str == resource_uses_port)
	 kind = Deployment::ResourceUsesPort;
       
       // Something wrong here.. Throw exception
       ACE_DEBUG ((LM_DEBUG,
		   "Config_Handler::RUK_Handler::process_ResourceUsageKind \
                    illegal <ResourceUsageKind> value found \n"));
       ACE_THROW (CORBA::INTERNAL ());
     }
  }
}
