// $Id$

// This example uses the ACE_Registry class to iterator through the
// entries in the predefined registries. The host of iteration can be
// varied through argv[1]. If no host is specified the local host is
// used. This is very similar to how regedt32 starts up.
//
// This examples points the cool iterators in ACE_Registry

#include "ace/OS_main.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)

#include "ace/Registry.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"



// Indentation while printing names
static const u_long INDENTATION_LEVEL = 3;

// Prototypes
static int print_naming_context (ACE_Registry::Naming_Context &naming_context,
                                 u_long indentation);
static void print_object (const ACE_TString &name,
                          u_long indentation);
static void print_context (ACE_Registry::Naming_Context &parent,
                           const ACE_TString &name,
                           u_long indentation);
static void indent (u_long indentation);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result;
  ACE_Registry::Naming_Context naming_context;

  // Connect to a predefined naming context
  result = ACE_Predefined_Naming_Contexts::connect (naming_context,
                                                    HKEY_LOCAL_MACHINE,
                                                    // HKEY_CLASSES_ROOT,
                                                    // HKEY_USERS,
                                                    // HKEY_CURRENT_USER,
                                                    argc == 2 ? argv[1] : 0);

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Predefined_Naming_Contexts::connect failed"), -1);

  // Print contents of naming context
  result = ::print_naming_context (naming_context, 0);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "print_naming_context failed"), -1);

  return 0;
}


// Print contents of <naming_context>
static int
print_naming_context (ACE_Registry::Naming_Context &naming_context,
                      u_long indentation)
{
  ACE_Registry::Binding_List list;

  // Get the list of all entries
  int result = naming_context.list (list);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Context::list"), -1);

  // Iterator through all entries
  for (ACE_Registry::Binding_List::iterator i = list.begin ();
       i != list.end ();
       ++i)
    {
      // Yeeesss! STL rules!
      ACE_Registry::Binding &binding = *i;

      if (binding.type () == ACE_Registry::OBJECT)
        // If object
        ::print_object (binding.name (),
                        indentation);
      else
        // If context
        ::print_context (naming_context,
                         binding.name (),
                         indentation);
    }
  return 0;
}


// Print an object with <name>
static void
print_object (const ACE_TString &name,
              u_long indentation)
{
  // Set indentation
  ::indent (indentation);
  cout << name << endl;
}


// Print an context with <name> and <parent>
static void
print_context (ACE_Registry::Naming_Context &parent,
               const ACE_TString &name,
               u_long indentation)
{
  // Set indentation
  indent (indentation);
  cout << name << endl;

  ACE_Registry::Naming_Context child_context;
  // Find child context
  int result = parent.resolve_context (name,
                                       child_context,
                                       KEY_READ);
  if (result != 0)
    ACE_ERROR ((LM_ERROR, "%s %s\n", "ACE_Registry::Naming_Context::resolve_context failed for:", name.c_str ()));
  else
    {
      // Print contents of the child
      result = ::print_naming_context (child_context,
                                       indentation + INDENTATION_LEVEL);
      if (result != 0)
        ACE_ERROR ((LM_ERROR, "%p\n", "print_naming_context failed"));
    }
}


// Pretty formating
static void
indent (u_long indentation)
{
  for (; indentation > 0; indentation--)
    cout << " ";
}
#else /* !ACE_WIN32 || ACE_LACKS_WIN32_REGISTRY */
int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_REGISTRY */
