// $Id$

#include "ace/Service_Config.h"
#include "tao/orbconf.h"
#include "Transport_Listener_Manager.h"

// #if !defined (__ACE_INLINE__)
// # include "Transport_Current_Listener.inl"
// #endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           Transport_Current_Listener,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO::Transport::Listener::~Listener (void)
{
}

int
TAO::Transport::Listener_Manager::insert (TAO::Transport::Listener* listener)
{
  return this->collection_.insert_tail (listener);
}

int
TAO::Transport::Listener_Manager::remove (TAO::Transport::Listener* listener)
{
  return this->collection_.remove (listener);
}

int
TAO::Transport::Listener_Manager::notify (const TAO_Transport* t)
{
  if (this->collection_.is_empty ())
    return 0;

  Listener_Collection::ITERATOR i(this->collection_);

  for (Listener** tmp = 0;
       i.next (tmp);
       i.advance ())
    {
      if (tmp != 0 && *tmp != 0)
        (*tmp)->notify (t);
    }

  return 0;
}

TAO::Transport::Listener_Manager::~Listener_Manager (void)
{
}


TAO_END_VERSIONED_NAMESPACE_DECL


ACE_STATIC_SVC_DEFINE (TAO_Transport_Listener_Manager,
                       ACE_TEXT ("TAO_Transport_Listener_Manager"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Transport_Listener_Manager),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_NAMESPACE_DEFINE (TAO, TAO_Transport_Listener_Manager, ::TAO::Transport::Listener_Manager)

