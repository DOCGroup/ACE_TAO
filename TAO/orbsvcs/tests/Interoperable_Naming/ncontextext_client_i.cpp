
//=============================================================================
/**
 *  @file    ncontextext_client_i.cpp
 *
 *  This class implements a simple CORBA client which
 *  converts a Name to a string and viceversa, forms a IIOPNAME
 *  url address and can resolve a stringified name.
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================

#include "ncontextext_client_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_time.h"

// Parses the command line arguments and returns an
// error status
int
NContextExt_Client_i::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("dvs"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        ++TAO_debug_level;
        break;
      case 'v':  // output needed
        this->print_values_ = true;
        break;
      case 's':
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-d] [-v]\n"),
                           argv[0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

CORBA::String_var
NContextExt_Client_i::get_name ()
{
  // Use time to produce the seed.
  ACE_OS::srand (static_cast<u_int> (ACE_OS::time (nullptr)));

  constexpr int len {10};
  CORBA::String_var name_component {CORBA::string_alloc (len)};
  // Borrowed from name_component, which owns the allocated buffer.
  char * const name_component_ptr = name_component.inout ();

  for (int i = 0; i < len; ++i)
    {
      int const rand_value = ACE_OS::rand () % 10;

      switch (rand_value)
        {
        case 0:
          name_component_ptr[i] = '/';
          break;

        case 1:
          name_component_ptr[i] = '.';
          break;

        case 2:
          name_component_ptr[i] = '\\';
          break;

        case 3:
          name_component_ptr[i] = '<';
          break;

        case 4:
          name_component_ptr[i] = '>';
          break;

        case 5:
          name_component_ptr[i] = ' ';
          break;

        case 6:
          name_component_ptr[i] = '%';
          break;

        case 7:
        case 8:
        case 9:
          name_component_ptr[i] = static_cast<char> ('A' + (ACE_OS::rand () % 26));
          break;

        default:
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("should not come here")));
          break;
        }
    }

  name_component_ptr[len] = '\0';
  return name_component;
}

int
NContextExt_Client_i::run ()
{
  try
    {
      CosNaming::Name name {};

      name.length (2);
      CORBA::String_var const id = this->get_name ();
      name[0].id = CORBA::string_dup (id.in ());
      CORBA::String_var const kind = this->get_name ();
      name[0].kind = CORBA::string_dup (kind.in ());
      name[1].id = CORBA::string_dup ("Iterator_Factory");
      name[1].kind = CORBA::string_dup ("factory");

      // Get the stringified form of the name
      CORBA::String_var const str_name =
        this->naming_context_->to_string (name);

      CORBA::Object_var factory_object {};

      try
        {
          // Resolve the name using the stringified form of the name
          factory_object =
            this->naming_context_->resolve_str (str_name.in ());
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
        }

      // Narrow
      Web_Server::Iterator_Factory_var const factory =
        Web_Server::Iterator_Factory::_narrow (factory_object.in ());

      // Create bindings
      CosNaming::BindingIterator_var iter {};
      CosNaming::BindingList_var bindings_list {};

      this->naming_context_->list (2,
                                   bindings_list.out (),
                                   iter.out ());

      // Convert the stringified name back as CosNaming::Name and print
      // them out.
      CosNaming::Name_var const nam =
        this->naming_context_->to_name (str_name.in ());

      // Keep the converted name for printing.
      CosNaming::Name const nm = nam.in ();

      // Test the to_url function:
      // For the address, we are assigning
      // some random address now. But, in real applications, the address
      // denotes the address of the NamingContext possibly returned from
      // the LocateReply or LOCATION_FORWARD reply by an agent listening
      // at that address
      //
      char const * const address = ":myhost.555xyz.com:9999";

      // Since we are just testing the functionality of the to_url
      // function, use a random object name.
      CORBA::String_var const obj_name = this->get_name ();

      CORBA::String_var const url_string =
        this->naming_context_->to_url (address, obj_name.in ());

      if (this->print_values_)
        {
          this->print_values (name,
                              str_name,
                              nm,
                              obj_name,
                              url_string);
        }
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("A system exception on client side");
      return -1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }

  return 0;
}

int
NContextExt_Client_i::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // There must be at least one argument, the file that has to be
      // retrieved
      if (this->parse_args (argc, argv) == -1)
        return 1;

      // Get a reference to the Naming Service
      CORBA::Object_var const naming_context_object =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_context_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot resolve Naming Service\n")),
                          1);

      // Narrow to get the correct reference
      this->naming_context_ =
        CosNaming::NamingContextExt::_narrow (naming_context_object.in ());

      if (CORBA::is_nil (this->naming_context_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot narrow Naming Service\n")),
                          1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
      return 1;
    }

  return 0;
}

void
NContextExt_Client_i::print_values (CosNaming::Name const &name,
                                    CORBA::String_var const &str_name,
                                    CosNaming::Name const &nm,
                                    CORBA::String_var const &obj_name,
                                    CORBA::String_var const &url_string)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The first component id is %C,")
             ACE_TEXT ("The first component kind is %C,")
             ACE_TEXT ("The second component id is %C,")
             ACE_TEXT ("The second component kind is %C\n\n"),
             name[0].id.in (),
             name[0].kind.in (),
             name[1].id.in (),
             name[1].kind.in ()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("The string form of the input name is: \n%C\n\n"),
              str_name.in ()));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The unstringified version of the name components are:,")
              ACE_TEXT ("The first component id is %C,")
              ACE_TEXT ("The first component kind is %C,")
              ACE_TEXT ("The second component id is %C,")
              ACE_TEXT ("The second component kind is %C\n\n"),
              nm[0].id.in (),
              nm[0].kind.in (),
              nm[1].id.in (),
              nm[1].kind.in ()));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("When the address of the NamingContext is:")
              ACE_TEXT ("myhost.555xyz.com:9999")
              ACE_TEXT ("and the Object name is \n%C\n"),
              obj_name.in ()));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The URL form of the string is \n %C\n"),
              url_string.in ()));
}
