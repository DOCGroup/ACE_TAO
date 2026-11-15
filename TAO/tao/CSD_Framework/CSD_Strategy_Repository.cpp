// -*- C++ -*-
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/debug.h"

#include <memory>
#include <utility>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Strategy_Repository::~TAO_CSD_Strategy_Repository ()
{
  while (this->strategy_list_head_ != nullptr)
    {
      std::unique_ptr<Strategy_Node> node = std::move (this->strategy_list_head_);
      this->strategy_list_head_ = std::move (node->next_);
    }
}

int
TAO_CSD_Strategy_Repository::init (int, ACE_TCHAR **)
{
  return 0;
}

CSD_Framework::Strategy_ptr
TAO_CSD_Strategy_Repository::find (std::string const &name)
{
  Strategy_Node const *node = this->strategy_list_head_.get ();
  while (node != nullptr && node->poa_name_ != name)
    {
      node = node->next_.get ();
    }

  if (node != nullptr)
    return CSD_Framework::Strategy::_duplicate (node->strategy_.in ());

  return CSD_Framework::Strategy::_nil();
}

int
TAO_CSD_Strategy_Repository::add_strategy (std::string const &name,
                                           CSD_Framework::Strategy_ptr strat)
{
  Strategy_Node *node {};
  ACE_NEW_RETURN (node, Strategy_Node (name, strat), -1);
  std::unique_ptr<Strategy_Node> node_owner (node);
  std::unique_ptr<Strategy_Node> *tail = std::addressof (this->strategy_list_head_);
  while (*tail != nullptr)
    tail = std::addressof ((*tail)->next_);
  *tail = std::move (node_owner);

  if (TAO_debug_level > 3)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Strategy_Repository::add_strategy for %C\n"),
                  name.c_str ()));
    }
  return 0;
}

TAO_CSD_Strategy_Repository::Strategy_Node::Strategy_Node (
  std::string const &name,
  CSD_Framework::Strategy_ptr strategy)
  : poa_name_ (name),
    strategy_ (CSD_Framework::Strategy::_duplicate (strategy))
{
}

TAO_CSD_Strategy_Repository::Strategy_Node::~Strategy_Node () = default;

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
