// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Parser_Registry.h"
#include "tao/IOR_Parser.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"

#if !defined(__ACE_INLINE__)
#include "tao/Parser_Registry.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Parser_Registry, "$Id$")

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
TAO_Parser_Registry::open (void)
{
  this->size_ = 1;
  ACE_NEW_RETURN (this->parsers_, TAO_IOR_Parser*[this->size_],
                  -1);

  this->parsers_[0] =
    ACE_Dynamic_Service<TAO_IOR_Parser>::instance ("DLL_Parser");

  if (this->parsers_[0] == 0)
    {
      int r = ACE_Service_Config::process_directive (
        "dynamic DLL_Parser Service_Object * TAO_IOR_DLL:_make_TAO_DLL_Parser()"
        );
      if (r == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error configuring DLL parser\n"), -1);
        }
      this->parsers_[0] =
        ACE_Dynamic_Service<TAO_IOR_Parser>::instance ("DLL_Parser");

    }


  // ACE_Service_Config::process_directive (
  // "dynamic TAO_FILE_Parser Service_Object * TAO_IOR_FILE:_make_TAO_FILE_Parser()"
  // );
  // this->parsers_[1] =
  //   ACE_Dynamic_Service<TAO_IOR_Parser>::instance
  //   ("TAO_FILE_Parser");

  return 0;
}

TAO_IOR_Parser *
TAO_Parser_Registry::match_parser (const char *ior_string)
{
  for (Parser_Iterator i = this->begin (); i != this->end (); ++i)
    {
      if ((*i)->match_prefix (ior_string))
        return *i;
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
