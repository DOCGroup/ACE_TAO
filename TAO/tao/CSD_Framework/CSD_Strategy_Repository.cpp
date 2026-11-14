// -*- C++ -*-
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/debug.h"

#include <utility>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Strategy_Repository::~TAO_CSD_Strategy_Repository () = default;

int
TAO_CSD_Strategy_Repository::init (int, ACE_TCHAR **)
{
  return 0;
}

CSD_Framework::Strategy_ptr
TAO_CSD_Strategy_Repository::find (std::string const &name)
{
  if (this->strategy_list_head_ != nullptr)
    {
      Strategy_Node const * const node = this->strategy_list_head_->find (name);
      if (node != nullptr)
        return CSD_Framework::Strategy::_duplicate (node->strategy_.in ());
    }

  return CSD_Framework::Strategy::_nil();
}

int
TAO_CSD_Strategy_Repository::add_strategy (std::string const &name,
                                           CSD_Framework::Strategy_ptr strat)
{
  Strategy_Node *node {};
  ACE_NEW_RETURN (node, Strategy_Node (name, strat), -1);
  std::unique_ptr<Strategy_Node> node_owner (node);
  if (!this->strategy_list_head_)
    this->strategy_list_head_ = std::move (node_owner);
  else
    this->strategy_list_head_->add_node (std::move (node_owner));

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

void
TAO_CSD_Strategy_Repository::Strategy_Node::add_node (
  std::unique_ptr<Strategy_Node> node)
{
  if (this->next_ != nullptr)
    this->next_->add_node (std::move (node));
  else
    this->next_ = std::move (node);
}

TAO_CSD_Strategy_Repository::Strategy_Node const *
TAO_CSD_Strategy_Repository::Strategy_Node::find (std::string const &name) const
{
  if (this->poa_name_ == name)
    return this;
  if (this->next_ != nullptr)
    return this->next_->find (name);
  return nullptr;
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
