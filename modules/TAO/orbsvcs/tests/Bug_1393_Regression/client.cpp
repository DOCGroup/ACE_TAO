// $Id$

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb_->resolve_initial_references ("InterfaceRepository");

      CORBA::Repository_var repo_ = CORBA::Repository::_narrow (object.in ());


      CORBA::ContainedSeq_var interfaces =
                  repo_->contents (CORBA::dk_Interface,    // Any type of contained object.
                                   1                       // Exclude parents of interfaces.
                                   );

      CORBA::ULong length = interfaces->length ();

      if (length != 1)
      {
        ACE_DEBUG ((LM_DEBUG, "CLIENT (%P): Incorrect number of elements in IFR - length is %d\n", length));
        return -1;
      }

      CORBA::ULong first_one = 0;

      CORBA::InterfaceDef_var the_interface =
            CORBA::InterfaceDef::_narrow (interfaces[first_one]);

      CORBA::String_var name = the_interface->name();

      if (ACE_OS::strcmp (name.in(), "int"))
      {
        ACE_DEBUG ((LM_DEBUG, "CLIENT (%P): Interface name has been munged - it is now %C\n",
                      name.in()));
        return -1;
      }


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception ...");
      return -1;
    }
  return 0;
}
