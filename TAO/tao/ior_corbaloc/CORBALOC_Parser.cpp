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

void
TAO_CORBALOC_Parser::parse_string_count_helper (const char * &corbaloc_name,
                                                CORBA::ULong &addr_list_length,
                                                CORBA::ULong
                                                &count_addr,
                                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::Boolean start_key_string = 1;
  
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

}

void
TAO_CORBALOC_Parser::assign_iiop_prefix_key_string (ACE_Array_Base <char *> &addr,
                                                    char * &cloc_name_ptr,
                                                    ACE_CString &key_string,
                                                    CORBA::ULong &current_addr,
                                                    CORBA::ULong &addr_list_length)
{
  
  // If there is no port number specified, append the default port
  // number i.e. 2809.
  char *port_seperator = ACE_OS::strchr (cloc_name_ptr,
                                         ':');

  const char iiop_prefix[] = "iiop:";
  
  if (port_seperator == 0)
    {
      // Implies that there is no seperator
      // append the default port
      
      const char def_port [] = ":2809";
      
      // Allocation of  memory
      addr [current_addr] =
        CORBA::string_alloc (addr_list_length +
                             sizeof (iiop_prefix) +
                             sizeof (def_port) +
                             key_string.length ());
              
      // If there is no explicit protocol specified, prepend the
      // default "iiop:"
      ACE_OS::strcpy (addr [current_addr],
                      iiop_prefix);
      
      ACE_OS::strcat (addr [current_addr],
                      cloc_name_ptr);
      
      // Append the def_port
      ACE_OS::strcat (addr [current_addr],
                      def_port);
    }
  else
    {
      // If there is no explicit protocol specified, prepend the
      // default "iiop:"
      
      // Allocation of  memory
      addr [current_addr] =
        CORBA::string_alloc (addr_list_length +
                             sizeof (iiop_prefix) +
                             key_string.length ());
      
      ACE_OS::strcpy (addr [current_addr],
                      iiop_prefix);
      
      ACE_OS::strcat (addr [current_addr],
                      cloc_name_ptr);
    }
  
}

void
TAO_CORBALOC_Parser::assign_key_string (ACE_Array_Base <char *> &addr,
                                        char * &cloc_name_ptr,
                                        ACE_CString &key_string,
                                        CORBA::ULong &current_addr,
                                        CORBA::ULong &addr_list_length)
{
  
  // If there is no port number specified, append the default port
  // number i.e. 2809.
  char *port_seperator = ACE_OS::strrchr (cloc_name_ptr,
                                    ':');
  
  if (*(port_seperator+1) == '/')
    {
      // Implies that there is no seperator
      // append the default port
      const char def_port [] = ":2809";
      
      // Allocation of  memory
      addr [current_addr] = 
        CORBA::string_alloc (addr_list_length +
                             sizeof (def_port) +
                             key_string.length ());
      
      ACE_OS::strcpy (addr [current_addr],
                      cloc_name_ptr);
      
      // Append the default port.
      ACE_OS::strcat (addr [current_addr],
                      def_port);
    }
  else
    {
      // Allocation of  memory
      addr [current_addr] = 
        CORBA::string_alloc (addr_list_length +
                             key_string.length ());
      
      // If the protocol is <iiop:>
      ACE_OS::strcpy (addr [current_addr],
                      cloc_name_ptr);
      
    }
}

void
TAO_CORBALOC_Parser::parse_string_assign_helper (ACE_Array_Base <char *> &addr,
                                                 CORBA::ULong &addr_list_length,
                                                 ACE_CString &key_string,
                                                 ACE_CString &cloc_name,
                                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *cloc_name_ptr = 0;
  CORBA::ULong current_addr = 0;

  // Tokenize using "," as the seperator
  char *last_addr = 0;
  cloc_name_ptr = ACE_OS::strtok_r (ACE_const_cast (char *, cloc_name.c_str ()),
                                    ",",
                                    &last_addr);

  const char iiop_prefix[] = "iiop:";

  while (cloc_name_ptr != 0)
    {
      // Loop till we get all the endpoints.
      if (ACE_OS::strncmp (cloc_name_ptr,
                           iiop_prefix,
                           sizeof (iiop_prefix)-1) != 0)
        {
          assign_iiop_prefix_key_string (addr,
                                         cloc_name_ptr,
                                         key_string,
                                         current_addr,
                                         addr_list_length);
        }
      else
        {
          assign_key_string (addr,
                             cloc_name_ptr,
                             key_string,
                             current_addr,
                             addr_list_length);
        }

      ACE_OS::strcat (addr [current_addr],
                      key_string.c_str ());
      
      ++current_addr;
      // Get the next token.
      cloc_name_ptr = ACE_OS::strtok_r (NULL,
                                        ",",
                                        &last_addr);
    }
}


CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string_mprofile_helper (ACE_Array_Base <char *>  &addr,
                                                   CORBA::ULong &count_addr,
                                                   CORBA::ORB_ptr orb,
                                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // Make one MProfile for all of these endpoints
  TAO_MProfile mprofile;

  for (CORBA::ULong j = 0; j != count_addr; ++j)
    {
      CORBA::Object_ptr obj = CORBA::Object::_nil ();

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

      // Check if this reference is a valid one, if it is a valid
      // reference, return the reference.
      if (obj->_is_a ("IDL:omg.org/CORBA/Object:1.0"))
          return obj;
    }

  return CORBA::Object::_nil ();
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string_rir_helper (const char *
                                              &corbaloc_name,
                                              CORBA::ORB_ptr orb,
                                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  const char rir_prefix [] = "rir:/";
  CORBA::Object_ptr rir_obj = CORBA::Object::_nil ();

  // "rir" protocol. Pass the key string as an
  // argument to the resolve_initial_references ()
  const char *key_string =
    corbaloc_name + sizeof (rir_prefix) -1;
  
  if (ACE_OS::strcmp (key_string, "") == 0)
    {
      // If the key string is empty, assume the default
      // "NameService".
      key_string =  "NameService";
    }

  rir_obj = orb->resolve_initial_references (key_string,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  
  return rir_obj;
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

  CORBA::Object_ptr object = CORBA::Object::_nil ();

  // No of endpoints
  CORBA::ULong count_addr = 1;
  
  // Length of obj_addr_list
  CORBA::ULong addr_list_length = 0;
  
  // If the protocol is "iiop:",
  if (this->check_prefix (corbaloc_name) == 0)
    {
      // Count the length of the obj_addr_list and number of
      // endpoints in the obj_addr_list
      this->parse_string_count_helper (corbaloc_name,
                                       addr_list_length,
                                       count_addr,
                                       ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
      
      // Convert corbaloc_name as a ACE_CString
      ACE_CString corbaloc_name_str (corbaloc_name, 0, 1);
      
      // Get the key_string which is a substring of corbaloc_name_str
      ACE_CString key_string = corbaloc_name_str.substring (addr_list_length, -1);
      
      // Array of <obj_addr>
      ACE_Array_Base<char*> addr (count_addr);
      
      // Copy the <obj_addr_list> to cloc_name.
      ACE_CString cloc_name (corbaloc_name,
                             addr_list_length,
                             0,
                             1);
      
      // Assign the <obj_addr> to the array elements
      this->parse_string_assign_helper (addr,
                                        addr_list_length,
                                        key_string,
                                        cloc_name,
                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
      
      // Get the Ptr to the NameService
      object = this->parse_string_mprofile_helper (addr,
                                                   count_addr,
                                                   orb,
                                                   ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  else
    {
      // RIR case:
      object = this->parse_string_rir_helper (corbaloc_name,
                                              orb,
                                              ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  
  return object;
}


ACE_FACTORY_DEFINE (TAO_IOR_CORBALOC, TAO_CORBALOC_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<char *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
pragma instantiate ACE_Array_Base<char *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
