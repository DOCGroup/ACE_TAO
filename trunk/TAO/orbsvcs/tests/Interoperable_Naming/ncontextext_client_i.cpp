// $Id$
//

// ===========================================================
//
// = LIBRARY
//    TAO/ORBSVCS/tests/SimpleNaming
//
// = FILENAME
//    ncontextext_client_i.cpp
//
// = DESCRIPTION
//    This class implements a simple CORBA client which
//    converts a Name to a string and viceversa, forms a IIOPNAME
//    url address and can resolve a stringified name.
//
// = AUTHORS
//    Priyanka Gontla <pgontla@ece.uci.edu>
//
//============================================================

#include "ncontextext_client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// Constructor
NContextExt_Client_i::NContextExt_Client_i (void)
{
}

NContextExt_Client_i::~NContextExt_Client_i (void)
{
}


// Parses the command line arguments and returns an
// error status
int
NContextExt_Client_i::parse_args (void)
{

  ACE_Get_Opt get_opts (argc_, argv_, "dvs");
  int c;

  this->view_ = 1;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'v':  // output needed
        this->view_ = 0;
        break;
      case 's':
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-v]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

char *
NContextExt_Client_i::get_name ()
{

  // @@ You should use the ACE_OS version of these functions...
  // @@ For randomized tests it it important to print the seed,
  //    should the test fail you want to run it again with the same
  //    seed.  The canonical trick is to receive the seed as a command
  //    line argument, if the argument is not there you initialize it
  //    using time() or something like that (ACE_OS::gethrtime??)
  // USe time (NULL) to produce the seed:
  srand (time (0));

  const int len = 10;
  char *name_component = CORBA::string_alloc (len);
  char *name_componentPtr = name_component;


  for (int i = 0; i < len; ++i)
    {

      int rand_value = rand () % 10;

      switch (rand_value)
        {
        case 0:
          *name_componentPtr = '/';
          ++name_componentPtr;
          break;

        case 1:
          *name_componentPtr = '.';
          ++name_componentPtr;
          break;

        case 2:
          *name_componentPtr = '\\';
          ++name_componentPtr;
          break;

        case 3:
          *name_componentPtr = '<';
          ++name_componentPtr;
          break;

        case 4:
          *name_componentPtr = '>';
          ++name_componentPtr;
          break;

        case 5:
          *name_componentPtr = ' ';
          ++name_componentPtr;
          break;

        case 6:
          *name_componentPtr = '%';
          ++name_componentPtr;
          break;

        case 7:
        case 8:
        case 9:
          *name_componentPtr = 'A' + ( rand () % 26 );
          ++name_componentPtr;
          break;

        default:
          cout << "shouldnt come here" << endl;
          break;
        }
    }

  *name_componentPtr = '\0';

  return name_component;

}

int
NContextExt_Client_i::run (CORBA::Environment &ACE_TRY_ENV)
{

  ACE_TRY
    {
      CosNaming::Name name;

      name.length (2);
      name[0].id = CORBA::string_dup (this->get_name ());
      name[0].kind = CORBA::string_dup (this->get_name ());
      name[1].id = CORBA::string_dup ("Iterator_Factory");
      name[1].kind = CORBA::string_dup ("factory");

      // Get the stringified form of the name
       CORBA::String_var str_name =
        this->naming_context_->to_string (name,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Resolve the name using the stringified form of the name
      CORBA::Object_var factory_object =
        this->naming_context_->resolve_str (str_name.in ());

      // Narrow
      Web_Server::Iterator_Factory_var factory =
        Web_Server::Iterator_Factory::_narrow (factory_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create bindings
      CosNaming::BindingIterator_var iter;
      CosNaming::BindingList_var bindings_list;

      this->naming_context_->list (2,
                                   bindings_list.out (),
                                   iter.out (),
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Convert the stringified name back as CosNaming::Name and print
      // them out.
      CosNaming::Name *nam =
        this->naming_context_->to_name (str_name.in (),
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Declare a CosNaming::Name variable and assign length to it.
      CosNaming::Name nm;
      nm.length (nam->length ());

      nm = *nam;

      // Test the to_url function:
      // For the address, we are assigning
      // some random address now. But, in real applications, the address
      // denotes the address of the NamingContext possibly returned from
      // the LocateReply or LOCATION_FORWARD reply by an agent listening
      // at that address
      //
      CORBA::String_var address = "myhost.555xyz.com:9999";

      // Since we are just testing the functionality of the to_url
      // function, use a random object name.
      //
      CORBA::String_var obj_name = get_name ();

      CORBA::String_var url_string = this->naming_context_->to_url (address.in (),
                                                                    obj_name.in(),
                                                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (this->view_ == 0)
        {
          this->print_values (name,
                              str_name,
                              nm,
                              obj_name,
                              url_string);
        }
    }
  ACE_CATCH (CORBA::NO_MEMORY, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "A system exception oc client side");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "A system exception oc client side");
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
NContextExt_Client_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (this->argc_,
                         this->argv_,
                         "" /* the ORB name, it can be anything! */,
                         ACE_TRY_ENV);

      // There must be at least one argument, the file that has to be
      // retrieved
      if (this->parse_args () == -1)
        return 1;

      // Get a reference to the Naming Service
      CORBA::Object_var naming_context_object =
        orb->resolve_initial_references ("NameService", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_context_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service\n"),
                          1);

      // Narrow to get the correct reference
      this->naming_context_ =
        CosNaming::NamingContextExt::_narrow (naming_context_object.in (),
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->naming_context_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot narrow Naming Service\n"),
                          1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

void
NContextExt_Client_i::print_values (CosNaming::Name name,
                                    CORBA::String_var str_name,
                                    CosNaming::Name nm,
                                    CORBA::String_var obj_name,
                                    CORBA::String_var url_string)
{
  
  ACE_DEBUG((LM_DEBUG, "The first component id is %s,
             The first component kind is %s,
             The second component id is %s,
             The second component kind is %s\n",
             name[0].id.in (),
             name[0].kind.in (),
             name[1].id.in (),
             name[1].kind.in ()));

  ACE_DEBUG ((LM_DEBUG, "The string form of the input name is:\n %s\n",
              str_name.in ()));

  ACE_DEBUG ((LM_DEBUG, "The unstringified version of the name components are:,
              The first component id is %s,
              The first component kind is %s,
                  The second component id is %s,
                  The second component kind is %s\n",
              nm[0].id.in (),
              nm[0].kind.in (),
              nm[1].id.in (),
              nm[1].kind.in ()));
  
 ACE_DEBUG ((LM_DEBUG, "When the address of the NamingCOntext is:"
             "myhost.555xyz.com:9999"
             "\nand the Object name is %s\n",
             obj_name.in ()));

 ACE_DEBUG ((LM_DEBUG,"The URL form of the string is \n %s\n",
             url_string.in ()));
            
}
