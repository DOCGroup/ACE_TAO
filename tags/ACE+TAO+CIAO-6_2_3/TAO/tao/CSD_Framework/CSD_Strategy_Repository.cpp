// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Strategy_Repository::TAO_CSD_Strategy_Repository() :
  strategy_list_head_ (0)
{
}


TAO_CSD_Strategy_Repository::~TAO_CSD_Strategy_Repository()
{
  delete this->strategy_list_head_;
}

int
TAO_CSD_Strategy_Repository::init(int, ACE_TCHAR **)
{

  static int initialized = 0;

  // Only allow initialization once.
  if (initialized)
    return 0;

  initialized = 1;
  this->strategy_list_head_ = 0;
  return 0;
}

CSD_Framework::Strategy_ptr
TAO_CSD_Strategy_Repository::find (const ACE_CString& name)
{

  if (this->strategy_list_head_ != 0)
    {
      Strategy_Node *node = this->strategy_list_head_->find(name);
      if (node != 0)
        return CSD_Framework::Strategy::_duplicate (node->strategy_.in());
    }

  return CSD_Framework::Strategy::_nil();
}


int
TAO_CSD_Strategy_Repository::add_strategy (const ACE_CString& name,
                                           CSD_Framework::Strategy_ptr strat)
{
  Strategy_Node *node = 0;
  ACE_NEW_RETURN (node, Strategy_Node(name,strat),-1);
  if (this->strategy_list_head_ == 0)
    this->strategy_list_head_ = node;
  else
    this->strategy_list_head_->add_node(node);

  if (TAO_debug_level > 3)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Strategy_Repository::add_strategy for %C\n"),
                  name.c_str ()));
    }
  return 0;
}

TAO_CSD_Strategy_Repository::Strategy_Node::Strategy_Node (const ACE_CString& name,
                                           CSD_Framework::Strategy_ptr strat)
  : poa_name_(name),
    strategy_(CSD_Framework::Strategy::_duplicate(strat)),
    next_(0)
{
}

TAO_CSD_Strategy_Repository::Strategy_Node::~Strategy_Node ()
{
  if (this->next_)
    delete this->next_;
}

void
TAO_CSD_Strategy_Repository::Strategy_Node::add_node(Strategy_Node *node)
{
  if (this->next_)
    this->next_->add_node(node);
  else
    this->next_ = node;
}

TAO_CSD_Strategy_Repository::Strategy_Node *
TAO_CSD_Strategy_Repository::Strategy_Node::find(const ACE_CString &name)
{
  if (this->poa_name_ == name)
    return this;
  if (this->next_)
    return this->next_->find(name);
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
