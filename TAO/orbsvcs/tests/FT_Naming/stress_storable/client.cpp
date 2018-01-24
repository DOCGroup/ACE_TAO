#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Time_Value.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/FT_NamingManagerC.h"
#include "orbsvcs/Naming/FaultTolerant/nsgroup_svc.h"
#include "ace/Task.h"


const ACE_TCHAR *group_names[] =
  {ACE_TEXT("C++ Group"), ACE_TEXT("FORTRAN Group"), ACE_TEXT("Groovy Group"), ACE_TEXT("Basic Group"),
   ACE_TEXT("Java Group"), ACE_TEXT("Forth Group"), ACE_TEXT("COBOL Group"), ACE_TEXT("Ruby Group"),
   ACE_TEXT("wrong answer dude"), ACE_TEXT("Scala Group"), ACE_TEXT("R Group"), 0};

class Hammer : public ACE_Task_Base
{
  NS_group_svc group_svc;

public:
  Hammer ()
    : group_svc(true)
  {}

  int setup (CORBA::ORB_ptr orb, FT_Naming::NamingManager_ptr nm)
  {
    if (group_svc.set_orb (orb) != 0 || group_svc.set_naming_manager (nm))
      {
        ACE_DEBUG ((LM_DEBUG, "Hammer setup failed\n"));
        return -1;
      }
    return 0;
  }

  int svc (void)
  {
    for (int g = 0, i = 0; i < 100; i++)
      {
        try {
          group_svc.member_list (group_names[g]);
        }
        catch (PortableGroup::ObjectGroupNotFound) {
          }
        catch (CORBA::Exception &ex) {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Hammer thread %t caught %C accessing %C\n"),
                      ex._name(), group_names[g]));
        }
        if (group_names[++g] == 0)
          g = 0;
      }

    return 0;
  }

};

const ACE_TCHAR *ior = ACE_TEXT("file://obj.ior");
int hammers = 10;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        hammers = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-k <ior> ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) - Acquiring Name Service\n")));


      CORBA::Object_var nmobj = orb->resolve_initial_references ("NameManager");
      FT_Naming::NamingManager_var naming_manager =
        FT_Naming::NamingManager::_narrow (nmobj.in());

      TAO_Naming_Client name_svc;
      try {

        ACE_Time_Value timeout (10); // Wait up to 10 seconds for the naming service
        if (name_svc.init (orb.in (), &timeout) != 0)
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("client: Could not connect to ")
                             ACE_TEXT ("naming service.\n")),
                            1);
      }
      catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception (
          ACE_TEXT ("Exception caught while initializing name ")
          ACE_TEXT ("service facade:"));
        return 1;
      }


      Hammer hammer;

      if (hammer.setup(orb.in(), naming_manager.in()) == -1)
        {
          return 1;
        }

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("basic_name");

      CORBA::Object_var tmp;
      Test::Basic_var basic;

      // Iterate enough so we get a few wrap-arounds
      for (int i = 0; i < 15; i++)
      {
        if (i == 3) {
          hammer.activate (THR_NEW_LWP | THR_JOINABLE, hammers);
        }

        try {

          // Each time we invoke resolve, we get a different member
          tmp =
            name_svc->resolve (name);

          // Narrow it to a Basic object
          basic =
            Test::Basic::_narrow (tmp.in ());

        }
        catch (CORBA::Exception& ex)
          {
            ex._tao_print_exception (ACE_TEXT ("Error resolving name.\n"));
          }

        if (CORBA::is_nil (basic.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               ACE_TEXT ("Server obj ref not obtained ")
                               ACE_TEXT ("from Load Balancing Name Service\n"),
                               ior),
                              1);
          }

        try {
          CORBA::String_var the_string =
            basic->get_string ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - Client request handled ")
                      ACE_TEXT ("by object at <%C>\n"),
                      the_string.in ()));

        }
        catch (CORBA::Exception& ex)
          {
            ex._tao_print_exception (
              ACE_TEXT ("Error invoking get_string on Basic object.\n"));
            return 1;
          }

        // Remove one member after we wrapped around to make sure naming manager can
        // handle it successufully
        if (i == 7)
          {
            try {
              // Try removing a member
              basic->remove_member ();
            }
            catch (CORBA::Exception& ex)
              {
                ex._tao_print_exception (
                  ACE_TEXT ("Error invoking get_string on Basic object.\n"));
                return 1;
              }
          }
      }

      hammer.wait ();

      try {

        basic->shutdown ();

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) - Shutting down server\n")));
      }
      catch (CORBA::Exception& ex)
      {
        ex._tao_print_exception (
          ACE_TEXT ("Error invoking get_string on Basic object.\n"));
        return 1;
      }
      orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("Exception caught in client.cpp:"));
    return 1;
  }

  return 0;
}
