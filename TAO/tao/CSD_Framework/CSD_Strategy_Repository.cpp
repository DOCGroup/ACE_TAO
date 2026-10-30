// -*- C++ -*-
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/debug.h"

#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_CSD_Strategy_Repository::init(int, ACE_TCHAR **)
{
  static bool initialized {};

  // Only allow initialization once.
  if (initialized)
    return 0;

  initialized = true;
  return 0;
}

CSD_Framework::Strategy_ptr
TAO_CSD_Strategy_Repository::find (ACE_CString const &name)
{
  std::string const key (name.c_str ());
  Strategy_Map::const_iterator const iter = this->strategy_map_.find (key);
  if (iter != this->strategy_map_.end ())
    {
      return CSD_Framework::Strategy::_duplicate (iter->second.in ());
    }

  return CSD_Framework::Strategy::_nil();
}


int
TAO_CSD_Strategy_Repository::add_strategy (ACE_CString const &name,
                                           CSD_Framework::Strategy_ptr strat)
{
  try
    {
      std::string const key (name.c_str ());
      CSD_Framework::Strategy_var strategy (
        CSD_Framework::Strategy::_duplicate (strat));
      this->strategy_map_.try_emplace (key, strategy);
    }
  catch (std::bad_alloc const &)
    {
      return -1;
    }

  if (TAO_debug_level > 3)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Strategy_Repository::add_strategy for %C\n"),
                  name.c_str ()));
    }
  return 0;
}

/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_CSD_FW, TAO_CSD_Strategy_Repository)
ACE_STATIC_SVC_DEFINE (TAO_CSD_Strategy_Repository,
                       ACE_TEXT ("TAO_CSD_Strategy_Repository"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CSD_Strategy_Repository),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
