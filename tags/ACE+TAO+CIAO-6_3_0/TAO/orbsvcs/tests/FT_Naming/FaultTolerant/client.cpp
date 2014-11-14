//=============================================================================
/**
 *  @file client.cpp
 *
 *  $Id$
 *
 *  This class implements a CORBA test client for the Fault Tolerant Naming Service.
 *
 *  @author Rich Seibel <seibel_r@ociweb.com>
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//=============================================================================

#include "TestC.h"
#include "test_objectS.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FT_NamingManagerC.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "tao/debug.h"
#include "tao/Stub.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "orbsvcs/Naming/FaultTolerant/nsgroup_svc.h"

#include <iostream>

#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

//==============================================================================
//
//==============================================================================
class My_Test_Object :
  public virtual POA_Test_Object
{
public:
  // = Initialization and termination methods.
  My_Test_Object (CORBA::Short id = 0);
  // Constructor.

  ~My_Test_Object (void);
  // Destructor.

  // = Interface implementation accessor methods.

  void id (CORBA::Short id);
  // Sets id.

  CORBA::Short id (void);
  // Gets id.

private:
  short id_;
};

My_Test_Object::My_Test_Object (CORBA::Short id)
  : id_ (id)
{
}

My_Test_Object::~My_Test_Object (void)
{
}

CORBA::Short
My_Test_Object::id (void)
{
  return id_;
}

void
My_Test_Object::id (CORBA::Short id)
{
  id_ = id;
}

//==============================================================================
//
//==============================================================================
/// Failover Name Test
int
do_failover_name_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *ns1ref,
  int c_breadth,
  int c_depth,
  int o_breadth );

/// Failover ObjectGroup Test
int
do_failover_objectgroup_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *nm1ref,
  int c_breadth,
  int c_depth,
  int o_breadth );

//==============================================================================
//
//==============================================================================
/// Persistence Name Test
int
do_persistence_name_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *ns1ref,
  int c_breadth,
  int c_depth,
  int o_breadth,
  bool validate_only);

/// Persistence ObjectGroup Test
int
do_persistence_objectgroup_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *nm1ref,
  int c_breadth,
  int c_depth,
  int o_breadth,
  bool validate_only);

//==============================================================================
//
//==============================================================================
/// Equivalence Name Test
int
do_equivalence_name_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *ns1ref,
  ACE_TCHAR *ns2ref,
  int c_breadth,
  int c_depth,
  int o_breadth);

/// Equivalence ObjectGroup Test
int
do_equivalence_objectgroup_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *nm1ref,
  ACE_TCHAR *nm2ref,
  int c_breadth,
  int c_depth,
  int o_breadth);


//==============================================================================
//
//==============================================================================
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  const int RC_SUCCESS =  0;
  const int RC_ERROR   =  1;

  int rc = RC_SUCCESS;

  int c_breadth = 4;
  int c_depth   = 4;
  int o_breadth = 4;

  ACE_TCHAR *ns1ref = 0;
  ACE_TCHAR *ns2ref = 0;

  ACE_TCHAR *nm1ref = 0;
  ACE_TCHAR *nm2ref = 0;

  typedef enum {
    TT_FAILOVER,
    TT_PERSISTENCE,
    TT_EQUIVALENCE
  } fault_tolerant_test;
  fault_tolerant_test test_type = TT_FAILOVER;

  typedef enum {
    TT_INIT,
    TT_CREATE,
    TT_VALIDATE
  } fault_tolerant_test_phase;
  fault_tolerant_test_phase test_phase = TT_INIT;

  //////////////////////////////////////////////////////////////////////////////
  // optional
  //////////////////////////////////////////////////////////////////////////////
  // -b <breadth of context tree>
  // -d <depth of context tree>
  // -o <breadth of object tree>
  //////////////////////////////////////////////////////////////////////////////
  // required
  //////////////////////////////////////////////////////////////////////////////
  // -p <ior of first name server>
  // -q <ior of second name server>
  // -r <ior of first naming service>
  // -s <ior of second naming service>
  // --failover run fault tolerant failover test
  // --persistence run fault tolerant persistence test
  // --equivalence run fault tolerant equivalence test
  //////////////////////////////////////////////////////////////////////////////
  // required with persistance
  //////////////////////////////////////////////////////////////////////////////
  // --create run creation test phase
  // --validate run validation test phase

  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("b:d:o:p:q:r:s:"));
    int c;
    int i;
    get_opts.long_option (ACE_TEXT ("failover"), ACE_Get_Opt::NO_ARG);
    get_opts.long_option (ACE_TEXT ("persistence"), ACE_Get_Opt::NO_ARG);
    get_opts.long_option (ACE_TEXT ("equivalence"), ACE_Get_Opt::NO_ARG);
    get_opts.long_option (ACE_TEXT ("create"), ACE_Get_Opt::NO_ARG);
    get_opts.long_option (ACE_TEXT ("validate"), ACE_Get_Opt::NO_ARG);

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'b':
          i = ACE_OS::atoi(get_opts.opt_arg ());
          if (i<2)
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT ("Invalid breadth, must be 2 or more\n")));
            ACE_OS::exit(RC_ERROR);
          }
          c_breadth = i;
          break;
        case 'd':
          i = ACE_OS::atoi(get_opts.opt_arg ());
          if (i<2)
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT ("Invalid depth, must be 2 or more\n")));
            ACE_OS::exit(RC_ERROR);
          }
          c_depth = i;
          break;
        case 'o':
          i = ACE_OS::atoi(get_opts.opt_arg ());
          if (i<2)
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT ("Invalid breadth, must be 2 or more\n")));
            ACE_OS::exit(RC_ERROR);
          }
          o_breadth = i;
          break;
        case 'p':
          ns1ref = get_opts.opt_arg ();
          break;
        case 'q':
          ns2ref = get_opts.opt_arg ();
          break;
        case 'r':
          nm1ref = get_opts.opt_arg ();
          break;
        case 's':
          nm2ref = get_opts.opt_arg ();
          break;
        case 0:   // A long option was found
          {
            const ACE_TCHAR * long_option = get_opts.long_option ();
            if (ACE_OS::strcmp (long_option,
                                ACE_TEXT ("failover")) == 0)
              {
                test_type = TT_FAILOVER;
              }
            else if (ACE_OS::strcmp (long_option,
                                     ACE_TEXT ("persistence")) == 0)
              {
                test_type = TT_PERSISTENCE;
              }
            else if (ACE_OS::strcmp (long_option,
                                     ACE_TEXT ("equivalence")) == 0)
              {
                test_type = TT_EQUIVALENCE;
              }
            else if (ACE_OS::strcmp (long_option,
                                     ACE_TEXT ("create")) == 0)
              {
                test_phase = TT_CREATE;
              }
            else if (ACE_OS::strcmp (long_option,
                                     ACE_TEXT ("validate")) == 0)
              {
                test_phase = TT_VALIDATE;
              }
          }
          break;

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Argument %c \n usage:  %s")
                             ACE_TEXT (" [-b <breadth of context tree>]")
                             ACE_TEXT (" [-d <depth of context tree>]")
                             ACE_TEXT (" [-o <breadth of object tree>]")
                             ACE_TEXT (" -p <ior of first name server>")
                             ACE_TEXT (" -q <ior of second name server>")
                             ACE_TEXT (" -r <ior of first naming server>")
                             ACE_TEXT (" -s <ior of second naming server>")
                             ACE_TEXT ("\n")),
                             RC_ERROR);
        }

    switch(test_type) {

      case TT_FAILOVER:

        if( RC_SUCCESS != do_failover_name_test (
            orb.in (),
            ns1ref,
            c_breadth,
            c_depth,
            o_breadth ))
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("ERROR: Failover Name Test Failed\n")));
          rc = RC_ERROR;
        } else {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("INFO: Failover Name Test OK\n")));
        }

        if( RC_SUCCESS != do_failover_objectgroup_test (
            orb.in (),
            nm1ref,
            c_breadth,
            c_depth,
            o_breadth ))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ERROR: Failover ObjectGroup ")
                      ACE_TEXT ("Test Failed\n")));
          rc = RC_ERROR;
        } else {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("INFO: Failover ObjectGroup Test OK\n")));
        }

      break;

      case TT_PERSISTENCE:

        switch(test_phase){

          case TT_CREATE:
            if (RC_SUCCESS != do_persistence_name_test (
                orb.in (),
                ns1ref,
                c_breadth,
                c_depth,
                o_breadth,
                false ))
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("ERROR: Persistence Creation Name ")
                          ACE_TEXT ("Test Failed\n")));
              rc = RC_ERROR;
            } else {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("INFO: Persistence Creation Name ")
                          ACE_TEXT ("Test OK\n")));
            }
            if (RC_SUCCESS != do_persistence_objectgroup_test(
              orb.in (),
              nm1ref,
              c_breadth,
              c_depth,
              o_breadth,
              false ))
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("ERROR: Persistence Creation ObjectGroup ")
                          ACE_TEXT ("Test Failed\n")));
              rc = RC_ERROR;
            } else {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("INFO: Persistence Creation ObjectGroup ")
                          ACE_TEXT ("Test OK\n")));
            }
          break;

          case TT_VALIDATE:
            if (RC_SUCCESS != do_persistence_name_test (
                orb.in (),
                ns1ref,
                c_breadth,
                c_depth,
                o_breadth,
                true ))
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("ERROR: Persistence Validation ")
                          ACE_TEXT ("Name Test Failed\n")));
              rc = RC_ERROR;
            } else {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("INFO: Persistence Validation ")
                          ACE_TEXT ("Name Test OK\n")));
            }

            if (RC_SUCCESS != do_persistence_objectgroup_test (
                orb.in (),
                nm1ref,
                c_breadth,
                c_depth,
                o_breadth,
                true ))
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("ERROR: Persistence Validation ")
                          ACE_TEXT ("ObjectGroup Test Failed\n")));
              rc = RC_ERROR;
            } else {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("INFO: Persistence Validation ")
                          ACE_TEXT ("ObjectGroup Test OK\n")));
            }
          break;

          case TT_INIT:
          default:
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("ERROR: Missing Required Persistence ")
                        ACE_TEXT ("Argument [--create|--validate] \n")));
            rc = RC_ERROR;
          break;
        }
      break;

      case TT_EQUIVALENCE:

        if (RC_SUCCESS != do_equivalence_name_test (
            orb.in (),
            ns1ref,
            ns2ref,
            c_breadth,
            c_depth,
            o_breadth ))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ERROR: Equivalence Name Test\n")));
          rc = RC_ERROR;
        } else {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("INFO: Equivalence Name Test OK\n")));
        }

        if (RC_SUCCESS != do_equivalence_objectgroup_test(
            orb.in (),
            nm1ref,
            nm2ref,
            c_breadth,
            c_depth,
            o_breadth ))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ERROR: Equivalence ObjectGroup Test\n")));
          rc = RC_ERROR;
        } else {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("INFO: Equivalence ObjectGroup Test OK\n")));
        }
      break;
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to resolve name servers"));
    return RC_ERROR;
  }


  return rc;

}

//==============================================================================
//
//==============================================================================
/// Failover Name Test
int
do_failover_name_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *ns1ref,
  int c_breadth,
  int c_depth,
  int o_breadth )
{
  const int RC_SUCCESS =  0;
  const int RC_ERROR   = -1;
  CosNaming::NamingContext_var root_context_1;
  int i;

  try {

    CORBA::ORB_var orb = CORBA::ORB::_duplicate(theOrb);

    if (CORBA::is_nil (orb.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (CORBA::is_nil (ns1ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ns1\n")),
                          RC_ERROR);
    }

    CORBA::Object_var ns1obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ns1ref));

    if (CORBA::is_nil (ns1obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns1ref),
                          RC_ERROR);
    }

    root_context_1 = CosNaming::NamingContext::_narrow (ns1obj.in ());

    if (CORBA::is_nil (root_context_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns1ref),
                          RC_ERROR);
    }

  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to resolve name servers"));
    return RC_ERROR;
  }

  // Create a bunch of objects in one context
  // Note: strings to the naming service must be char, not wchar
  try
  {
    // Bind one context level under root.
    CosNaming::Name level1;
    level1.length (1);
    level1[0].id = CORBA::string_dup ("level1_context");
    CosNaming::NamingContext_var level1_context;
    level1_context = root_context_1->bind_new_context (level1);
    ACE_OS::thr_yield ();

    for (i=0; i<o_breadth; i++)
    {
      // Instantiate a dummy object and bind it under the new context.
      My_Test_Object *impl1 = new My_Test_Object (i+1);
      Test_Object_var obj1 = impl1->_this ();

      impl1->_remove_ref ();

      level1.length (2);

      char wide_name[16];
      ACE_OS::sprintf (wide_name, "obj_%d", i);
      level1[1].id = CORBA::string_dup (wide_name);

      root_context_1->bind (level1, obj1.in ());
      ACE_OS::thr_yield ();

      // See if the newly bound object is available in the
      // replica
      try {
        CORBA::Object_var obj1_on_replica = root_context_1->resolve (level1);
      }
      catch (const CosNaming::NamingContext::NotFound& ex)
        {
          ex._tao_print_exception (
            ACE_TEXT ("INFO: Unable to resolve object from replica. Sleeping for a second.\n"));

          // Give it a second to be processed
          ACE_OS::sleep (1);

          // Try again...
          try {

            CORBA::Object_var obj1_on_replica =
              root_context_1->resolve (level1);

            // We did find the object on the replica, but only after a wait.
            // This would be caused by a race condition to access the variable.
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("INFO: Object appeared after a short ")
                        ACE_TEXT ("wait.\n")));
          }
          catch (const CosNaming::NamingContext::NotFound& second_ex)
            {
              second_ex._tao_print_exception (
                ACE_TEXT ("ERROR: It really is not there. Failing...\n"));
              return RC_ERROR;
            }
        }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create a lot of objects"));
    return RC_ERROR;
  }

  // Create a deep context tree
  try
  {
    CosNaming::NamingContext_var next_context = root_context_1;
    for (i=0; i<c_depth; i++)
    {
      // Bind level1 context under root.
      CosNaming::Name deep;
      deep.length (1);
      char deep_name[16];
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[0].id = CORBA::string_dup (deep_name);
      CosNaming::NamingContext_var deep_context;
      deep_context = next_context->bind_new_context (deep);
      ACE_OS::thr_yield ();
      next_context = deep_context;
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create deep context"));
    return RC_ERROR;
  }

  // Create a wide context tree
  try
  {
    for (i=0; i<c_breadth; i++)
    {
      // Bind all level of context under root.
      CosNaming::Name wide;
      wide.length (1);
      char wide_name[16];
      ACE_OS::sprintf (wide_name, "wide_%d", i);
      wide[0].id = CORBA::string_dup (wide_name);
      CosNaming::NamingContext_var wide_context;
      wide_context = root_context_1->bind_new_context (wide);
      ACE_OS::thr_yield ();

      try {
        // Check if the new context is available in the replica
        CORBA::Object_var obj1_on_replica = root_context_1->resolve (wide);
        // Make sure it is a context
        CosNaming::NamingContext_var nc =
          CosNaming::NamingContext::_narrow (obj1_on_replica.in ());
      }
      catch (const CosNaming::NamingContext::NotFound& ex)
        {
          ex._tao_print_exception (
              ACE_TEXT ("INFO: Unable to resolve wide context object from ")
              ACE_TEXT ("replica. Sleeping for a second.\n"));

          ACE_OS::sleep (1);

          // Try again to see if it just was a race condition
          try {
            CORBA::Object_var obj1_on_replica = root_context_1->resolve (wide);
            // We did find the object on the replica, but only after a wait.
            // This would be caused by a race condition to access the variable.
            ACE_ERROR (( LM_ERROR,
                         ACE_TEXT ("INFO: Object appeared after a short ")
                         ACE_TEXT ("wait.\n")));
          }
          catch (const CosNaming::NamingContext::NotFound& second_ex)
            {
              second_ex._tao_print_exception (
                ACE_TEXT ("ERROR: It really is not there. Failing...\n"));
              return RC_ERROR;
            }
        }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create wide context"));
    return RC_ERROR;
  }

  // Delete three selected things, one from each tree
  try {
    // Remove the second to last object from the Naming Context
    CosNaming::Name wide1;
    wide1.length (2);
    wide1[0].id = CORBA::string_dup ("level1_context");
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "obj_%d", o_breadth-2);
    wide1[1].id = CORBA::string_dup (wide_name);
    root_context_1->unbind (wide1);
    ACE_OS::thr_yield ();

    bool retried = false;
    // Make sure it is gone from the replica
    try {
      CORBA::Object_var obj1_on_replica = root_context_1->resolve (wide1);

      // An exception should be thrown by the above or
      // there is an error. This means the replica did
      // not register the loss of the context.
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("INFO: Unbound deep context not removed from ")
                  ACE_TEXT ("replica. Trying again...\n")));
      retried = true;  // Mark this so it can be reported in catch block.

      obj1_on_replica = root_context_1->resolve (wide1);

      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Unbound context not removed from ")
                          ACE_TEXT ("on retry\n")),
                          RC_ERROR);
    }
    catch (const CosNaming::NamingContext::NotFound&)
      {
        // Not on replica --- as it should be.
        if (retried)  // Was found on the retry
          ACE_ERROR (( LM_ERROR,
                       ACE_TEXT ("INFO: Was removed after short wait.\n")));
      }

    // Remove the second to last context from the wide root Naming Context
    CosNaming::Name wide2;
    wide2.length (1);
    ACE_OS::sprintf (wide_name, "wide_%d", c_breadth-2);
    wide2[0].id = CORBA::string_dup (wide_name);

    CORBA::Object_var result_obj_ref = root_context_1->resolve (wide2);

    CosNaming::NamingContext_var result_object =
      CosNaming::NamingContext::_narrow (result_obj_ref.in ());

    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Problems with resolving wide context ")
                         ACE_TEXT ("- nil object ref.\n")),
                         RC_ERROR);
    }

    result_object->destroy();
    root_context_1->unbind (wide2);
    ACE_OS::thr_yield ();

    // Remove the last context from the deep Naming Context
    CosNaming::Name deep;
    deep.length (c_depth);

    char deep_name[16];
    for (i=0; i<c_depth; i++)
    {
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[i].id = CORBA::string_dup (deep_name);
    }

    result_obj_ref = root_context_1->resolve (deep);

    result_object = CosNaming::NamingContext::_narrow (result_obj_ref.in ());
    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Problems with resolving deep context ")
                         ACE_TEXT ("- nil object ref.\n")),
                         RC_ERROR);
    }

    result_object->destroy();
    root_context_1->unbind (deep);
    ACE_OS::thr_yield ();

    retried = false;
    // Make sure it is gone from the replica
    try {
      CORBA::Object_var obj1_on_replica = root_context_1->resolve (deep);

      // An exception should be thrown by the above or
      // there is an error. This means the replica did
      // not register the loss of the context.
      ACE_ERROR (( LM_ERROR,
                   ACE_TEXT ("INFO: Unbound deep context not removed from ")
                   ACE_TEXT ("replica. Trying again...\n")));

      retried = true;  // Mark this so it can be reported in catch block.
      obj1_on_replica = root_context_1->resolve (deep);

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Unbound context not removed from ")
                         ACE_TEXT ("on retry\n")),
                         RC_ERROR);
    }
    catch (const CosNaming::NamingContext::NotFound&)
      {
        // Not on replica --- as it should be.
        if (retried)  // Was found on the retry
          ACE_ERROR (( LM_ERROR,
                       ACE_TEXT ("INFO: Was removed after short wait.\n")));
      }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to delete objects"));
    return RC_ERROR;
  }

  // Now use the other name server to access 3 objects next to the
  // deleted objects and the 3 deleted objects
  try
  {
    // Access the last object from the Naming Context
    CosNaming::Name wide;
    wide.length (2);
    wide[0].id = CORBA::string_dup ("level1_context");
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "obj_%d", o_breadth-1);
    wide[1].id = CORBA::string_dup (wide_name);
    CORBA::Object_var result_obj_ref = root_context_1->resolve (wide);
    Test_Object_var result_object = Test_Object::_narrow (result_obj_ref.in ());
    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Problems with resolving object from ")
                         ACE_TEXT ("redundant server - nil object ref.\n")),
                         RC_ERROR);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
        ACE_TEXT ("ERROR: Unable to resolve object from redundant server"));
    return RC_ERROR;
  }

  try
  {
    // Access the deleted second to last object from the Naming Context
    CosNaming::Name wide;
    wide.length (2);
    wide[0].id = CORBA::string_dup ("level1_context");
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "obj_%d", o_breadth-2);
    wide[1].id = CORBA::string_dup (wide_name);

    CORBA::Object_var result_obj_ref = root_context_1->resolve (wide);

    ACE_ERROR_RETURN ((LM_ERROR,
                    ACE_TEXT ("ERROR: Problems with resolving object from ")
                    ACE_TEXT ("redundant server - deleted object found.\n")),
                    RC_ERROR);
  }
  catch (const CosNaming::NamingContext::NotFound& ex)
  {
    // expect exception since the context was deleted.
    // Make sure the right exception reason is provided.
    if (ex.why != CosNaming::NamingContext::missing_node)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Wrong exception returned during resolve.\n")),
                         RC_ERROR);
    }
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
          ACE_TEXT ("ERROR: Wrong exception type returned from resolve.\n"));
      return RC_ERROR;
    }

  try
  {
    // Access the last context from the wide Naming Context
    CosNaming::Name wide;
    wide.length (1);
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "wide_%d", c_breadth-1);
    wide[0].id = CORBA::string_dup (wide_name);

    CORBA::Object_var result_obj_ref = root_context_1->resolve (wide);

    CosNaming::NamingContext_var result_object =
      CosNaming::NamingContext::_narrow (result_obj_ref.in ());

    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Problems with resolving wide context from ")
                          ACE_TEXT ("redundant server - nil object ref.\n")),
                          RC_ERROR);
    }
  }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      // Expected exception
    }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ( ACE_TEXT ("ERROR: Unexpected Exception received.\n"));
    return RC_ERROR;
  }

  try
  {
    // Access the deleted second to last object from the Naming Context
    CosNaming::Name wide;
    wide.length (2);
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "wide_%d", c_breadth-2);
    wide[0].id = CORBA::string_dup (wide_name);

    CORBA::Object_var result_obj_ref = root_context_1->resolve (wide);

    ACE_ERROR_RETURN ((LM_ERROR,
                    ACE_TEXT ("ERROR: Problems with resolving wide context from ")
                    ACE_TEXT ("redundant server - deleted object found.\n")),
                    RC_ERROR);
  }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      // Expected exception
    }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ( ACE_TEXT ("ERROR: Unexpected Exception received.\n"));
    return RC_ERROR;
  }

  try
  {
    // Access the deleted last context from the deep Naming Context
    CosNaming::Name deep;
    deep.length (c_depth);
    char deep_name[16];
    for (i=0; i<c_depth; i++)
    {
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[i].id = CORBA::string_dup (deep_name);
    }

    CORBA::Object_var result_obj_ref = root_context_1->resolve (deep);

    ACE_ERROR_RETURN ((LM_ERROR,
                    ACE_TEXT ("ERROR: Problems with resolving deep context from ")
                    ACE_TEXT ("redundant server - deleted object found.\n")),
                    RC_ERROR);
  }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      // Expected exception
    }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
             ACE_TEXT ("ERROR: Unexpected Exception received resolving ")
             ACE_TEXT ("deep cxt from redundant server.\n"));
    return RC_ERROR;
  }

  try
  {
    // Access the second to last object from the Naming Context
    CosNaming::Name deep;
    deep.length (c_depth-1);
    char deep_name[16];
    for (i=0; i<c_depth-1; i++)
    {
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[i].id = CORBA::string_dup (deep_name);
    }

    CORBA::Object_var result_obj_ref =
      root_context_1->resolve (deep);

    CosNaming::NamingContext_var result_object =
      CosNaming::NamingContext::_narrow (result_obj_ref.in ());

    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("ERROR: Problems with resolving deep context from ")
                      ACE_TEXT ("redundant server - nil object ref.\n")),
                      RC_ERROR);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to resolve deep context from redundant server"));
    return RC_ERROR;
  }

  return RC_SUCCESS;
}

/// Failover ObjectGroup Test
int
do_failover_objectgroup_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *nm1ref,
  int c_breadth,
  int c_depth,
  int o_breadth )
{
  ACE_UNUSED_ARG (c_breadth);
  ACE_UNUSED_ARG (c_depth);
  ACE_UNUSED_ARG (o_breadth);

  const int RC_SUCCESS =  0;
  const int RC_ERROR   = -1;

  FT_Naming::NamingManager_var naming_manager_1;

  try {

    CORBA::ORB_var orb = CORBA::ORB::_duplicate(theOrb);
    if (CORBA::is_nil (orb.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (CORBA::is_nil (nm1ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid nm1\n")),
                          RC_ERROR);
    }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("INFO: nm1ref: %s\n"), nm1ref));

    CORBA::Object_var nm1obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (nm1ref));

    if (CORBA::is_nil (nm1obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),nm1ref),
                          RC_ERROR);
    }

    naming_manager_1 = FT_Naming::NamingManager::_narrow (nm1obj.in ());
    if (CORBA::is_nil (naming_manager_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),nm1ref),
                          RC_ERROR);
    }

    NS_group_svc svc;

    if (RC_SUCCESS != svc.set_orb (theOrb))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (RC_SUCCESS != svc.set_naming_manager (naming_manager_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),nm1ref),
                          RC_ERROR);
    }


    const ACE_TCHAR * test_group = ACE_TEXT ("test_group");
    const ACE_TCHAR * policy     = ACE_TEXT ("round");

    if (RC_SUCCESS != svc.group_create (test_group, policy))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: unable to create %s\n"),test_group),
                          RC_ERROR);
    }

    if (false == svc.group_exist(test_group))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Object Group Not Found In Repository\n")),
                          RC_ERROR);
    } else {
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("INFO: Object Group Found In Repository\n")));
    }


  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to resolve name manager servers"));
    return RC_ERROR;
  }

  return RC_SUCCESS;
}


//==============================================================================
//
//==============================================================================
/// Persistence Name Test
int
do_persistence_name_test (
    CORBA::ORB_ptr theOrb,
    ACE_TCHAR *ns1ref,
    int c_breadth,
    int c_depth,
    int o_breadth,
    bool validate_only)
{
  const int RC_ERROR   = -1;
  const int RC_SUCCESS =  0;
  CosNaming::NamingContext_var root_context_1;
  int i;

  try {

    CORBA::ORB_var orb = CORBA::ORB::_duplicate(theOrb);

    if (CORBA::is_nil (orb.in () ))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (CORBA::is_nil (ns1ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ns1\n")),
                          RC_ERROR);
    }

    CORBA::Object_var ns1obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ns1ref));

    if (CORBA::is_nil (ns1obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns1ref),
                          RC_ERROR);
    }

    root_context_1 = CosNaming::NamingContext::_narrow (ns1obj.in ());

    if (CORBA::is_nil (root_context_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns1ref),
                          RC_ERROR);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (ACE_TEXT ("ERROR: Unable to resolve name servers"));
    return RC_ERROR;
  }

  // Create a bunch of objects in one context
  // Note: strings to the naming service must be char, not wchar
  try
  {
    // Bind one context level under root.
    CosNaming::Name level1;
    level1.length (1);
    level1[0].id = CORBA::string_dup ("level1_context");

    if( false == validate_only ) {
      CosNaming::NamingContext_var level1_context;
      level1_context = root_context_1->bind_new_context (level1);
      ACE_OS::thr_yield ();
    }

    for (i=0; i<o_breadth; i++)
    {
      // Instantiate a dummy object and bind it under the new context.
      My_Test_Object *impl1 = new My_Test_Object (i+1);
      Test_Object_var obj1 = impl1->_this ();
      impl1->_remove_ref ();

      level1.length (2);
      char wide_name[16];
      ACE_OS::sprintf (wide_name, "obj_%d", i);
      level1[1].id = CORBA::string_dup (wide_name);

      if( false == validate_only ) {
        root_context_1->bind (level1, obj1.in ());
        ACE_OS::thr_yield ();
      }


      // See if the newly bound object is available in the repository
      try {
        CORBA::Object_var obj1_on_replica = root_context_1->resolve (level1);
      }
      catch (const CosNaming::NamingContext::NotFound& ex)
        {
          ex._tao_print_exception (
            ACE_TEXT ("ERROR: Unable to resolve object from repository. Sleeping for a second.\n"));

          ACE_OS::sleep (1);

          // Try again...
          try {
            CORBA::Object_var obj1_on_replica = root_context_1->resolve (level1);
            // We did find the object on the repository, but only after a wait.
            // This would be caused by a race condition to access the variable.
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("INFO: Object appeared after a short wait.\n")));
          }
          catch (const CosNaming::NamingContext::NotFound& second_ex)
            {
              second_ex._tao_print_exception (
                ACE_TEXT ("ERROR: It really is not there. Failing...\n"));
              return RC_ERROR;
            }
        }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create or validate a lot of objects"));

    return RC_ERROR;
  }

  if( false == validate_only ) {
    // Create a deep context tree
    try
    {
      CosNaming::NamingContext_var next_context = root_context_1;
      for (i=0; i<c_depth; i++)
      {
        // Bind level1 context under root.
        CosNaming::Name deep;
        deep.length (1);
        char deep_name[16];
        ACE_OS::sprintf (deep_name, "deep_%d", i);
        deep[0].id = CORBA::string_dup (deep_name);

        CosNaming::NamingContext_var deep_context;
        deep_context = next_context->bind_new_context (deep);
        ACE_OS::thr_yield ();
        next_context = deep_context;
      }
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("ERROR: Unable to create deep context"));
      return RC_ERROR;
    }
  }

  // Create a wide context tree
  try
  {
    for (i=0; i<c_breadth; i++)
    {
      // Bind all level of context under root.
      CosNaming::Name wide;
      wide.length (1);
      char wide_name[16];
      ACE_OS::sprintf (wide_name, "wide_%d", i);
      wide[0].id = CORBA::string_dup (wide_name);

      if( false == validate_only ) {
        CosNaming::NamingContext_var wide_context;
        wide_context = root_context_1->bind_new_context (wide);
        ACE_OS::thr_yield ();
      }

      try {

        // Check if the new context is available in the repository
        CORBA::Object_var obj1_on_replica =
          root_context_1->resolve (wide);

        // Make sure it is a context
        CosNaming::NamingContext_var nc =
          CosNaming::NamingContext::_narrow (obj1_on_replica.in ());
      }
      catch (const CosNaming::NamingContext::NotFound& ex)
        {
          ex._tao_print_exception (
              ACE_TEXT ("INFO: Unable to resolve wide context object from ")
              ACE_TEXT ("repository. Sleeping for a second.\n"));

          ACE_OS::sleep (1);

          // Try again to see if it just was a race condition
          try {
            CORBA::Object_var obj1_on_replica = root_context_1->resolve (wide);
            // We did find the object on the replica, but only after a wait.
            // This would be caused by a race condition to access the variable.
            ACE_ERROR (( LM_ERROR,
                         ACE_TEXT ("INFO: Object appeared after a short ")
                         ACE_TEXT ("wait.\n")));
          }
          catch (const CosNaming::NamingContext::NotFound& second_ex)
            {
              second_ex._tao_print_exception (
                  ACE_TEXT ("ERROR: It really is not there. Failing...\n"));
              return RC_ERROR;
            }
        }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create or validate wide context"));
    return RC_ERROR;
  }

  return RC_SUCCESS;

}


/// Persistence ObjectGroup Test
int
do_persistence_objectgroup_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *nm1ref,
  int c_breadth,
  int c_depth,
  int o_breadth,
  bool validate_only)
{
  ACE_UNUSED_ARG (c_breadth);
  ACE_UNUSED_ARG (c_depth);
  ACE_UNUSED_ARG (o_breadth);

  const int RC_ERROR   = -1;
  const int RC_SUCCESS =  0;

  FT_Naming::NamingManager_var naming_manager_1;

  try {

    CORBA::ORB_var orb = CORBA::ORB::_duplicate(theOrb);

    if (CORBA::is_nil (orb.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (CORBA::is_nil (nm1ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid nm1\n")),
                          RC_ERROR);
    }

    //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("nm1ref: %s\n"), nm1ref));

    CORBA::Object_var nm1obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (nm1ref));

    if (CORBA::is_nil (nm1obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),nm1ref),
                          RC_ERROR);
    }

    naming_manager_1 = FT_Naming::NamingManager::_narrow (nm1obj.in ());
    if (CORBA::is_nil (naming_manager_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),nm1ref),
                          RC_ERROR);
    }

    NS_group_svc group_svc;

    if( RC_SUCCESS != group_svc.set_orb (theOrb) )
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if( RC_SUCCESS != group_svc.set_naming_manager (naming_manager_1.in ()) )
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),nm1ref),
                          RC_ERROR);
    }

    /// get BasicGroup member object and verify that it reports the same location
    const ACE_TCHAR * basic_group_name = ACE_TEXT ("BasicGroup");

    if (false == group_svc.group_exist(basic_group_name))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Object Group %s Not Found In Repository\n"),
                          basic_group_name),
                          RC_ERROR);
    } else {
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("INFO: Object Group %s Found In Repository\n"),
                   basic_group_name));
    }

    try
    {
      PortableGroup::ObjectGroup_var group_var =
        naming_manager_1->get_object_group_ref_from_name (
          ACE_TEXT_ALWAYS_CHAR (basic_group_name));

      PortableGroup::Locations_var locations =
        naming_manager_1->locations_of_members (group_var.in());

      for (unsigned int i = 0; i < locations->length(); ++i)
      {
        const PortableGroup::Location & loc = locations[i];
        if (loc.length() > 0) {

          ACE_DEBUG (( LM_DEBUG,
                       ACE_TEXT ("INFO: validating group member %C\n"),
                       loc[0].id.in()));

          try
          {

            PortableGroup::Location location_name (1);
            location_name.length (1);
            location_name[0].id = CORBA::string_dup(loc[0].id.in());

            CORBA::Object_var ior_var =
              naming_manager_1->get_member_ref (group_var.in(), location_name);

              // Narrow it to a Basic object
            Test::Basic_var basic = Test::Basic::_narrow (ior_var.in ());
            if (CORBA::is_nil (basic.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR: Unable to narrow from member ior from %C\n"),
                                 loc[0].id.in()),
                                 RC_ERROR);
            }

            try
            {
              CORBA::String_var the_string = basic->get_string ();

              ACE_DEBUG (( LM_DEBUG,
                           ACE_TEXT ("INFO: object group member at %C reports ")
                           ACE_TEXT ("%C\n"),
                           loc[0].id.in(),
                           the_string.in ()));

              if ( ACE_OS::strcmp (the_string.in (), loc[0].id.in()) != 0 ) {

                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("ERROR: object group member at %C reports %C\n\n"),
                                   loc[0].id.in(),
                                   the_string.in ()),
                                   RC_ERROR);
              }
            }
            catch (CORBA::Exception& ex)
            {
              ex._tao_print_exception ("ERROR: invoking get_string on Basic object.\n");
              return RC_ERROR;
            }
          }
          catch (const PortableGroup::MemberNotFound&)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("ERROR: Unable to find member location %C\n"),
                               loc[0].id.in()),
                               RC_ERROR);
          }
        }
      }

      std::string member_data_file ("member.dat");

      // Test persisting group modification in first pass and reading
      // the persistent data in the second pass.
      // This is done by removal of a group member in first pass and
      // then adding it back in the second, validation, pass,
      // verifying that member count is what it was before the removal.
      // This is tracked by saving member data to a file
      // in the first pass and then reading it in the second pass.

      if (!validate_only)
        {
          try
          {
            PortableGroup::Locations_var locations =
              naming_manager_1->locations_of_members (group_var.in());
            const PortableGroup::Location & loc = locations.inout ()[0];
            unsigned int num_locations = locations->length();

            PortableGroup::Location location_name (1);
            location_name.length (1);
            location_name[0].id = CORBA::string_dup(loc[0].id.in());

            CORBA::Object_var ior_var =
              naming_manager_1->get_member_ref (group_var.in(), location_name);

              // Narrow it to a Basic object
            Test::Basic_var basic = Test::Basic::_narrow (ior_var.in ());
            if (CORBA::is_nil (basic.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR: Unable to narrow from ")
                                 ACE_TEXT ("member ior from %C\n"),
                                 loc[0].id.in()),
                                 RC_ERROR);
            }
            group_var =
              naming_manager_1->remove_member (group_var.in(),
                                               location_name);

            // Save data for the removed member
            CORBA::String_var member_str = theOrb->object_to_string(basic.in ());
            std::ofstream out(member_data_file.c_str ());
            out << num_locations << std::endl;
            out << loc[0].id.in () << std::endl;
            out << member_str.in() << std::endl;

          }
          catch (const PortableGroup::ObjectGroupNotFound&)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR: Unable to find group %C\n"),
                                 basic_group_name),
                                RC_ERROR);
            }
          catch (const CORBA::Exception&)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR: Unable to remove members ")
                                 ACE_TEXT ("for group %C\n"),
                                 basic_group_name),
                                RC_ERROR);
            }
        } else {
        try
          {
            // Read in member data
            std::ifstream in (member_data_file.c_str ());
            unsigned int num_locations;
            in >> num_locations;
            std::string location_str;
            in >> location_str;
            std::string member_ior;
            in >> member_ior;

            if (!in.good ())
              {
                ACE_ERROR_RETURN  ((LM_ERROR,
                                    ACE_TEXT ("ERROR: Unable to read member data ")
                                    ACE_TEXT ("from file %C\n"),
                                    member_data_file.c_str ()),
                                   RC_ERROR);
              }

            CORBA::Object_var member =
              theOrb->string_to_object(member_ior.c_str ());
            PortableGroup::Location location_name (1);
            location_name.length (1);
            location_name[0].id = CORBA::string_dup(location_str.c_str ());
            PortableGroup::ObjectGroup_var group_var =
              naming_manager_1->get_object_group_ref_from_name
              (ACE_TEXT_ALWAYS_CHAR (basic_group_name));

            // Add back member
            group_var =
              naming_manager_1->add_member (group_var.in (),
                                            location_name,
                                            member.in ());
            PortableGroup::Locations_var locations =
              naming_manager_1->locations_of_members (group_var.in ());

            // Verify number of locations was what it was before removing/adding
            if (locations->length () != num_locations)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("ERROR: Expected %d members but got %d ")
                            ACE_TEXT ("instead\n"),
                            num_locations, locations->length ()));
              }

            // Verify data wasn't corrupt so still can still get
            // group information
            FT_Naming::GroupNames_var group_names =
              naming_manager_1->groups (FT_Naming::ROUND_ROBIN);
            if (group_names->length () == 0)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("ERROR: No group names found")));
              }
          }
        catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception ("CORBA::Exception caught:");
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("ERROR: Unable to remove member for group %C\n"),
                               basic_group_name),
                              RC_ERROR);
          }
      }

    }
    catch (const PortableGroup::ObjectGroupNotFound&)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Unable to find group %C\n"),
                         basic_group_name),
                         RC_ERROR);
    }
    catch (const CORBA::Exception&)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Unable to list members for group %C\n"),
                         basic_group_name),
                         RC_ERROR);
    }

  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
       ACE_TEXT ("ERROR: Unable To Resolve Name Manager Servers"));
    return RC_ERROR;
  }

  return RC_SUCCESS;
}

//==============================================================================
//
//==============================================================================
/// Equivalence Name Test
int
do_equivalence_name_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *ns1ref,
  ACE_TCHAR *ns2ref,
  int c_breadth,
  int c_depth,
  int o_breadth)
{
  const int RC_SUCCESS =  0;
  const int RC_ERROR   = -1;

  CosNaming::NamingContext_var root_context_1;
  CosNaming::NamingContext_var root_context_2;
  int i;

  try {

    CORBA::ORB_var orb = CORBA::ORB::_duplicate(theOrb);

    if (CORBA::is_nil (orb.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (CORBA::is_nil (ns1ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ns1\n")),
                          RC_ERROR);
    }

    if (CORBA::is_nil (ns2ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ns2\n")),
                          RC_ERROR);
    }

    CORBA::Object_var ns1obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ns1ref));

    CORBA::Object_var ns2obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ns2ref));

    if (CORBA::is_nil (ns1obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns1ref),
                          RC_ERROR);
    }

    if (CORBA::is_nil (ns2obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns2ref),
                          RC_ERROR);
    }

    root_context_1 = CosNaming::NamingContext::_narrow (ns1obj.in ());
    root_context_2 = CosNaming::NamingContext::_narrow (ns2obj.in ());

    if (CORBA::is_nil (root_context_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns1ref),
                          RC_ERROR);
    }

    if (CORBA::is_nil (root_context_2.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),ns2ref),
                          RC_ERROR);
    }

    /// Make sure the two naming manager references point to different objects
    if ( true == root_context_1->_stubobj ()->is_equivalent (
      root_context_2.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: ns1ref is equivalent to ns2ref\n")),
                          RC_ERROR);
    }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("INFO: ns1ref is not equivalent to ns2ref\n")));

    int ns1_count =
      root_context_1->_stubobj ()->base_profiles ().profile_count ();

    int ns2_count =
      root_context_2->_stubobj ()->base_profiles ().profile_count ();


    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("INFO: ns1ref profile count: %d\n"),
                ns1_count));

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("INFO: ns2ref profile count: %d\n"),
                ns2_count));

  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (ACE_TEXT ("ERROR: Unable to resolve name servers"));
    return RC_ERROR;
  }

  // Create a bunch of objects in one context
  // Note: strings to the naming service must be char, not wchar
  try
  {
    // Bind one context level under root.
    CosNaming::Name level1;
    level1.length (1);
    level1[0].id = CORBA::string_dup ("level1_context");
    CosNaming::NamingContext_var level1_context;
    level1_context = root_context_1->bind_new_context (level1);
    ACE_OS::thr_yield ();

    for (i=0; i<o_breadth; i++)
    {
      // Instantiate a dummy object and bind it under the new context.
      My_Test_Object *impl1 = new My_Test_Object (i+1);
      Test_Object_var obj1 = impl1->_this ();
      impl1->_remove_ref ();

      level1.length (2);
      char wide_name[16];
      ACE_OS::sprintf (wide_name, "obj_%d", i);
      level1[1].id = CORBA::string_dup (wide_name);
      root_context_1->bind (level1, obj1.in ());
      ACE_OS::thr_yield ();

      // See if the newly bound object is available in the
      // replica
      try {
        CORBA::Object_var obj1_on_replica = root_context_2->resolve (level1);
      }
      catch (const CosNaming::NamingContext::NotFound& )
        {
          ACE_DEBUG ((LM_INFO,
                      "INFO: Unable to resolve object from replica. "
                      "Sleeping for a second.\n"));

          ACE_OS::sleep (1);

          // Try again...
          try {
            CORBA::Object_var obj1_on_replica = root_context_2->resolve (level1);
            // We did find the object on the replica, but only after a wait.
            // This would be caused by a race condition to access the variable.
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("INFO: Object appeared after a short ")
                        ACE_TEXT ("wait.\n")));
          }
          catch (const CosNaming::NamingContext::NotFound& second_ex)
            {
              second_ex._tao_print_exception (
                ACE_TEXT ("ERROR: It really is not there. Failing...\n"));
              return RC_ERROR;
            }
        }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create a lot of objects"));
    return RC_ERROR;
  }

  // Create a deep context tree
  try
  {
    CosNaming::NamingContext_var next_context = root_context_1;
    for (i=0; i<c_depth; i++)
    {
      // Bind level1 context under root.
      CosNaming::Name deep;
      deep.length (1);
      char deep_name[16];
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[0].id = CORBA::string_dup (deep_name);
      CosNaming::NamingContext_var deep_context;
      deep_context = next_context->bind_new_context (deep);
      ACE_OS::thr_yield ();
      next_context = deep_context;
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create deep context"));
    return RC_ERROR;
  }

  // Create a wide context tree
  try
  {
    for (i=0; i<c_breadth; i++)
    {
      // Bind all level of context under root.
      CosNaming::Name wide;
      wide.length (1);
      char wide_name[16];
      ACE_OS::sprintf (wide_name, "wide_%d", i);
      wide[0].id = CORBA::string_dup (wide_name);
      CosNaming::NamingContext_var wide_context;
      wide_context = root_context_1->bind_new_context (wide);
      ACE_OS::thr_yield ();

      try {
        // Check if the new context is available in the replica
        CORBA::Object_var obj1_on_replica =
          root_context_2->resolve (wide);

        // Make sure it is a context
        CosNaming::NamingContext_var nc =
          CosNaming::NamingContext::_narrow (obj1_on_replica.in ());
      }
      catch (const CosNaming::NamingContext::NotFound& ex)
        {
          ex._tao_print_exception (
              ACE_TEXT ("INFO: Unable to resolve wide context object from ")
              ACE_TEXT ("replica. Sleeping for a second.\n"));

          ACE_OS::sleep (1);

          // Try again to see if it just was a race condition
          try {

            CORBA::Object_var obj1_on_replica =
              root_context_2->resolve (wide);

            // We did find the object on the replica, but only after a wait.
            // This would be caused by a race condition to access the variable.
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("INFO: Object appeared after a ")
                        ACE_TEXT ("short wait.\n")));
          }
          catch (const CosNaming::NamingContext::NotFound& second_ex)
            {
              second_ex._tao_print_exception (
                ACE_TEXT ("ERROR: It really is not there. Failing...\n"));
              return RC_ERROR;
            }
        }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to create wide context"));
    return RC_ERROR;
  }

  // Delete three selected things, one from each tree
  try {
    // Remove the second to last object from the Naming Context
    CosNaming::Name wide1;
    wide1.length (2);

    wide1[0].id = CORBA::string_dup ("level1_context");

    char wide_name[16];
    ACE_OS::sprintf (wide_name, "obj_%d", o_breadth-2);
    wide1[1].id = CORBA::string_dup (wide_name);
    root_context_1->unbind (wide1);
    ACE_OS::thr_yield ();

    bool retried = false;
    // Make sure it is gone from the replica
    try {
      CORBA::Object_var obj1_on_replica = root_context_2->resolve (wide1);

      // An exception should be thrown by the above or
      // there is an error. This means the replica did
      // not register the loss of the context.
      ACE_ERROR (( LM_ERROR,
                   ACE_TEXT ("INFO: Unbound deep context not removed from ")
                   ACE_TEXT ("replica. Trying again...\n")));

      retried = true;  // Mark this so it can be reported in catch block.
      obj1_on_replica = root_context_2->resolve (wide1);

      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Unbound context not removed from ")
                          ACE_TEXT ("on retry\n")),
                          RC_ERROR);
    }
    catch (const CosNaming::NamingContext::NotFound&)
      {
        // Not on replica --- as it should be.
        if (retried)  // Was found on the retry
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("INFO: Was removed after short wait.\n")));
      }

    // Remove the second to last context from the wide root Naming Context
    CosNaming::Name wide2;
    wide2.length (1);
    ACE_OS::sprintf (wide_name, "wide_%d", c_breadth-2);
    wide2[0].id = CORBA::string_dup (wide_name);

    CORBA::Object_var result_obj_ref = root_context_1->resolve (wide2);

    CosNaming::NamingContext_var result_object =
      CosNaming::NamingContext::_narrow (result_obj_ref.in ());

    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Problems with resolving wide")
                          ACE_TEXT (" context - nil object ref.\n")),
                          RC_ERROR);
    }

    result_object->destroy();
    root_context_1->unbind (wide2);
    ACE_OS::thr_yield ();

    // Remove the last context from the deep Naming Context
    CosNaming::Name deep;
    deep.length (c_depth);
    char deep_name[16];
    for (i=0; i<c_depth; i++)
    {
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[i].id = CORBA::string_dup (deep_name);
    }
    result_obj_ref = root_context_1->resolve (deep);
    result_object = CosNaming::NamingContext::_narrow (result_obj_ref.in ());
    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Problems with resolving deep")
                         ACE_TEXT (" context - nil object ref.\n")),
                         RC_ERROR);
    }
    result_object->destroy();
    root_context_1->unbind (deep);
    ACE_OS::thr_yield ();

    retried = false;
    // Make sure it is gone from the replica
    try {
      CORBA::Object_var obj1_on_replica = root_context_2->resolve (deep);

      // An exception should be thrown by the above or
      // there is an error. This means the replica did
      // not register the loss of the context.
      ACE_ERROR (( LM_ERROR,
                   ACE_TEXT ("INFO: Unbound deep context not removed from")
                   ACE_TEXT ("replica. Trying again...\n")));

      retried = true;  // Mark this so it can be reported in catch block.
      obj1_on_replica = root_context_2->resolve (deep);

      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Unbound context not removed from")
                          ACE_TEXT (" on retry\n")),
                          RC_ERROR);
    }
    catch (const CosNaming::NamingContext::NotFound&)
      {
        // Not on replica --- as it should be.
        if (retried)  // Was found on the retry
          ACE_ERROR (( LM_ERROR,
                       ACE_TEXT ("INFO: Was removed after short wait.\n")));
      }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to delete objects"));
    return RC_ERROR;
  }

  // Now use the other name server to access 3 objects next to the
  // deleted objects and the 3 deleted objects
  try
  {
    // Access the last object from the Naming Context
    CosNaming::Name wide;
    wide.length (2);
    wide[0].id = CORBA::string_dup ("level1_context");
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "obj_%d", o_breadth-1);
    wide[1].id = CORBA::string_dup (wide_name);
    CORBA::Object_var result_obj_ref = root_context_2->resolve (wide);
    Test_Object_var result_object = Test_Object::_narrow (result_obj_ref.in ());

    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Problems with resolving object ")
                         ACE_TEXT ("from redundant server ")
                         ACE_TEXT ("- nil object ref.\n")),
                         RC_ERROR);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
        ACE_TEXT ("ERROR: Unable to resolve object from redundant server"));
    return RC_ERROR;
  }

  try
  {
    // Access the deleted second to last object from the Naming Context
    CosNaming::Name wide;
    wide.length (2);
    wide[0].id = CORBA::string_dup ("level1_context");
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "obj_%d", o_breadth-2);
    wide[1].id = CORBA::string_dup (wide_name);

    CORBA::Object_var result_obj_ref = root_context_2->resolve (wide);

    ACE_ERROR_RETURN ((LM_ERROR,
                    ACE_TEXT ("ERROR: Problems with resolving object from ")
                    ACE_TEXT ("redundant server - deleted object found.\n")),
                    RC_ERROR);
  }
  catch (const CosNaming::NamingContext::NotFound& ex)
  {
    // expect exception since the context was deleted.
    // Make sure the right exception reason is provided.
    if (ex.why != CosNaming::NamingContext::missing_node)
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Wrong exception returned during ")
                          ACE_TEXT ("resolve.\n")),
                          RC_ERROR);
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
          ACE_TEXT ("ERROR: Wrong exception type returned from resolve.\n"));
      return RC_ERROR;
    }

  try
  {
    // Access the last context from the wide Naming Context
    CosNaming::Name wide;
    wide.length (1);
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "wide_%d", c_breadth-1);
    wide[0].id = CORBA::string_dup (wide_name);
    CORBA::Object_var result_obj_ref = root_context_2->resolve (wide);

    CosNaming::NamingContext_var result_object =
      CosNaming::NamingContext::_narrow (result_obj_ref.in ());

    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Problems with resolving wide ")
                         ACE_TEXT ("context from redundant server ")
                         ACE_TEXT ("- nil object ref.\n")),
                         RC_ERROR);
    }
  }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      // Expected exception
    }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ( ACE_TEXT ("ERROR: Unexpected Exception ")
                              ACE_TEXT ("received.\n"));
    return RC_ERROR;
  }

  try
  {
    // Access the deleted second to last object from the Naming Context
    CosNaming::Name wide;
    wide.length (2);
    char wide_name[16];
    ACE_OS::sprintf (wide_name, "wide_%d", c_breadth-2);
    wide[0].id = CORBA::string_dup (wide_name);
    CORBA::Object_var result_obj_ref = root_context_2->resolve (wide);

    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("ERROR: Problems with resolving wide context ")
                       ACE_TEXT ("from redundant server ")
                       ACE_TEXT ("- deleted object found.\n")),
                       RC_ERROR);
  }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      // Expected exception
    }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ( ACE_TEXT ("ERROR: Unexpected Exception ")
                              ACE_TEXT ("received.\n"));
    return RC_ERROR;
  }

  try
  {
    // Access the deleted last context from the deep Naming Context
    CosNaming::Name deep;
    deep.length (c_depth);
    char deep_name[16];
    for (i=0; i<c_depth; i++)
    {
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[i].id = CORBA::string_dup (deep_name);
    }
    CORBA::Object_var result_obj_ref = root_context_1->resolve (deep);

    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("ERROR: Problems with resolving deep context ")
                       ACE_TEXT ("from redundant server ")
                       ACE_TEXT ("- deleted object found.\n")),
                       RC_ERROR);
  }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      // Expected exception
    }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
             ACE_TEXT ("ERROR: Unexpected Exception received resolving ")
             ACE_TEXT ("deep cxt from redundant server.\n"));
    return RC_ERROR;
  }

  try
  {
    // Access the second to last object from the Naming Context
    CosNaming::Name deep;
    deep.length (c_depth-1);
    char deep_name[16];
    for (i=0; i<c_depth-1; i++)
    {
      ACE_OS::sprintf (deep_name, "deep_%d", i);
      deep[i].id = CORBA::string_dup (deep_name);
    }
    CORBA::Object_var result_obj_ref = root_context_1->resolve (deep);

    CosNaming::NamingContext_var result_object =
      CosNaming::NamingContext::_narrow (result_obj_ref.in ());

    if (CORBA::is_nil (result_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                      ACE_TEXT ("ERROR: Problems with resolving deep ")
                      ACE_TEXT ("context from redundant server ")
                      ACE_TEXT ("- nil object ref.\n")),
                      RC_ERROR);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("ERROR: Unable to resolve deep context from redundant server."));
    return RC_ERROR;
  }

  return RC_SUCCESS;
}


/// Equivalence ObjectGroup Test
int
do_equivalence_objectgroup_test (
  CORBA::ORB_ptr theOrb,
  ACE_TCHAR *nm1ref,
  ACE_TCHAR *nm2ref,
  int c_breadth,
  int c_depth,
  int o_breadth )
{
  ACE_UNUSED_ARG (c_breadth);
  ACE_UNUSED_ARG (c_depth);
  ACE_UNUSED_ARG (o_breadth);

  const int RC_SUCCESS =  0;
  const int RC_ERROR   = -1;
  int error_count = 0;


  FT_Naming::NamingManager_var naming_manager_1;
  FT_Naming::NamingManager_var naming_manager_2;

  try {

    CORBA::ORB_var orb = CORBA::ORB::_duplicate(theOrb);
    if (CORBA::is_nil (orb.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (CORBA::is_nil (nm1ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid nm1\n")),
                          RC_ERROR);
    }

    ACE_DEBUG (( LM_DEBUG,
                 ACE_TEXT ("INFO: nm1ref: %s\n"),
                 nm1ref));

    CORBA::Object_var nm1obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (nm1ref));

    if (CORBA::is_nil (nm1obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),
                          nm1ref),
                          RC_ERROR);
    }

    naming_manager_1 = FT_Naming::NamingManager::_narrow (nm1obj.in ());
    if (CORBA::is_nil (naming_manager_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),
                          nm1ref),
                          RC_ERROR);
    }

    if (CORBA::is_nil (nm2ref))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid nm2\n")),
                          RC_ERROR);
    }

    ACE_DEBUG (( LM_DEBUG,
                 ACE_TEXT ("INFO: nm2ref: %s\n"),
                 nm2ref));

    CORBA::Object_var nm2obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (nm2ref));

    if (CORBA::is_nil (nm2obj.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),
                          nm2ref),
                          RC_ERROR);
    }

    naming_manager_2 = FT_Naming::NamingManager::_narrow (nm2obj.in ());

    if (CORBA::is_nil (naming_manager_2.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),
                          nm2ref),
                          RC_ERROR);
    }

    /// Make sure the two naming manager references point to different objects
    if ( true == naming_manager_1->_stubobj ()->is_equivalent (
      naming_manager_2.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: nm1ref is equivalent to nm2ref\n")),
                          RC_ERROR);
    }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("INFO: nm1ref is not equivalent to nm2ref\n")));
    int nm1_count =
      naming_manager_1->_stubobj ()->base_profiles ().profile_count ();

    int nm2_count =
      naming_manager_2->_stubobj ()->base_profiles ().profile_count ();


    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("INFO: nm1ref profile count: %d\n"),
                nm1_count));

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("INFO: nm2ref profile count: %d\n"),
                nm2_count));


    /// Setup 1st NS_group_svc with 1st Naming Manager
    NS_group_svc svc1;
    if (RC_SUCCESS != svc1.set_orb (theOrb))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (RC_SUCCESS != svc1.set_naming_manager (naming_manager_1.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),
                          nm1ref),
                          RC_ERROR);
    }

    /// Setup 2nd NS_group_svc with 2nd Naming Manager
    NS_group_svc svc2;
    if (RC_SUCCESS != svc2.set_orb (theOrb))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid orb\n")),
                          RC_ERROR);
    }

    if (RC_SUCCESS != svc2.set_naming_manager (naming_manager_2.in ()))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: invalid ior <%s>\n"),nm2ref),
                          RC_ERROR);
    }

    /// Now validate seamless operations between the two instances of NS_group
    const ACE_TCHAR * policy       = ACE_TEXT ("round");
    const ACE_TCHAR * test_group_1 = ACE_TEXT ("test_group_1");
    const ACE_TCHAR * test_group_2 = ACE_TEXT ("test_group_2");

    if (RC_SUCCESS != svc1.group_create (test_group_1, policy))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Unable to create %s\n"),
                          test_group_1),
                          RC_ERROR);
    }

    if (RC_SUCCESS != svc2.group_create (test_group_2, policy))
    {
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("ERROR: Unable to create %s\n"),
                          test_group_2),
                          RC_ERROR);
    }

    if (false == svc1.group_exist(test_group_1))
    {
      ++error_count;
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("ERROR: Primary Not Able To Find Object Group ")
                   ACE_TEXT ("%s Created By Primary In Repository\n"),
                   test_group_1));
    } else {
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("INFO: Primary Found Object Group %s Created By ")
                   ACE_TEXT ("Primary In Repository\n"),
                   test_group_1));
    }

    if (false == svc2.group_exist(test_group_2))
    {
      ++error_count;
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("ERROR: Backup Not Able To Find Object Group %s ")
                   ACE_TEXT ("Created By Backup In Repository\n"),
                   test_group_2));
    } else {
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("INFO: Backup Found Object Group %s Created By ")
                   ACE_TEXT ("Backup In Repository\n"),
                   test_group_2));
    }

    if (false == svc1.group_exist(test_group_2))
    {
      ++error_count;
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("ERROR: Primary Not Able To Find Object Group %s ")
                   ACE_TEXT ("Created By Backup In Repository\n"),
                   test_group_2));
    } else {
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("INFO: Primary Found Object Group %s Created By ")
                   ACE_TEXT ("Backup In Repository\n"),
                   test_group_2));
    }

    if (false == svc2.group_exist(test_group_1))
    {
      ++error_count;
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("ERROR: Backup Not Able To Find Object Group %s ")
                   ACE_TEXT ("Created By Primary In Repository\n"),
                   test_group_1));
    } else {
      ACE_DEBUG (( LM_DEBUG,
                   ACE_TEXT ("INFO: Backup Found Object Group %s Created By ")
                   ACE_TEXT ("Primary In Repository\n"),
                   test_group_1));
    }

  }
  catch (const CORBA::Exception& ex)
  {
    ++error_count;
    ex._tao_print_exception (
        ACE_TEXT ("ERROR: Unable to resolve name manager servers"));
  }

  return (error_count > 0) ? RC_ERROR : RC_SUCCESS;
}
