// -*- C++ -*-
#include "tao/Parser_Registry.h"
#include "tao/IOR_Parser.h"
#include "tao/ORB_Core.h"

#include "ace/Dynamic_Service.h"

#include <new>
#include <stdexcept>

#if !defined(__ACE_INLINE__)
#include "tao/Parser_Registry.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Parser_Registry::TAO_Parser_Registry () = default;

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

  std::vector<TAO_IOR_Parser *> parsers;
  try
    {
      parsers.reserve (static_cast<std::size_t> (number_of_names));

      for (int i = 0; i != number_of_names; ++i)
        {
          TAO_IOR_Parser *parser =
            ACE_Dynamic_Service<TAO_IOR_Parser>::instance (
              orb_core->configuration (), names[i]);

          if (parser == nullptr)
            {
              if (TAO_debug_level >= 1)
                {
                  TAOLIB_DEBUG ((LM_DEBUG,
                                 "TAO (%P|%t) Failed to find Service Object"
                                 " for %C.\n",
                                 names[i]));
                }
              continue;
            }

          parsers.push_back (parser);
        }
    }
  catch (std::bad_alloc const&)
    {
      return -1;
    }
  catch (std::length_error const&)
    {
      return -1;
    }

  this->parsers_.swap (parsers);
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

  return nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
