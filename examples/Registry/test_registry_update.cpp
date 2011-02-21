// $Id$

// Suppose this application belonged to AcmeSoft. AcmeSoft wants to
// keep track of the number of times this application is
// executed. They want two counters: one counts the number of
// executions per machine, the other keeps track of the number of
// executions per user.
//
// This application uses the ACE_Registry class to create and update
// entries in the LOCAL_MACHINE and CURRENT_USER predefined registries
// to store the counters.
//
// Note that this application will not work with remote registries
// if used with the CURRENT_USER predefined registry.

#include "ace/OS_main.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)

#include "ace/Registry.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"



// Name for application's naming context
static ACE_Registry::Name application_context_name;

// Name for instance counter.
static ACE_Registry::Name counter_name;

// Protypes
static int update_counter (HKEY predefined,
                           u_long &current_counter);
static void setup_names ();

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int result;
  u_long current_counter = 0;

  // initialize name
  setup_names ();

  // Update counter per user
  result = ::update_counter (HKEY_CURRENT_USER,
                             current_counter);
  if (result == 0)
    {
      cout << "User counter: " << current_counter << endl;

      // Update counter per machine
      result = ::update_counter (HKEY_LOCAL_MACHINE,
                                 current_counter);
      if (result == 0)
        cout << "Machine counter: " << current_counter << endl;
    }

  if (result != 0)
    ACE_DEBUG ((LM_DEBUG, "test failed\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "test succeeded\n"));

  return 0;
}

static int
update_counter (HKEY predefined,
                u_long &current_counter)
{
  int result;
  ACE_Registry::Naming_Context parent_context;
  ACE_Registry::Naming_Context application_context;

  // Connect to predefined entry
  result = ACE_Predefined_Naming_Contexts::connect (parent_context,
                                                    predefined);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Predefined_Naming_Contexts::connect failed"), -1);

  // Find application context name
  result = parent_context.resolve_context (application_context_name,
                                           application_context);

  if (result != 0)
    // Failed to find: create a new context
    result = parent_context.bind_new_context (application_context_name,
                                              application_context);

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Contexts::bind/resolve_context failed"), -1);

  // Counter
  u_long counter = 0;
  // Represent counter as an ACE_Registry::Object
  ACE_Registry::Object object ((void *) &counter,
                               sizeof counter,
                               REG_DWORD);
  // Find counter
  result = application_context.resolve (counter_name,
                                        object);

  if (result != 0)
    // Failed to find: create new binding for object
    {
      counter = 1;
      result = application_context.bind (counter_name,
                                         object);
    }
  else
    // Counter was found
    {
      // increment counter
      counter++;
      // Update
      result = application_context.rebind (counter_name,
                                           object);
    }

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Contexts::bind/resolve failed"), -1);
  else
    current_counter = counter;

  return 0;
}


static void
setup_names ()
{
  // Stupid implementation of STL is broken
  /*
  ::application_context_name.reserve (4);
  ::application_context_name [0].id_ = ACE_TEXT ("Software");
  ::application_context_name [1].id_ = ACE_TEXT ("AcmeSoft");
  ::application_context_name [2].id_ = ACE_TEXT ("AcmeApplication");
  ::application_context_name [3].id_ = ACE_TEXT ("1.0");

  ::counter_name.reserve (1);
  ::counter_name [0].id_ = ACE_TEXT ("Instance Counter");
  */

  ACE_Registry::Name_Component component;

  component.id_ = ACE_TEXT ("Software"),         ::application_context_name.insert (component);
  component.id_ = ACE_TEXT ("AcmeSoft"),         ::application_context_name.insert (component);
  component.id_ = ACE_TEXT ("AcmeApplication"),  ::application_context_name.insert (component);
  component.id_ = ACE_TEXT ("1.0"),              ::application_context_name.insert (component);

  component.id_ = ACE_TEXT ("Instance Counter"), ::counter_name.insert (component);
}
#else /* !ACE_WIN32 || ACE_LACKS_WIN32_REGISTRY */
int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_REGISTRY */
