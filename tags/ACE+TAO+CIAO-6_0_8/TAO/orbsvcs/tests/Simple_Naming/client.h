/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    client.h
 *
 *  $Id$
 *
 *    This class tests the facilities to connect to the naming service.
 *
 *
 *  @author   Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#include "test_objectS.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "tao/Utils/ORB_Manager.h"
#include "ace/Task.h"

/**
 * @class Naming_Test
 *
 * @brief This is an abstract class which is subclassed
 * to create different Naming Service tests.
 *
 * This is a basic example of the "Strategy" pattern.  This class
 * provides a common interface for different tests (or
 * "strategies"), so that a specific test to be used can be
 * chosen at runtime.
 */
class Naming_Test
{

public:
  /// Execute the test code.  <root_context> is the context to assume
  /// as the root for all tests operations.
  virtual ~Naming_Test (void);
  virtual int execute (TAO_Naming_Client &root_context) = 0;

protected:
  Naming_Test (PortableServer::POA_ptr poa);

  PortableServer::POA_var poa_;
};

/**
 * @class Simple_Test
 *
 * @brief This class implements a simple Naming Service test.
 *
 * The test binds(), resolves(), and unbinds() an object
 * reference from the given Naming Context.
 */
class Simple_Test : public Naming_Test
{
public:
  Simple_Test (PortableServer::POA_ptr poa);

  /// Execute the simple test code.
  virtual int execute (TAO_Naming_Client &root_context);
};

/**
 * @class MT_Test
 *
 * @brief This class implements a simple Multithreaded (multiclient) Naming Service test.
 *
 * The test spawns multiple threads: each attempts to
 * bind(), resolve(), and unbind() an object
 * reference using the same name, and the same Naming Context.
 */
class MT_Test : public Naming_Test, public ACE_Task_Base
{
public:
  // = Initialization and termination methods.

  /// Constructor.  Takes in an orb pointer and number of threads to spawn.
  MT_Test (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa,
           int size = 10);

  /// Execute the MT test code.
  virtual int execute (TAO_Naming_Client &root_context);

  /// This code is executed by each thread.
  virtual int svc (void);

private:
  /// Number of threads to spawn.  By default is set to 10.
  int size_;

  /// A pointer to our ORB.
  CORBA::ORB_var orb_;

  /**
   * IOR in the string format for Naming Service we are to deal with.
   * Each thread will use string_to_object() and this IOR to create
   * its own NamingContext stub for invoking operations on the
   * Naming Service.  If all threads try to use the same stub, bad things
   * happen...
   */
  CORBA::String_var name_service_ior_;

  // This can be replaced with CORBA::String_var when <string_to_object>
  // is fixed - this will clean up the memory properly.

  /// Holds name used for registering the object with Naming Service.
  CosNaming::Name test_name_;

  /// Holds object to be registered with the Naming Service by each thread.
  Test_Object_var test_ref_;

};

/**
 * @class Tree_Test
 *
 * @brief This class implements a test of all Naming Service functions
 * on a tree of Naming Contexts.
 *
 * Bind_context() under the root context with the name level1.
 * Create_new_context(), bind() foo object into it, and
 * bind the context into root/level1 under the name level2.
 * Resolve (root/level1/level2/foo).
 * Unbind (root/level1/level2/foo).
 * Bind (root/level1/level2/foo, obj)
 * Create_new_context()
 * and invoke rebind_context() to substitute it for the current
 * level2 context.
 * Bind (root/level1/level2/foo, obj)
 * Resolve (root/level1/level2/foo).
 * Rebind() to have a different object under the name bar.
 * Resolve (root/level1/level2/foo) to make sure correct reference is returned.
 */
class Tree_Test : public Naming_Test
{
public:
  /// Execute the tree test code.
  Tree_Test (PortableServer::POA_ptr poa);
  virtual int execute (TAO_Naming_Client &root_context);
};

/**
 * @class Iterator_Test
 *
 * @brief This class implements a test of Naming Service functions
 * which involve BindingIterator.
 *
 * The test binds foo1, foo2, foo3, and foo4 objects to the
 * Naming Context.  It lists() one binding and receives
 * BindingIterator to iterate over the rest of the bindings.  It
 * then invokes next_one(), next_n(2), next_one(), and destroy()
 * on the iterator.
 */
class Iterator_Test : public Naming_Test
{
public:
  /// Execute the iterator test code.
  Iterator_Test (PortableServer::POA_ptr poa);
  virtual int execute (TAO_Naming_Client &root_context);
};

/**
 * @class Exceptions_Test
 *
 * @brief This class implements a test of exceptions in the Naming Service.
 *
 * Makes sure that Naming Service throws exceptions as expected, and
 * data inside exceptions is set correctly.  The test creates a tree of
 * of Naming Contexts: root context -> level1 -> level2.  It then binds() an
 * object with the name foo to each of Naming Contexts in the tree.
 * Invoke resolve() with a Name of length 0 - make sure we get InvalidName exception.
 * Invoke bind( foo, obj) on root context - make sure we get AlreadyBound exception.
 * Invoke bind( level1/foo, obj) on root context - make sure we get AlreadyBound exc.
 * Invoke unbind( level1/level2/bar) on root context - make sure we get NotFound exc.
 * with why = not_object, rest_of_name = bar.
 * Invoke unbind( level1/level3/foo) on root context - make sure we get NotFound exc.
 * with why = missing_node, rest_of_name = level3/foo.
 * Invoke unbind( level1/foo/foo) on root context - make sure we get NotFound exc.
 * with why = not_context, rest_of_name = foo/foo.
 */
class Exceptions_Test : public Naming_Test
{
public:
  /// Execute the exceptions test code.
  Exceptions_Test (PortableServer::POA_ptr poa);
  virtual int execute (TAO_Naming_Client &root_context);

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

/**
 * @class Destroy_Test
 *
 * @brief This class implements a test of destroy() function
 * in the Naming Service.
 *
 * Create a context and bind an object under it.
 * Attempt to destroy the context - NotEmpty exception should be raised.
 * Unbind the object and call destroy on the context.
 * Attempt to call destroy on the object again - OBJECT_NOT_EXIST
 * exception should be raised.
 */
class Destroy_Test : public Naming_Test
{
public:
  /// Execute the destroy test code.
  Destroy_Test (PortableServer::POA_ptr poa);
  virtual int execute (TAO_Naming_Client &root_context);

private:
  // = The following functions isolate specific tests.
  void not_empty_test (CosNaming::NamingContext_var &ref);
  void not_exist_test (CosNaming::NamingContext_var &ref);
};

/**
 * @class Persistent_Test_Begin
 *
 * @brief This class implements the first part of the Persistent Naming
 * Service test.
 *
 * This test creates the Naming Context hierarchy:
 * root -> level1_context -> level2_context,
 * and prints out the ior of the <level1_context>.
 */
class Persistent_Test_Begin : public Naming_Test
{
public:
  // = Initialization and termination methods.

  /// Constructor.  Takes in an orb pointer.
  Persistent_Test_Begin (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         FILE * ior_output_file);

  /// Destructor.
  virtual ~Persistent_Test_Begin (void);

  /// Execute the persistent test (part 1) code.
  virtual int execute (TAO_Naming_Client &root_context);

private:

  /// A pointer to our ORB (needed for object/string conversion).
  CORBA::ORB_var orb_;

  /// File where we output the ior for use by part 2 of persistent test.
  FILE *file_;
};

/**
 * @class Persistent_Test_End
 *
 * @brief This class implements the second part of the Persistent Naming
 * Service test.
 *
 * This test attempts to resolve <level2_context> both through the
 * <root> Naming Context, which it gets from <resolve_initial_references>, and
 * through <level1_context> stringified ior, which it gets from part 1 of
 * the persistent test.  The results of both methods are then
 * compared for equality.
 */
class Persistent_Test_End : public Naming_Test
{
public:
  // = Initialization and termination methods.

  /// Constructor.  Takes in an orb pointer and the ior received from
  /// <Persistent_Test_Begin>.
  Persistent_Test_End (CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr poa,
                       const ACE_TCHAR * ior);

  /// Destructor.
  virtual ~Persistent_Test_End (void);

  /// Execute the persistent test (part 2).
  virtual int execute (TAO_Naming_Client &root_context);

private:

  /// A pointer to our ORB (used for string/object conversion).
  CORBA::ORB_var orb_;

  /// IOR of <level1_context> recorded during the run of part 1 of
  /// persistent test.
  const ACE_TCHAR* ior_;
};

/**
 * @class Persistent_List_Test
 *
 * @brief This class implements the third part of the Persistent Naming
 * Service test.
 *
 * This test attempts to invoke various list() methods on different
 * known contexts.
 */
class Persistent_List_Test : public Naming_Test
{
public:
  // = Initialization and termination methods.

  /// Constructor.  Takes in an orb pointer.
  Persistent_List_Test (CORBA::ORB_ptr orb,
                        PortableServer::POA_ptr poa);

  /// Destructor.
  virtual ~Persistent_List_Test (void);

  /// Execute the persistent test (part 3).
  virtual int execute (TAO_Naming_Client &root_context);

private:

  /// A pointer to our ORB (used for string/object conversion).
  CORBA::ORB_var orb_;
};

/**
 * @class CosNaming_Client
 *
 * @brief Defines a class that encapsulates behaviour of the CosNaming
 * client example.  Provides a better understanding of the logic
 * in an object-oriented way.
 *
 * This class declares an interface to run the example client for
 * CosNaming CORBA server.  All the complexity for initializing
 * the server is hidden in the class.  Just the <run> interface
 * is needed.
 */
class CosNaming_Client
{
public:
  // = Initialization and termination methods.

  /// Constructor.
  CosNaming_Client (void);

  /// Destructor.
  ~CosNaming_Client (void);

  /// Execute client example code.
  int run (void);

  /// Initialize the client communication endpoint with server.
  int init (int argc, ACE_TCHAR **argv);

  // = Symbolic ids.
  enum OBJ_ID
  {
    OBJ1_ID = 5,
    OBJ2_ID = 6
  };

private:
  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// A pointer to the specific Naming Service test a client will
  /// execute.
  Naming_Test *test_;

  /// Our ORB manager helper class.
  TAO_ORB_Manager orbmgr_;

  /// Our naming client helper class.
  TAO_Naming_Client naming_client_;
};
