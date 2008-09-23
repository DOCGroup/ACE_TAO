// -*- C++ -*-

// $Id$

#include <tao/IFR_Client_Adapter.h>
#include <tao/IFR_Client/IFR_BasicC.h>
#include <tao/corbafwd.h>
#include <tao/ORB_Core.h>
#include <tao/CORBA_String.h>
#include <tao/AnyTypeCode/Any_Unknown_IDL_Type.h>
#include <ace/OS_NS_string.h>

int
main (int argc, char ** argv)
{
  //init IFR objref

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  TAO_IFR_Client_Adapter *ifr_client = 
     ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance(TAO_ORB_Core::ifr_client_adapter_name());

  if (ifr_client == 0)
    throw ::CORBA::INTF_REPOS();

  ACE_DEBUG ((LM_DEBUG,"Got IFR_Client ref.\n"));

  CORBA::InterfaceDef_var intDef = ifr_client->get_interface(orb,"IDL:IFR_Test/test_if:1.0");

  if (CORBA::is_nil (intDef.in()))
    ACE_ERROR_RETURN ((LM_ERROR,"get interface returned nil ref\n"), 1);

  
  CORBA::ContainedSeq_var attributes =
    intDef->contents (CORBA::dk_Attribute,1);
  CORBA::ULong n_ats = attributes->length();

  CORBA::String_var name = (*attributes)[0]->name();
  ACE_DEBUG ((LM_DEBUG,"found %d attributes, name = %s\n", n_ats, name.in()));

  CORBA::AttributeDef_var attr =
    CORBA::AttributeDef::_narrow ((*attributes)[0]);

  if (CORBA::is_nil (attr.in()))
    ACE_ERROR_RETURN ((LM_ERROR,"could not narrow attribute ref\n"), 1);

  CORBA::TypeCode_var tc = attr->type();

  ACE_DEBUG ((LM_DEBUG,"foo attr typecode = %s\n", tc->id()));

  return 0;
}
