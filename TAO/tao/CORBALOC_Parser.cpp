#include "CORBALOC_Parser.h"
#include "ORB_Core.h"
#include "Stub.h"
#include "MProfile.h"
#include "Connector_Registry.h"

#if !defined(__ACE_INLINE__)
#include "CORBALOC_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO,
           CORBALOC_Parser,
           "$Id$")

TAO_CORBALOC_Parser::~TAO_CORBALOC_Parser (void)
{
}

static const char corbaloc_prefix[] = "corbaloc:";
static const char iiop_prefix[] = "iiop:";
static const char uiop_prefix[] = "uiop:";
static const char shmiop_prefix[] = "shmiop:";
static const char miop_prefix[] = "miop:";
static const char rir_prefix[] = "rir:";

int
TAO_CORBALOC_Parser::match_prefix (const char *ior_string) const
{
  // Check if the prefix is 'corbaloc:' and return the result.
  return (ACE_OS::strncmp (ior_string,
                           corbaloc_prefix,
                           sizeof corbaloc_prefix - 1) == 0);
}

void
TAO_CORBALOC_Parser::parse_string_count_helper (const char * s,
                                                CORBA::ULong &addr_list_length,
                                                CORBA::ULong &addr_count
                                                TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char object_key_delimiter = '/';

  if (ACE_OS::strncmp (s, "uiop", 4) == 0)
      object_key_delimiter = '|';

  for (const char *i = s; *i != '\0'; ++i)
    {
      if (*i == ',')
        {
          // Increment the address count
          ++addr_count;
        }

      if (*i == ':'
          && *(i + 1) == '/'
          && *(i + 2) == '/')
        {
          if (TAO_debug_level > 0)
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t) Invalid Syntax: %s\n"),
                       s));

          ACE_THROW (CORBA::BAD_PARAM (TAO_OMG_VMCID | 10,
                                       CORBA::COMPLETED_NO));
        }

      if (*i == object_key_delimiter
          && *(i+1) != object_key_delimiter)
        {
          // Indication that the next characters are to be
          // assigned to key_string
          return;
        }

      ++addr_list_length;
    }
}

void
TAO_CORBALOC_Parser::assign_key_string (char *& cloc_name_ptr,
                                        ACE_CString & key_string,
                                        CORBA::ULong
                                        &addr_list_length,
                                        CORBA::ORB_ptr orb,
                                        TAO_MProfile &mprofile
                                        TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var end_point;
  const char protocol_prefix[] = ":";
  const char protocol_suffix_append[] = "://";
  const char iiop_prefix[] = "iiop";
  const char uiop_prefix[] = "uiop";
  const char def_port[] = ":2809";

  // Copy the cloc_name_ptr to cloc_name_cstring.
  ACE_CString cloc_name_cstring (cloc_name_ptr,
                                 addr_list_length,
                                 0,
                                 1);

  // pos_colon is the position of the ':' in the iiop_id
  // <iiop_id> = ":" | <iiop_prot_token>":"
  int pos_colon = cloc_name_cstring.find (':', 0);

  if (ACE_OS::strncmp (cloc_name_ptr,
                       protocol_prefix,
                       sizeof (protocol_prefix) - 1) == 0)
    {
      // If there is no protocol explicitly specified then default to
      // "iiop".
      end_point = CORBA::string_alloc (addr_list_length
                                       + sizeof (iiop_prefix) - 1
                                       + 1  // Object key separator
                                       + 3  // "://"
                                       + sizeof (def_port) - 1
                                       + key_string.length ());

      // Copy the default <iiop> prefix.
      ACE_OS::strcpy (end_point,
                      iiop_prefix);

      // Append '://'
      ACE_OS::strcat (end_point,
                      protocol_suffix_append);
    }
  else
    {
      // The case where the protocol to be used is explicitly
      // specified.

      // The space allocated for the default IIOP port is not needed
      // for all protocols, but it is only 5 bytes.  No biggy.
      end_point = CORBA::string_alloc (addr_list_length
                                       + 1  // Object key separator
                                       + 3  // "://"
                                       + sizeof (def_port) - 1
                                       + key_string.length ());

      ACE_CString prot_name = cloc_name_cstring.substring (0,
                                                           pos_colon);

      // Form the endpoint

      // Example:
      // prot_name.c_str () = iiop
      ACE_OS::strcpy (end_point,
                      prot_name.c_str ());;


      // Example:
      // The endpoint will now be of the form 'iiop'

      ACE_OS::strcat (end_point,
                      protocol_suffix_append);

      // The endpoint will now be of the form 'iiop://'
    }

  ACE_CString addr =
    cloc_name_cstring.substring (pos_colon + 1, -1);

  ACE_OS::strcat (end_point,
                  addr.c_str ());

  // Check for an IIOP corbaloc IOR.  If the port number is not
  // specified, append the default corbaloc port number (i.e. "2809")
  if (ACE_OS::strncmp (cloc_name_ptr,
                       iiop_prefix,
                       sizeof (iiop_prefix) - 1) == 0
      && addr.find (':') == ACE_CString::npos)
    ACE_OS::strcat (end_point, def_port);

  // Example:
  // The End_point will now be of the form
  //    'iiop://1.0@doc.ece.uci.edu:12345'

  if (ACE_OS::strncmp (cloc_name_ptr,
                       uiop_prefix,
                       sizeof (uiop_prefix) - 1) == 0)
    {
      // The separator for the uiop protocol is '|'. This should
      // actually be later changed so that the separator is '/' as the
      // other protocols.
      ACE_OS::strcat (end_point, "|");
    }
  else
    {
      ACE_OS::strcat (end_point, "/");
    }

  // Append the key string.
  ACE_OS::strcat (end_point,
                  key_string.c_str ());

  // Example: The End_point will now be of the form:
  // 'iiop://1.0@doc.ece.uci.edu:12345/object_name'

  // Call the mprofile helper which makes an mprofile for this
  // endpoint and adds it to the big mprofile.
  this->parse_string_mprofile_helper (end_point.in (),
                                      orb,
                                      mprofile
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CORBALOC_Parser::parse_string_assign_helper (
    CORBA::ULong &addr_list_length,
    ACE_CString &key_string,
    ACE_CString &cloc_name,
    CORBA::ORB_ptr orb,
    TAO_MProfile &mprofile
    TAO_ENV_ARG_DECL)
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
      this->assign_key_string (cloc_name_ptr,
                               key_string,
                               addr_list_length,
                               orb,
                               mprofile
                               TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Get the next token.
      cloc_name_ptr = ACE_OS::strtok_r (NULL,
                                        ",",
                                        &last_addr);
    }
}


void
TAO_CORBALOC_Parser::parse_string_mprofile_helper (
    const char * end_point,
    CORBA::ORB_ptr orb,
    TAO_MProfile &mprofile
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_MProfile jth_mprofile;

  int retv =
    orb->orb_core ()->connector_registry ()->make_mprofile (
      end_point,
      jth_mprofile
      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (retv != 0)
    {
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA_SystemException::_tao_minor_code (
                      TAO_DEFAULT_MINOR_CODE,
                      EINVAL),
                   CORBA::COMPLETED_NO));
    }

  TAO_MProfile *jth_mprofile_ptr = &jth_mprofile;

  /// Add this profile to the main mprofile.
  int result = mprofile.add_profiles (jth_mprofile_ptr);

  if (result == -1)
    {
      // The profile is not added.  Either way, go to the next
      // endpoint.
    }
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::make_stub_from_mprofile (CORBA::ORB_ptr orb,
                                              TAO_MProfile &mprofile
                                              TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create a TAO_Stub.
  TAO_Stub *data = orb->orb_core ()->create_stub ((const char *) 0,
                                                  mprofile
                                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub_Auto_Ptr safe_data (data);

  CORBA::Object_var obj = orb->orb_core ()->create_object (data);

  if (!CORBA::is_nil (obj.in ()))
    {
      /// All is well, so release the stub object from its
      /// auto_ptr.
      (void) safe_data.release ();

      /// Return the object reference to the application.
      return obj._retn ();
    }

  /// Shouldnt come here: if so, return nil reference.
  return CORBA::Object::_nil ();
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string_rir_helper (const char *& corbaloc_name,
                                              CORBA::ORB_ptr orb
                                              TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{


  // "rir" protocol. Pass the key string as an
  // argument to the resolve_initial_references.
  const char *key_string = corbaloc_name + sizeof ("rir:/") -1;

  if (ACE_OS::strcmp (key_string, "") == 0)
    {
      // If the key string is empty, assume the default
      // "NameService".
      key_string =  "NameService";
    }

  CORBA::Object_var rir_obj =
    orb->resolve_initial_references (key_string
                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return rir_obj._retn ();
}

int
TAO_CORBALOC_Parser::check_prefix (const char *end_point
                                   TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // This checks if the prefix is "rir:" or not. Returns a -1 if it is
  // "rir:" else returns a zero;
  // Check for a valid string
  if (!end_point || !*end_point)
    return -1; // Failure

  const char *protocol[] = {"rir:"};
  size_t slot = ACE_OS::strchr (end_point, '/') - end_point;
  size_t colon_slot = ACE_OS::strchr (end_point, ':') - end_point;
  size_t len0 = ACE_OS::strlen (protocol[0]);

  // Lets first check if it is a valid protocol:
  if (colon_slot != 0 &&
      !((ACE_OS::strncmp (end_point,
                         iiop_prefix,
                         sizeof iiop_prefix - 1) == 0) ||

        (ACE_OS::strncmp (end_point,
                          shmiop_prefix,
                          sizeof shmiop_prefix - 1) == 0) ||

        (ACE_OS::strncmp (end_point,
                          uiop_prefix,
                          sizeof uiop_prefix - 1) == 0) ||

        (ACE_OS::strncmp (end_point,
                          miop_prefix,
                          sizeof miop_prefix - 1) == 0) ||

        (ACE_OS::strncmp (end_point,
                          rir_prefix,
                          sizeof rir_prefix - 1) == 0)))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) ")
                    ACE_TEXT ("no usable transport protocol ")
                    ACE_TEXT ("was found.\n")));

      ACE_THROW_RETURN (CORBA::BAD_PARAM (TAO_OMG_VMCID | 10,
                                          CORBA::COMPLETED_NO),
                        -1);
    }

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (end_point, protocol[0], len0) == 0)
    return 0;

  return 1;
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string (const char * ior,
                                   CORBA::ORB_ptr orb
                                   TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// MProfile which consists of the profiles for each endpoint.
  TAO_MProfile mprofile;

  // Skip the prefix.  We know it is there because this method is only
  // called if match_prefix() returns 1.
  const char *corbaloc_name =
    ior + sizeof corbaloc_prefix - 1;

  CORBA::Object_ptr object = CORBA::Object::_nil ();

  // Number of endpoints
  CORBA::ULong count_addr = 1;

  // Length of obj_addr_list
  CORBA::ULong addr_list_length = 0;

  // If the protocol is not "rir:" and also is a valid protocol
  int check_prefix_result = this->check_prefix (corbaloc_name
                                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (check_prefix_result != 0)
    {
      // Count the length of the obj_addr_list and number of
      // endpoints in the obj_addr_list
      this->parse_string_count_helper (corbaloc_name,
                                       addr_list_length,
                                       count_addr
                                       TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      // Convert corbaloc_name to an ACE_CString
      ACE_CString corbaloc_name_str (corbaloc_name, 0, 1);

      // Get the key_string which is a substring of corbaloc_name_str
      ACE_CString key_string =
        corbaloc_name_str.substring (addr_list_length + 1);

      // Copy the <obj_addr_list> to cloc_name.
      ACE_CString cloc_name (corbaloc_name,
                             addr_list_length,
                             0,
                             1);

      // Get each endpoint: For each endpoint, make a MProfile and add
      // it to the main MProfile whose reference is passed to the
      // application
      this->parse_string_assign_helper (addr_list_length,
                                        key_string,
                                        cloc_name,
                                        orb,
                                        mprofile
                                        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      // Create the stub for the mprofile and get the object reference
      // to it which is to be returned to the client application.
      object = this->make_stub_from_mprofile (orb,
                                              mprofile
                                              TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  else
    {
      // RIR case:
      object = this->parse_string_rir_helper (corbaloc_name,
                                              orb
                                              TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }
  return object;
}

ACE_STATIC_SVC_DEFINE (TAO_CORBALOC_Parser,
                       ACE_TEXT ("CORBALOC_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CORBALOC_Parser),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_CORBALOC_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<char *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array_Base<char *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
