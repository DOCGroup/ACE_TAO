// $Id$

#include "FILE_Parser.h"
#include "Object_Loader.h"
#include "tao/Object.h"
#include "tao/Exception.h"
#include "tao/Environment.h"
#include "ace/Read_Buffer.h"

#if !defined(__ACE_INLINE__)
#include "FILE_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, FILE_Parser, "$Id$")

TAO_FILE_Parser::~TAO_FILE_Parser (void)
{
}

static const char file_prefix[] = "file:";

int
TAO_FILE_Parser::match_prefix (const char *ior_string) const
{
  return (ACE_OS::strncmp (str,
                           file_prefix,
                           sizeof file_prefix - 1) == 0);
}

CORBA::Object_ptr
TAO_FILE_Parser::parse_string (const char *ior,
                              CORBA::ORB_ptr orb,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  const char *filename =
    ior + sizeof file_prefix - 1;

  FILE* file = ACE_OS::fopen (filename, "r");

  if (file == 0)
    return CORBA::Object::_nil ();

  ACE_Read_Buffer reader (file, 1);

  char* string = reader.read ();

  if (string == 0)
    return CORBA::Object::_nil ();

  CORBA::Object_ptr object = CORBA::Object::_nil ();
  ACE_TRY
    {
      object = this->string_to_object (string, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      reader.alloc ()->free (string);
    }
  ACE_CATCHANY
    {
      reader.alloc ()->free (string);
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return object;
}

ACE_FACTORY_DEFINE (TAO_IOR_FILE, TAO_FILE_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
