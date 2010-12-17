// $Id$

#include "tao/FILE_Parser.h"

#if (TAO_HAS_FILE_PARSER == 1)

#include "tao/ORB.h"
#include "tao/Object.h"

#include "ace/Read_Buffer.h"
#include "ace/Malloc_Base.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

static const char file_prefix[] = "file:";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FILE_Parser::~TAO_FILE_Parser (void)
{
}


bool
TAO_FILE_Parser::match_prefix (const char *ior_string) const
{
  return (ACE_OS::strncmp (ior_string,
                           ::file_prefix,
                           sizeof (::file_prefix) - 1) == 0);
}

CORBA::Object_ptr
TAO_FILE_Parser::parse_string (const char *ior, CORBA::ORB_ptr orb)
{
  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  const char *filename =
    ior + sizeof (::file_prefix)+1;

  FILE* file = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR (filename),
                              ACE_TEXT("r"));

  if (file == 0)
    return CORBA::Object::_nil ();

  ACE_Read_Buffer reader (file, true);

  char* string = reader.read ();

  if (string == 0)
    return CORBA::Object::_nil ();

  CORBA::Object_ptr object = CORBA::Object::_nil ();
  try
    {
      object = orb->string_to_object (string);

      reader.alloc ()->free (string);
    }
  catch (const ::CORBA::Exception&)
    {
      reader.alloc ()->free (string);
      throw;
    }

  return object;
}


ACE_STATIC_SVC_DEFINE (TAO_FILE_Parser,
                       ACE_TEXT ("FILE_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FILE_Parser),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_FILE_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_FILE_PARSER == 1 */

