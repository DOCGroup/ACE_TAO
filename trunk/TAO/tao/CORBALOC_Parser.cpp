// $Id$

#include "CORBALOC_Parser.h"

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
  // Check if the prefix is 'corbaloc:' and return the result.
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
TAO_CORBALOC_Parser::assign_key_string (char * &cloc_name_ptr,
                                        ACE_CString &key_string,
                                        CORBA::ULong
                                        &addr_list_length,
                                        CORBA::Environment
                                        &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var end_point;
  const char protocol_prefix[] = "//";
  const char iiop_prefix[] = "iiop:";
  const char uiop_prefix[] = "uiop:";

  if (ACE_OS::strncmp (cloc_name_ptr,
                       protocol_prefix,
                       sizeof (protocol_prefix)-1) == 0)
    {
      // If there is no protocol explicitly specified, it defaults
      // down to <iiop:> ... so allocate memory even for that and
      // later append the prefix.

      // Allocation of  memory
      end_point = CORBA::string_alloc (addr_list_length +
                                       sizeof (iiop_prefix) +
                                       1 + // For the seperator
                                       key_string.length ());

      // Copy the default <iiop:> prefix.
      ACE_OS::strcpy (end_point,
                      iiop_prefix);

      // Append the endpoint that is being passed.
      ACE_OS::strcat (end_point,
                      cloc_name_ptr);

    }
  else
    {
      // The case where the protocol to be used is explicitly
      // specified.

      // Allocate memory for the end_point.
      end_point = CORBA::string_alloc (addr_list_length +
                                       1 + // For the seperator
                                       key_string.length ());

      ACE_OS::strcpy (end_point,
                      cloc_name_ptr);

    }

  if (ACE_OS::strncmp (cloc_name_ptr,
                       uiop_prefix,
                       sizeof (uiop_prefix)-1) == 0)
    {
      // The separator for the uiop protocol is '|'. This should
      // actually be later changed so that the separator is '/' as the
      // other protocols.
      ACE_OS::strcat (end_point,
                      "|");
    }
  else
    {
      ACE_OS::strcat (end_point,
                      "/");
    }

  // Append the key string.
  ACE_OS::strcat (end_point,
                  key_string.c_str ());

  // Call the mprofile helper which makes an mprofile for this
  // endpoint and adds it to the big mprofile.
  this->parse_string_mprofile_helper (end_point,
                                      ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_CORBALOC_Parser::parse_string_assign_helper (CORBA::ULong
                                                 &addr_list_length,
                                                 ACE_CString &key_string,
                                                 ACE_CString &cloc_name,
                                                 CORBA::Environment
                                                 &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *cloc_name_ptr = 0;

  // Tokenize using "," as the seperator
  char *last_addr = 0;

  cloc_name_ptr =
    ACE_OS::strtok_r (ACE_const_cast (char *, cloc_name.c_str ()),
                      ",",
                      &last_addr);

  while (cloc_name_ptr != 0)
    {
      // Forms the endpoint and calls the mprofile_helper.
      assign_key_string (cloc_name_ptr,
                         key_string,
                         addr_list_length,
                         ACE_TRY_ENV);
      ACE_CHECK;

      // Get the next token.
      cloc_name_ptr = ACE_OS::strtok_r (NULL,
                                        ",",
                                        &last_addr);
    }
}


void
TAO_CORBALOC_Parser::parse_string_mprofile_helper (CORBA::String_var end_point,
                                                   CORBA::Environment
                                                   &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  TAO_MProfile jth_mprofile;
  // = new TAO_MProfile;

  int retv =
    this->orb_->orb_core ()->connector_registry ()->make_mprofile
    (end_point.in (),
     jth_mprofile,
     ACE_TRY_ENV);

  if (retv != 0)
    {
      ACE_THROW(CORBA::INV_OBJREF (
                  CORBA_SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                  CORBA::COMPLETED_NO));
    }


  // Add this profile to the main mprofile.
  TAO_MProfile *jth_mprofile_ptr = &jth_mprofile;

  int result = mprofile_.add_profiles (jth_mprofile_ptr);

  if (result == -1)
    {
      // The profle is not added. Either ways, go to the
      // next endpoint.
    }

}

CORBA::Object_ptr
TAO_CORBALOC_Parser::make_stub_from_mprofile (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  // Now make the TAO_Stub.
  TAO_Stub *data = 0;
  ACE_NEW_THROW_EX (data,
                    TAO_Stub ((char *) 0, this->mprofile_,
                              this->orb_->orb_core ()),
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub_Auto_Ptr safe_data (data);

  obj = this->orb_->orb_core ()->create_object (data);

  if (!CORBA::is_nil (obj))
    {
      // All is well, so release the stub object from its
      // auto_ptr.
      (void) safe_data.release ();

      // Return the object reference to the application.
      return obj;
    }

  // Shouldnt come here: if so, return nil reference.
  return CORBA::Object::_nil ();
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string_rir_helper (const char *
                                              &corbaloc_name,
                                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_ptr rir_obj = CORBA::Object::_nil ();

  // "rir" protocol. Pass the key string as an
  // argument to the resolve_initial_references.
  const char *key_string = corbaloc_name + sizeof ("rir:/") -1;
  
  if (ACE_OS::strcmp (key_string, "") == 0)
    {
      // If the key string is empty, assume the default
      // "NameService".
      key_string =  "NameService";
    }

  rir_obj = this->orb_->resolve_initial_references (key_string,
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  
  return rir_obj;
}

int
TAO_CORBALOC_Parser::check_prefix (const char *end_point)
{

  // This checks if the prefix is "rir:" or not. Returns a -1 if it is
  // "rir:" else returns a zero;
  // Check for a valid string
  if (!end_point || !*end_point)
    return -1; // Failure

  const char *protocol[] = {"rir:"};

  size_t slot = ACE_OS::strchr (end_point, '/') - end_point;

  size_t len0 = ACE_OS::strlen (protocol[0]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (end_point, protocol[0], len0) == 0)
    return 0;

  return 1;
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string (const char *ior,
                                   CORBA::ORB_ptr orb,
                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  const char *corbaloc_name =
    ior + sizeof corbaloc_prefix - 1;

  CORBA::Object_ptr object = CORBA::Object::_nil ();

  // No of endpoints
  CORBA::ULong count_addr = 1;

  // Length of obj_addr_list
  CORBA::ULong addr_list_length = 0;

  // If the protocol is not "rir:"
  if (this->check_prefix (corbaloc_name) != 0)
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
      ACE_CString key_string =
        corbaloc_name_str.substring ((addr_list_length+1), -1);

      // Copy the <obj_addr_list> to cloc_name.
      ACE_CString cloc_name (corbaloc_name,
                             addr_list_length,
                             0,
                             1);

      // Get each endpoint: For each endpoint, make a mprofile and add
      // it to the one big mprofile whose reference is passed to the
      // application
      this->parse_string_assign_helper (addr_list_length,
                                        key_string,
                                        cloc_name,
                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      // Create the stub for the mprofile and get the object reference
      // to it which is to be returned to the client application.
      object = this->make_stub_from_mprofile (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  else
    {
      // RIR case:
      object = this->parse_string_rir_helper (corbaloc_name,
                                              ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }

  return object;
}


ACE_FACTORY_DEFINE (TAO, TAO_CORBALOC_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<char *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
pragma instantiate ACE_Array_Base<char *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
