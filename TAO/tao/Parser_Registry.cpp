// -*- C++ -*-
#include "tao/Parser_Registry.h"
#include "tao/IOR_Parser.h"
#include "tao/ORB_Core.h"

#include "ace/Dynamic_Service.h"
#include <new>

#if !defined(__ACE_INLINE__)
#include "tao/Parser_Registry.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Parser_Registry::TAO_Parser_Registry () = default;

TAO_Parser_Registry::~TAO_Parser_Registry () = default;

int
TAO_Parser_Registry::open (TAO_ORB_Core *orb_core)
{
  char **names = nullptr;
  int number_of_names = 0;

  if (orb_core->resource_factory () == nullptr)
    {
      return -1;
    }

  orb_core->resource_factory ()->get_parser_names (names, number_of_names);

  if (number_of_names == 0)
    {
      return -1;
    }

  int const parser_count = number_of_names;
  this->parsers_.clear ();
  try
    {
      this->parsers_.reserve (static_cast<size_t> (parser_count));
    }
  catch (std::bad_alloc const&)
    {
      return -1;
    }

  for (int i = 0; i != parser_count; ++i)
    {
      TAO_IOR_Parser * const parser =
        ACE_Dynamic_Service<TAO_IOR_Parser>::instance (
          orb_core->configuration (), names[i]);

      if (parser == nullptr)
        {
          if (TAO_debug_level >= 1)
            {
              TAOLIB_DEBUG ((LM_DEBUG, "TAO (%P|%t) Failed to find Service Object"
                            " for %C.\n", names[i]));
            }
        }
      else
        {
          this->parsers_.push_back (parser);
        }
    }

  return 0;
}

TAO_IOR_Parser *
TAO_Parser_Registry::match_parser (const char *ior_string)
{
  for (TAO_IOR_Parser * const parser : this->parsers_)
    {
      if (parser->match_prefix (ior_string))
        {
          return parser;
        }
    }

  return nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
