
//=============================================================================
/**
 *  @file   Naming_Context_Interface.cpp
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Naming/Naming_Context_Interface.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Naming_Context::TAO_Naming_Context (TAO_Naming_Context_Impl *impl)
  : impl_ (impl)
{
}

TAO_Naming_Context::~TAO_Naming_Context (void)
{
  delete impl_;
}

PortableServer::POA_ptr
TAO_Naming_Context::_default_POA (/*env*/)
{
  return impl_->_default_POA ();
}

void
TAO_Naming_Context::bind (const CosNaming::Name &n, CORBA::Object_ptr obj)
{
  impl_->bind (n, obj);
}

void
TAO_Naming_Context::rebind (const CosNaming::Name &n, CORBA::Object_ptr obj)
{
  impl_->rebind (n, obj);
}

void
TAO_Naming_Context::bind_context (const CosNaming::Name &n,
                                  CosNaming::NamingContext_ptr nc)
{
  impl_->bind_context (n, nc);
}

void
TAO_Naming_Context::rebind_context (const CosNaming::Name &n,
                                    CosNaming::NamingContext_ptr nc)
{
  impl_->rebind_context (n, nc);
}

CORBA::Object_ptr
TAO_Naming_Context::resolve (const CosNaming::Name &n)
{
  return impl_->resolve (n);
}

void
TAO_Naming_Context::unbind (const CosNaming::Name &n)
{
  impl_->unbind (n);
}

CosNaming::NamingContext_ptr
TAO_Naming_Context::new_context (void)
{
  return impl_->new_context ();
}

CosNaming::NamingContext_ptr
TAO_Naming_Context::bind_new_context (const CosNaming::Name &n)
{
  return impl_->bind_new_context (n);
}

void
TAO_Naming_Context::destroy (void)
{
  impl_->destroy ();
}

void
TAO_Naming_Context::list (CORBA::ULong how_many,
                          CosNaming::BindingList_out bl,
                          CosNaming::BindingIterator_out bi)
{
  impl_->list (how_many, bl, bi);
}

void
TAO_Naming_Context::
to_string_helper_length (CORBA::ULong &len, const char * &src)
{
  for (const char *j =src; *j != '\0'; ++j)
    {
      ++len;
      if (*j == '.' || *j == '\\' || *j == '/')
        ++len;
    }
  ++len; // '.' seperator or '/' seperator

}

void
TAO_Naming_Context::
to_string_helper_assign (char * &k, const char * &src)
{
  for (; *src != '\0'; ++src)
    {
      if (*src == '.' || *src == '\\' || *src == '/')
        {
          *k = '\\';
          ++k;
        }
      *k = *src;
      ++k;
    }

}

char *
TAO_Naming_Context::to_string (const CosNaming::Name &n)
{
  // Accepts a Name and returns a stringified name.

  // Check for invalid name.
  if (n.length () == 0)
    throw CosNaming::NamingContext::InvalidName();

  // Length of the return string
  CORBA::ULong len = 0;

  CORBA::ULong i;
  for (i=0; i < n.length (); ++i)
    {
      const char *id = n[i].id.in ();

      // Count number of characters in id
      this->to_string_helper_length (len, id);

      const char *kind = n[i].kind.in ();

      // Count number of characters in kind
      this->to_string_helper_length (len, kind);
    }

  // Allocate memory to the return parameter
  //
  char *str_name = CORBA::string_alloc (len);

  // check for memory allocation
  //
  if (str_name == 0)
    {
      throw CORBA::NO_MEMORY ();
    }

  char *k = str_name;

  // Stringify the name
  for (i=0; i < n.length (); ++i)
    {

      // Stringify Id
      //
      const char *id = n[i].id.in ();
      this->to_string_helper_assign (k, id);

      const char *kind = n[i].kind.in ();

      if (*kind != '\0')
        {
          // If 'kind' is set,
          // Append a seperator between the id and kind.
          //
          *k = '.';
          ++k;

          // Stringify Kind
          //
          this->to_string_helper_assign (k, kind);
        }

      // If this is not the last name component, add a seperator
      // between the name components
      //
      if (i != (n.length ()-1))
        {
          *k = '/';
          ++k;
        }

    }
  // Terminate
  *k = '\0';
  ++k;

  return str_name;
}

void
TAO_Naming_Context::
to_name_helper (char *dest, const char*& src, Hint hint)
{
  for (; *src != '\0'; ++src, ++dest)
    {
      if ((hint == HINT_ID && *src == '.') || *src == '/')
        {
          *dest = '\0';
          return;
        }

      if (*src == '\\')
        {
          src++;
          if (*src == '\0')
            {
              // Case: The very last component
              *dest = '\0';
              return;
            }
        }

      // In all cases.
      *dest = *src;
    }

  // Terminate.
  *dest = '\0';
}

CosNaming::Name *
TAO_Naming_Context::to_name (const char *sn)
{
  // Returns the Name from its stringified form.
  CosNaming::Name n;

  // Total number of name components in the name
  CORBA::ULong ncomp = 0;

  // Total length of the unstrigified name
  CORBA::ULong len=0;


  for (const char *j = sn; *j != '\0'; ++j)
    {
      // Make a pass through the Name and count each character
      if (*j == '/')
        {
          ncomp++;
        }
      else if (*j == '\\')
        {
          ++j;

          if (*j == '\0')
            {
              // Case: The very last component
              ++len;
            }
        }

      // In all cases.
      ++len;
    }


  // Check for InvalidName i.e. Invalid stringified name
  //
  if (len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // Assign the length of the return unstringified name.
  //
  n.length (ncomp+1);

  // Keeps track of the number of the name component
  //
  CORBA::ULong count = 0;

  for (const char *k = sn; *k != '\0';)
    {
      if (count > ncomp)
        {
          throw CosNaming::NamingContext::InvalidName();
        }

      char *id = CORBA::string_alloc (len);
      char *kind = CORBA::string_alloc (len);

      // Assign to the id.
      this->to_name_helper (id, k, HINT_ID);

      if (*k == '.')
        {
          k++;
          // Assign to kind
          this->to_name_helper (kind, k, HINT_KIND);
        }
      else
        {
          *kind = '\0';
        }

      n[count].id   = id;
      n[count].kind = kind;

      count++;

      // End
       if (*k == '\0')
         break;
      k++;
    }

  return new CosNaming::Name (n);
}

int
TAO_Naming_Context::to_url_is_alnum_or_punctuation (char c)
{
  if (ACE_OS::ace_isalnum (c))
    return 1;

  // NON US-ASCII characters excluding those in this array are the
  // characters that need to be escaped
  static char non_escaped_punctuation[] =
    { ';', '/', ':', '?', '@', '=', '+', '$', ',', '-',
      '_', '.', '!', '~', '*', '\'', '(', ')' };
  const size_t non_escaped_punctuation_count =
    sizeof(non_escaped_punctuation)/sizeof(non_escaped_punctuation[0]);
  for (const char *j = non_escaped_punctuation;
       j != non_escaped_punctuation + non_escaped_punctuation_count;
       ++j)
    {
      // But if the character is one of the 18 non US-ASCII characters
      // and hence need not be escaped, then don't increment the
      // count.
      if (*j == c)
        return 1;
    }
  return 0;
}

size_t
TAO_Naming_Context::to_url_validate_and_compute_size (
    const char *addr,
    const char *sn)
{
  size_t addr_len = ACE_OS::strlen (addr);

  // Check for invalid address
  if (addr_len == 0)
    throw CosNaming::NamingContextExt::InvalidAddress ();

  // Make a pass through the in string name to count the number of
  // characters and if the character
  // is to be escaped, increment the number of characters by 3.
  size_t sn_len = 0;
  for (const char *i = sn; *i != '\0'; ++i)
    {
      ++sn_len;

      if (TAO_Naming_Context::to_url_is_alnum_or_punctuation (*i))
        continue;
      sn_len += 3;
    }

  if (sn_len == 0)
    throw CosNaming::NamingContextExt::InvalidName ();

  return addr_len + sn_len;
}

char *
TAO_Naming_Context::to_url (const char * addr,
                            const char * sn)
{
  /// Compute how many characters will be required for the URL
  size_t no_char =
    TAO_Naming_Context::to_url_validate_and_compute_size (addr, sn);


  // The 'corbaname:' tag is to be prepended at the starting of the
  // return parameter.
  //
  char prefix []= "corbaname:";

  // Allocate dynamic memory
  //
  char *str_url = CORBA::string_alloc (static_cast<CORBA::ULong> (no_char + sizeof (prefix)));

  // Copy 'prefix' to the return parameter.
  char *dest = ACE_OS::strcpy (str_url , prefix);

  /// move to end of dest string
  dest += ACE_OS::strlen(dest);

  // Concatenate the address
  dest = ACE_OS::strcat (dest, addr);

  /// Concatenate the seperator between the addr and Name
  dest += ACE_OS::strlen(dest);
  dest = ACE_OS::strcat (dest, "#");

  /// move to end of dest string
  dest += ACE_OS::strlen(dest);

  // Now append the stringified object name to the return variable.
  // The percent '%' character is used as an escape. If a character
  // that requires escaping is present in a name component it is
  // encoded as two hexadecimal digits following a '%' character to
  // represent the octet. The first hexadecimal character represents
  // the low-order nibble of the octet and the second hexadecimal
  // character represents the low order nibble.

  for (const char *i = sn; *i != '\0'; ++i)
    {
      if (TAO_Naming_Context::to_url_is_alnum_or_punctuation (*i))
        {
          // If the character is a US-ASCII Alphanumeric value...
          *dest = *i; ++dest;
          continue;
        }
      // this must be an escaped character

      *dest = '%'; ++dest;

      // Append the hexadecimal representation of the character.
      *dest = static_cast<char> (ACE::nibble2hex ((*i) >> 4)); ++dest;
      *dest = static_cast<char> (ACE::nibble2hex (*i)); ++dest;
    }

  // Terminate the string
  *dest = '\0';

  //  ACE_OS::strcat (str_url, dest);
  return str_url;
}

CORBA::Object_ptr
TAO_Naming_Context::resolve_str (const char * n)
{
  // Similar to <resolve> above. It accepts a strigified name as an
  // argument instead of a Name.

  // Get the unstrigified name.
  CosNaming::Name_var name = this->to_name (n);

  // Pass this unstringified name to resolve and return an Object_ptr
  return this->resolve (name.in ());
}

void
TAO_Naming_Context::stale (bool value)
{
  this->impl_->stale (value);
}

TAO_Naming_Context_Impl::~TAO_Naming_Context_Impl (void)
{
}

void
TAO_Naming_Context_Impl::stale (bool value)
{
  ACE_UNUSED_ARG (value);
  // Default implementation is no-op
}

bool
TAO_Naming_Context_Impl::stale (void)
{
  // Default implementation is to reply false
  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
