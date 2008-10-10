/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests
//
// = FILENAME
//    client.h
//
// = DESCRIPTION
//      This class tests the facilities to connect to the naming service.
//
// = AUTHORS
//      Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#include "test_objectS.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "tao/Utils/ORB_Manager.h"
#include "ace/Task.h"

class Naming_Test
{
  // = TITLE
  //    This is an abstract class which is subclassed
  //    to create different Naming Service tests.
  //
  // = DESCRIPTION
  //    This is a basic example of the "Strategy" pattern.  This class
  //    provides a common interface for different tests (or
  //    "strategies"), so that a specific test to be used can be
  //    chosen at runtime.

public:
  virtual ~Naming_Test (void) {}
  virtual int execute (TAO_Naming_Client &root_context) = 0;
  // Execute the test code.  <root_context> is the context to assume
  // as the root for all tests operations.
};

class Simple_Test : public Naming_Test
{
  // = TITLE
  //    This class implements a simple Naming Service test.
  //
  // = DESCRIPTION
  //    The test binds(), resolves(), and unbinds() an object
  //    reference from the given Naming Context.
public:
  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the simple test code.
};

class MT_Test : public Naming_Test, public ACE_Task_Base
{
  // = TITLE
  //    This class implements a simple Multithreaded (multiclient) Naming Service test.
  //
  // = DESCRIPTION
  //    The test spawns multiple threads: each attempts to
  //    bind(), resolve(), and unbind() an object
  //    reference using the same name, and the same Naming Context.
public:
  // = Initialization and termination methods.

  MT_Test (CORBA::ORB_ptr orb,
           int size = 10);
  // Constructor.  Takes in an orb pointer and number of threads to spawn.

  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the MT test code.

  virtual int svc (void);
  // This code is executed by each thread.

private:
  int size_;
  // Number of threads to spawn.  By default is set to 10.

  CORBA::ORB_var orb_;
  // A pointer to our ORB.

  char* name_service_ior_;
  // IOR in the string format for Naming Service we are to deal with.
  // Each thread will use string_to_object() and this IOR to create
  // its own NamingContext stub for invoking operations on the
  // Naming Service.  If all threads try to use the same stub, bad things
  // happen...

  // This can be replaced with CORBA::String_var when <string_to_object>
  // is fixed - this will clean up the memory properly.

  CosNaming::Name test_name_;
  // Holds name used for registering the object with Naming Service.

  Test_Object_var test_ref_;
  // Holds object to be registered with the Naming Service by each thread.

};

class Tree_Test : public Naming_Test
{
  // = TITLE
  //    This class implements a test of all Naming Service functions
  //    on a tree of Naming Contexts.
  //
  // = DESCRIPTION
  //    Bind_context() under the root context with the name level1.
  //    Create_new_context(), bind() foo object into it, and
  //    bind the context into root/level1 under the name level2.
  //    Resolve (root/level1/level2/foo).
  //    Unbind (root/level1/level2/foo).
  //    Bind (root/level1/level2/foo, obj)
  //    Create_new_context()
  //    and invoke rebind_context() to substitute it for the current
  //    level2 context.
  //    Bind (root/level1/level2/foo, obj)
  //    Resolve (root/level1/level2/foo).
  //    Rebind() to have a different object under the name bar.
  //    Resolve (root/level1/level2/foo) to make sure correct reference is returned.
public:
  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the tree test code.
};

class Iterator_Test : public Naming_Test
{
  // = TITLE
  //    This class implements a test of Naming Service functions
  //    which involve BindingIterator.
  //
  // = DESCRIPTION
  //    The test binds foo1, foo2, foo3, and foo4 objects to the
  //    Naming Context.  It lists() one binding and receives
  //    BindingIterator to iterate over the rest of the bindings.  It
  //    then invokes next_one(), next_n(2), next_one(), and destroy()
  //    on the iterator.
public:
  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the iterator test code.
};

class Exceptions_Test : public Naming_Test
{
  // = TITLE
  //    This class implements a test of exceptions in the Naming Service.
  //
  // = DESCRIPTION
  //    Makes sure that Naming Service throws exceptions as expected, and
  //    data inside exceptions is set correctly.  The test creates a tree of
  //    of Naming Contexts: root context -> level1 -> level2.  It then binds() an
  //    object with the name foo to each of Naming Contexts in the tree.
  //    Invoke resolve() with a Name of length 0 - make sure we get InvalidName exception.
  //    Invoke bind( foo, obj) on root context - make sure we get AlreadyBound exception.
  //    Invoke bind( level1/foo, obj) on root context - make sure we get AlreadyBound exc.
  //    Invoke unbind( level1/level2/bar) on root context - make sure we get NotFound exc.
  //    with why = not_object, rest_of_name = bar.
  //    Invoke unbind( level1/level3/foo) on root context - make sure we get NotFound exc.
  //    with why = missing_node, rest_of_name = level3/foo.
  //    Invoke unbind( level1/foo/foo) on root context - make sure we get NotFound exc.
  //    with why = not_context, rest_of_name = foo/foo.
public:
  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the exceptions test code.

private:
  // the following functions isolate specific tests due to the
  // limitation of only 1 TAO_TRY being allowed per function.

  void invalid_name_test (TAO_Naming_Client &root_context);
  void already_bound_test (TAO_Naming_Client &root_context);
  void already_bound_test2 (TAO_Naming_Client &root_context);
  void not_found_test (TAO_Naming_Client &root_context);
  void not_found_test2 (TAO_Naming_Client &root_context);
  void not_found_test3 (TAO_Naming_Client &root_context);
};

class Destroy_Test : public Naming_Test
{
  // = TITLE
  //    This class implements a test of destroy() function
  //    in the Naming Service.
  //
  // = DESCRIPTION
  //    Create a context and bind an object under it.
  //    Attempt to destroy the context - NotEmpty exception should be raised.
  //
  //    Unbind the object and call destroy on the context.
  //    Attempt to call destroy on the object again - OBJECT_NOT_EXIST
  //    exception should be raised.
  //
public:
  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the destroy test code.

private:
  // = The following functions isolate specific tests.
  void not_empty_test (CosNaming::NamingContext_var &ref);
  void not_exist_test (CosNaming::NamingContext_var &ref);
};

class Persistent_Test_Begin : public Naming_Test
{
  // = TITLE
  //    This class implements the first part of the Persistent Naming
  //    Service test.
  //
  // = DESCRIPTION
  //    This test creates the Naming Context hierarchy:
  //    root -> level1_context -> level2_context,
  //    and prints out the ior of the <level1_context>.
public:
  // = Initialization and termination methods.

  Persistent_Test_Begin (CORBA::ORB_ptr orb,
                         FILE * ior_output_file);
  // Constructor.  Takes in an orb pointer.

  virtual ~Persistent_Test_Begin (void);
  // Destructor.

  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the persistent test (part 1) code.

private:

  CORBA::ORB_var orb_;
  // A pointer to our ORB (needed for object/string conversion).

  FILE *file_;
  // File where we output the ior for use by part 2 of persistent test.
};

class Persistent_Test_End : public Naming_Test
{
  // = TITLE
  //    This class implements the second part of the Persistent Naming
  //    Service test.
  //
  // = DESCRIPTION
  //   This test attempts to resolve <level2_context> both through the
  //   <root> Naming Context, which it gets from <resolve_initial_references>, and
  //   through <level1_context> stringified ior, which it gets from part 1 of
  //   the persistent test.  The results of both methods are then
  //   compared for equality.
public:
  // = Initialization and termination methods.

  Persistent_Test_End (CORBA::ORB_ptr orb,
                       const ACE_TCHAR * ior);
  // Constructor.  Takes in an orb pointer and the ior received from
  // <Persistent_Test_Begin>.

  virtual ~Persistent_Test_End (void);
  // Destructor.

  virtual int execute (TAO_Naming_Client &root_context);
  // Execute the persistent test (part 2).

private:

  CORBA::ORB_var orb_;
  // A pointer to our ORB (used for string/object conversion).

  const ACE_TCHAR* ior_;
  // IOR of <level1_context> recorded during the run of part 1 of
  // persistent test.
};

class CosNaming_Client
{
  // = TITLE,
  //    Defines a class that encapsulates behaviour of the CosNaming
  //    client example.  Provides a better understanding of the logic
  //    in an object-oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    CosNaming CORBA server.  All the complexity for initializing
  //    the server is hidden in the class.  Just the <run> interface
  //    is needed.
public:
  // = Initialization and termination methods.

  CosNaming_Client (void);
  // Constructor.

  ~CosNaming_Client (void);
  // Destructor.

  int run (void);
  // Execute client example code.

  int init (int argc, ACE_TCHAR **argv);
  // Initialize the client communication endpoint with server.

  // = Symbolic ids.
  enum OBJ_ID
  {
    OBJ1_ID = 5,
    OBJ2_ID = 6
  };

private:
  int parse_args (void);
  // Parses the arguments passed on the command line.

  int argc_;
  // # of arguments on the command line.

  ACE_TCHAR **argv_;
  // arguments from command line.

  Naming_Test *test_;
  // A pointer to the specific Naming Service test a client will
  // execute.

  TAO_ORB_Manager orbmgr_;
  // Our ORB manager helper class.

  TAO_Naming_Client naming_client_;
  // Our naming client helper class.
};
