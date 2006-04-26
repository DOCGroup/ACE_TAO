// $Id$

#include "tao/Parser_Registry.h"
#include "tao/IOR_Parser.h"
#include "tao/ORB_Core.h"

#include "ace/Dynamic_Service.h"

#if !defined(__ACE_INLINE__)
#include "tao/Parser_Registry.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Parser_Registry,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Parser_Registry::TAO_Parser_Registry (void)
  : parsers_ (0),
    size_ (0)
{
}

TAO_Parser_Registry::~TAO_Parser_Registry (void)
{
  delete [] this->parsers_;
}

int
TAO_Parser_Registry::open (TAO_ORB_Core *orb_core)
{
  char **names;
  int number_of_names = 0;

  orb_core->resource_factory ()->get_parser_names (names,
                                                   number_of_names);

  if (number_of_names == 0)
    {
      return -1;
    }

  this->size_ = number_of_names;
  ACE_NEW_RETURN (this->parsers_,
                  TAO_IOR_Parser*[this->size_],
                  -1);

  for (size_t i = 0; i != this->size_; ++i)
    {
      this->parsers_[i] =
        ACE_Dynamic_Service<TAO_IOR_Parser>::instance (orb_core->configuration (),
                                                       names [i]);

      if (this->parsers_[i] == 0)
        {
          return -1;
        }
    }

  return 0;
}

TAO_IOR_Parser *
TAO_Parser_Registry::match_parser (const char *ior_string)
{
  for (Parser_Iterator i = this->begin (); i != this->end (); ++i)
    {
      if ((*i)->match_prefix (ior_string))
        {
          return *i;
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
