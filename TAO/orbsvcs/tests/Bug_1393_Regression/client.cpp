#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include <cstring>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const object =
        orb_->resolve_initial_references ("InterfaceRepository");

      CORBA::Repository_var const repo_ = CORBA::Repository::_narrow (object.in ());
      CORBA::ContainedSeq_var interfaces =
        repo_->contents (CORBA::dk_Interface, // Any type of contained object.
                         1); // Exclude parents of interfaces.

      CORBA::ULong const length = interfaces->length ();

      if (length != 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "CLIENT (%P): Incorrect number of elements in IFR - length is %d\n",
                      length));
          return -1;
        }

      CORBA::InterfaceDef_var const the_interface =
        CORBA::InterfaceDef::_narrow (interfaces[0]);
      CORBA::String_var const name = the_interface->name ();

      if (std::strcmp (name.in (), "int") != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "CLIENT (%P): Interface name has been munged - it is now %C\n",
                      name.in ()));
          return -1;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception ...");
      return -1;
    }
  return 0;
}
