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

  // First separates each <obj_addr> from the list of the obj_addr
  // Then concatenates the obj. key to the end of each one and 
  // tries to find a binding .........
 
  // Find the position where '/' seperator btn <obj_addr_list> and
  // <key_string>

  cout << "The corbaloc name is " << corbaloc_name << endl;
  CORBA::ULong count_addr = 0;
  CORBA::ULong pos = 0;
  CORBA::Boolean start_key_string = 1;

  const char rir_prefix [] = "rir:/";

  // If the protocol is "rir:", there is no need to do any of this.
  //
  if (ACE_OS::strncmp (corbaloc_name, rir_prefix, 
                       sizeof (rir_prefix)-1) != 0)
    {
      
      char *key_string = CORBA::string_alloc (sizeof (corbaloc_name));
      char *key_stringPtr = key_string;
      
      for (const char *i = corbaloc_name; *i != '\0'; ++i)
        {
          if (*i == ',')
            {
              // Increment the count of the addresses in the list
              ++count_addr;
            }
          
          if (*i == '/')
            {
              if (*(i+1) == '/')
                {
                  ++i;
                  ++pos;
                }
              else if (*(i+1) != '/')
                {
                  // This is the last addr and will not be terminated with a
                  // ',' ..but need to count it too.
                  ++count_addr;
                  // Indication that the next characters are to be
                  // assigned to key_string
                  start_key_string = 0;
                }
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
      
      // Copy the <obj_addr_list> to cloc_name.
      char *cloc_name = CORBA::string_alloc (pos);
      cloc_name = ACE_OS::strncpy (cloc_name, corbaloc_name, pos);
      
      ACE_DEBUG ((LM_DEBUG, "The obj_addr_list is %s\n", cloc_name));
      
      // Declare an array of addr.
      char *addr [count_addr];
      char *cloc_namePtr;
      
      CORBA::ULong current_addr = 0;
      
      addr [current_addr] = CORBA::string_alloc (pos);
      // Tokenize using "," as the seperator
      cloc_namePtr = ACE_OS::strtok (cloc_name, ",");
      
      const char file_prefix[] = "iiop://";
      
      while (cloc_namePtr != NULL)
        {

          addr [current_addr] = ACE_OS::strcpy (addr [current_addr],
                                                "\0");
          
          if (ACE_OS::strncmp (cloc_namePtr, file_prefix, 
                               sizeof (file_prefix)-1) != 0)
            {
              // If there is no explicit protocol specified, use the
              // default "iiop://"
              addr [current_addr] = ACE_OS::strcat (addr [current_addr],
                                                    file_prefix);
              
            }
          
          addr [current_addr] = ACE_OS::strcat(addr [current_addr],
                                               cloc_namePtr);
          
          addr [current_addr] = ACE_OS::strcat (addr [current_addr],
                                                key_string);
          
          addr [current_addr] = ACE_OS::strcat (addr [current_addr],
                                                "\0");
          
          ACE_DEBUG ((LM_DEBUG, "The obj_addr [%d] is %s\n", current_addr,
                      addr [current_addr]));
          
          ++current_addr;
          
          addr [current_addr] = CORBA::string_alloc (pos);
          cloc_namePtr = ACE_OS::strtok (NULL, ",");
        }
    }

  CORBA::Object_ptr object = CORBA::Object::_nil ();

  if (ACE_OS::strncmp (corbaloc_name, rir_prefix, 
                       sizeof (rir_prefix)-1) == 0)
    {
      // "rir" protocol used ... pass the key string as an
      // argument to the resolve_initial_references ()
      const char *key_string =
        corbaloc_name + sizeof (rir_prefix) -1;
      
      ACE_TRY 
        {
          ACE_DEBUG ((LM_DEBUG, "The key string is %s\n", key_string));
          object = orb->resolve_initial_references (key_string, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::SystemException, ex)
        {
          ACE_PRINT_EXCEPTION (ex, "CORBALOC_Parser");
        }
      ACE_ENDTRY;
     
    }
  
  return object;
}

ACE_FACTORY_DEFINE (TAO_IOR_CORBALOC, TAO_CORBALOC_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
