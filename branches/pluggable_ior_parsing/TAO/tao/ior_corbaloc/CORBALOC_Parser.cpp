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
  // @@ Priyanka: this routine turns out to be a little big, you may
  // want to divide in in several pieces, just to make it more
  // readable.

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
  CORBA::ULong addr_list_length = 0;
  CORBA::Boolean start_key_string = 1;

  const char rir_prefix [] = "rir:/";

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  // If the protocol is "rir:", there is no need to do any of this.
  //
  if (this->check_prefix (corbaloc_name) == 0)
    {
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
                  ++addr_list_length;
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
              ++addr_list_length;
            }
        }

      CORBA::String_var key_string = CORBA::string_dup (corbaloc_name + addr_list_length);

      // Copy the <obj_addr_list> to cloc_name.
      CORBA::String_var cloc_name = CORBA::string_alloc (addr_list_length);
      cloc_name = ACE_OS::strncpy (cloc_name, corbaloc_name, addr_list_length);
      cloc_name [addr_list_length] = '\0';

      ACE_DEBUG ((LM_DEBUG, "The obj_addr_list is %s\n", cloc_name.in ()));

      // Declare an array of addr.

      // @@ Priyanka: this is a non-standard extension, you are
      // declaring a variable sized array, you probably want to use
      //    char **addr;
      //    ACE_NEW_RETURN (addr, char*[count_addr], 0);
      // or better yet use an ACE_Array:
      //    ACE_Array_Base<char*> addr (count_addr);
      // because the latter is exception safe
      // Speaking of exception safety.... you may want to use
      // ACE_CString or something like that for your strings, so they
      // are all automatically deallocated if an exception is raised.
      //
      char **addr;
      ACE_NEW_RETURN (addr, char*[count_addr], 0);

      // @@ Priyanka: don't declare a variable without initializing
      // it, that is bad style, also: we don't use fooPtr in ACE+TAO,
      // please use <cloc_name_ptr> or something like that.

      char *cloc_name_ptr = '\0';

      CORBA::ULong current_addr = 0;

      // Tokenize using "," as the seperator
      // @@ Priyanka: in general you want to use ACE_OS::strtok_r()
      // because that version is reentrant and works with multiple
      // threads without any problems.
      // The regular strtok() uses a static variable to keep the last
      // pointer, so multiple threads cannot call it simultaneously.

      cloc_name_ptr = ACE_OS::strtok (cloc_name, ",");

      const char file_prefix[] = "iiop://";

      // @@ Priyanka: please don't use NULL, I know some books and
      // documents use it, but 0 is good enough for C++ (and safer).
      while (cloc_name_ptr != 0)
        {
          addr [current_addr] = CORBA::string_alloc (addr_list_length +
                                                     ACE_OS::strlen (key_string.in ()));
          
          if (ACE_OS::strncmp (cloc_name_ptr, file_prefix,
                               sizeof (file_prefix)-1) != 0)
            {
              // If there is no explicit protocol specified, use the
              // default "iiop://"
              ACE_OS::strcpy (addr [current_addr],
                              file_prefix);

              ACE_OS::strcat (addr [current_addr],
                              cloc_name_ptr);

            }
          else
            {
              ACE_OS::strcpy (addr [current_addr], 
                              cloc_name_ptr);

            }
          
          ACE_OS::strcat (addr [current_addr],
                          key_string.in ());
          
          ++current_addr;
          cloc_name_ptr = ACE_OS::strtok (NULL, ",");
          
        }
      
      // Make one MProfile for all of these endpoints
      TAO_MProfile mprofile;
  
      for (CORBA::ULong j = 0; j != count_addr; ++j)
        {
          int retv =
            orb->orb_core ()->connector_registry ()->make_mprofile (addr [j],
                                                                  mprofile,
                                                                  ACE_TRY_ENV);

          ACE_CHECK_RETURN (CORBA::Object::_nil ());   // Return nil.

          if (retv != 0)
            {
              ACE_THROW_RETURN (CORBA::INV_OBJREF (
                                  CORBA_SystemException::_tao_minor_code (
                                    TAO_DEFAULT_MINOR_CODE,
                                    EINVAL),
                                  CORBA::COMPLETED_NO),
                                CORBA::Object::_nil ());
            }

          // Now make the TAO_Stub.
          TAO_Stub *data = 0;
          ACE_NEW_THROW_EX (data,
                            TAO_Stub ((char *) 0, mprofile,
                                      orb->orb_core ()),
                            CORBA::NO_MEMORY (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          TAO_Stub_Auto_Ptr safe_data (data);

          // Figure out if the servant is collocated.
          TAO_ServantBase *servant = 0;
          TAO_SERVANT_LOCATION servant_location =
            orb->_get_collocated_servant (safe_data.get (),
                                           servant);

          int collocated = 0;
          if (servant_location != TAO_SERVANT_NOT_FOUND)
            collocated = 1;


          // Create the CORBA level proxy.  This will increase the ref_count
          // on data by one
          ACE_NEW_THROW_EX (obj,
                            CORBA_Object (safe_data.get (),
                                          servant,
                                          (CORBA::Boolean) collocated),
                            CORBA::NO_MEMORY (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          // All is well, so release the stub object from its auto_ptr.
          data = safe_data.release ();

        }

    }

  // @@ Priyanka: This is something that you could move to another
  // routine, all the RIR case should be easy to separate...
  if (ACE_OS::strncmp (corbaloc_name, rir_prefix,

                       sizeof (rir_prefix)-1) == 0)
    {

      //      CORBA::Object_ptr object = CORBA::Object::_nil ();

      // "rir" protocol used ... pass the key string as an
      // argument to the resolve_initial_references ()
      const char *key_string =
        corbaloc_name + sizeof (rir_prefix) -1;

      ACE_TRY
        {
          obj = orb->resolve_initial_references (key_string, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::SystemException, ex)
        {
          ACE_PRINT_EXCEPTION (ex, "CORBALOC_Parser:RIR:");
        }
      ACE_ENDTRY;
      
    }

  return obj;
}

int
TAO_CORBALOC_Parser::check_prefix (const char *endpoint)
{

  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1; // Failure

  const char *protocol[] = { "iiop:", "" };

  size_t slot = ACE_OS::strchr (endpoint, '/') - endpoint;

  size_t len0 = ACE_OS::strlen (protocol[0]);
  size_t len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint, protocol[0], len0) == 0)
    return 0;
  else if (slot == len1
           && ACE_OS::strncasecmp (endpoint, protocol[1], len1) == 0)
    return 0;

  return -1;
  // Failure: not an IIOP IOR
  // DO NOT throw an exception here.
}

ACE_FACTORY_DEFINE (TAO_IOR_CORBALOC, TAO_CORBALOC_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
