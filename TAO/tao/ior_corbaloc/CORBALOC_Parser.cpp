// $Id$

#include "CORBALOC_Parser.h"
#include "tao/ior_dll/Object_Loader.h"
#include "tao/Object.h"
#include "tao/ORB.h"
#include "tao/Exception.h"
#include "tao/Environment.h"
#include "ace/Read_Buffer.h"

#if !defined(__ACE_INLINE__)
#include "CORBALOC_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, CORBALOC_Parser, "$Id$")

TAO_CORBALOC_Parser::~TAO_CORBALOC_Parser (void)
{
}


static const char corbaloc_prefix[] = "corbaloc:";

int
TAO_CORBALOC_Parser::match_prefix (const char *ior_string) const
{
  cout << "in match prefix" << endl;
  return (ACE_OS::strncmp (ior_string,
                           corbaloc_prefix,
                           sizeof corbaloc_prefix - 1) == 0);
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string (const char *ior,
                                   CORBA::ORB_ptr orb,
                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  const char *corbaloc_name =
    ior + sizeof corbaloc_prefix - 1;

  cout << "After removing the prefix: the string is" << corbaloc_name
       << endl;
  // What does this function do ????
  // First separates each <obj_addr> from the list of the obj_addr
  // Then concatenates the obj. key to the end of each one and tries
  // to find a binding .........
  // First seperate ...

  // Find the position where '/' seperator btn <obj_addr_list> and
  // <key_string>

  CORBA::ULong count_addr = 0;
  CORBA::ULong pos = 0;
  CORBA::Boolean start_key_string = 1;

  cout << "It is coming here" << endl;

  char *key_string = CORBA::string_alloc (sizeof (corbaloc_name));
  char *key_stringPtr = key_string;


  for (const char *i = corbaloc_name; *i != '\0'; ++i)
    {
      if (*i == ',')
        {
          // Increment the count of the addresses in the list
        ++count_addr;
        }

      if (*i == '/' && *(i-1) != '/')
        {
          // This is the last addr and will not be terminated with a
          // ',' ..but need to count it too.
          ++count_addr;
          start_key_string = 0;
        }

      if (start_key_string == 1)
        {
          ++pos;
        }
      else
        {
          *key_stringPtr = *i;
          ++key_stringPtr;
        }

    }

  cout << "The position of the seperation btn obj_list and key_string
 is" << pos << endl;

  // Copy the <obj_addr_list> to cloc_name.
  char *cloc_name = CORBA::string_alloc (pos+1);
  cloc_name = ACE_OS::strncpy (cloc_name, corbaloc_name, pos);

  ACE_DEBUG ((LM_DEBUG, "The obj_addr_list is %s\n", cloc_name));

  // Declare an array of addr.
  char *addr [count_addr];

  CORBA::ULong current_addr = 0;

  addr [current_addr] = CORBA::string_alloc (pos);
  addr [current_addr] = ACE_OS::strtok (cloc_name, ",");
  addr [current_addr] = ACE_OS::strcat (addr [current_addr], key_string);
  cout << "The addr[0] is " << addr [current_addr] << endl;

  while ( addr [current_addr] != NULL)
    {
      ++current_addr;

      addr [current_addr] = CORBA::string_alloc (pos);

      // Now that presumably I have seperated the fields ... append the
      // <key_string> at the end of each of them.
      addr [current_addr] = ACE_OS::strcat (addr [current_addr], key_string);
      addr [current_addr] = ACE_OS::strtok (NULL, ",");
      ACE_DEBUG ((LM_DEBUG, "The obj_addr %d is %s\n", current_addr,
                  addr [current_addr]));

    }


  CORBA::Object_ptr object = CORBA::Object::_nil ();

  return object;

}

ACE_FACTORY_DEFINE (TAO_IOR_CORBALOC, TAO_CORBALOC_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
