// -*- C++ -*-

// $Id$

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB_Core.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  //init IFR objref
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      TAO_IFR_Client_Adapter *ifr_client =
        ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
          TAO_ORB_Core::ifr_client_adapter_name ());

      if (ifr_client == 0)
        {
          throw ::CORBA::INTF_REPOS ();
        }

      ACE_DEBUG ((LM_DEBUG, "Got IFR_Client ref.\n"));

      CORBA::InterfaceDef_var intDef =
        ifr_client->get_interface (orb.in (), "IDL:IFR_Test/test_if:1.0");

      if (CORBA::is_nil (intDef.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "get interface returned nil ref\n"),
                            1);
        }

      CORBA::ContainedSeq_var attributes =
        intDef->contents (CORBA::dk_Attribute, 1);
      CORBA::ULong n_ats = attributes->length ();

      CORBA::ULong index = 0UL;
      CORBA::String_var name = attributes[index]->name ();
      ACE_DEBUG ((LM_DEBUG,
                  "found %d attributes, name = %s\n",
                  n_ats,
                  name.in ()));

      CORBA::AttributeDef_var attr =
        CORBA::AttributeDef::_narrow (attributes[index]);

      if (CORBA::is_nil (attr.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "could not narrow attribute ref\n"),
                            1);
        }

      CORBA::TypeCode_var tc = attr->type ();

      ACE_DEBUG ((LM_DEBUG, "foo attr typecode = %s\n", tc->id ()));
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      return 1;
    }

  return 0;
}
