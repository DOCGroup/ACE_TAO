// $Id$

#include "Notify_Default_Collection_Factory.h"
#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Immediate_Changes.h"
#include "orbsvcs/ESF/ESF_Copy_On_Write.h"
#include "Notify_Factory.h"

ACE_RCSID(Notify, Notify_Default_Collection_Factory, "$Id$")

typedef TAO_ESF_Proxy_List<TAO_Notify_EventListener>::Iterator TAO_Notify_EventListener_List_Iterator;
typedef TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>::Iterator TAO_Notify_UpdateListener_List_Iterator;

int
TAO_Notify_Default_Collection_Factory::init_svc (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_Notify_Default_Collection_Factory);
}

TAO_Notify_Default_Collection_Factory::TAO_Notify_Default_Collection_Factory (void)
{
}

TAO_Notify_Default_Collection_Factory::~TAO_Notify_Default_Collection_Factory ()
{
}

int
TAO_Notify_Default_Collection_Factory::init (int /*argc*/, ACE_TCHAR* /*argv*/[])
{
  return 0;
}

int
TAO_Notify_Default_Collection_Factory::fini (void)
{
  return 0;
}

TAO_Notify_EventListener_List*
TAO_Notify_Default_Collection_Factory::create_event_listener_list (ACE_ENV_SINGLE_ARG_DECL_NOT_USED/*ACE_ENV_SINGLE_ARG_PARAMETER*/)
{
  // @@ Pradeep: the copy_on_write versions are a much better choice
  // for a default than the immediate changes version.  The latter
  // fails with any sort of recursive calls.

  TAO_Notify_EventListener_List* listener_list =
    new TAO_ESF_Copy_On_Write<TAO_Notify_EventListener,
    TAO_ESF_Proxy_List<TAO_Notify_EventListener>,
    TAO_Notify_EventListener_List_Iterator,
    ACE_SYNCH> ();
  return listener_list;

  /*
  new TAO_ESF_Immediate_Changes<TAO_Notify_EventListener,
    TAO_ESF_Proxy_List<TAO_Notify_EventListener>,
    TAO_Notify_EventListener_List_Iterator,
    TAO_SYNCH_MUTEX> ();

ACE_NEW_THROW_EX (listener_list,
                    TAO_ESF_Immediate_Changes<TAO_Notify_EventListener,
                    TAO_ESF_Proxy_List<TAO_Notify_EventListener>,
                    TAO_Notify_EventListener_List_Iterator,
                    TAO_SYNCH_MUTEX> (),
                    CORBA::NO_MEMORY ());*/
  // TODO: create other kinds of lists.
}

TAO_Notify_UpdateListener_List*
TAO_Notify_Default_Collection_Factory::create_update_listener_list (ACE_ENV_SINGLE_ARG_DECL_NOT_USED/*ACE_ENV_SINGLE_ARG_PARAMETER*/)
{

  /*  TAO_Notify_UpdateListener_List* listener_list =
    new TAO_ESF_Immediate_Changes<TAO_Notify_UpdateListener,
    TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>,
    TAO_Notify_UpdateListener_List_Iterator,
    TAO_SYNCH_MUTEX> ();  //  ACE_SYNCH_NULL_MUTEX

  return listener_list;
  */

  TAO_Notify_UpdateListener_List* listener_list =
    new TAO_ESF_Copy_On_Write<TAO_Notify_UpdateListener,
    TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>,
    TAO_Notify_UpdateListener_List_Iterator,
    ACE_SYNCH> ();
  return listener_list;

}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Notify_Default_Collection_Factory,
                       ACE_TEXT (TAO_NOTIFY_DEF_COLLECTION_FACTORY_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Notify_Default_Collection_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Notify, TAO_Notify_Default_Collection_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Worker<TAO_Notify_EventListener>;
template class TAO_ESF_Worker<TAO_Notify_UpdateListener>;

template class TAO_ESF_Immediate_Changes<TAO_Notify_EventListener, TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener *>, TAO_SYNCH_MUTEX>;
template class TAO_ESF_Immediate_Changes<TAO_Notify_UpdateListener, TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener *>, TAO_SYNCH_MUTEX>;

template class TAO_ESF_Copy_On_Write<TAO_Notify_EventListener, TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener *>, ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write<TAO_Notify_UpdateListener, TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener *>, ACE_SYNCH>;

template class TAO_ESF_Copy_On_Write_Write_Guard<TAO_ESF_Proxy_List<TAO_Notify_EventListener>,
  TAO_Notify_EventListener_List_Iterator,
  ACE_SYNCH>;

template class TAO_ESF_Copy_On_Write_Write_Guard<TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>,
  TAO_Notify_UpdateListener_List_Iterator,
  ACE_SYNCH>;

template class TAO_ESF_Copy_On_Write_Collection<TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener *> >;

template class TAO_ESF_Copy_On_Write_Collection<TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener *> >;

template class TAO_ESF_Proxy_List<TAO_Notify_EventListener>;
template class TAO_ESF_Proxy_Collection<TAO_Notify_EventListener>;

template class TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>;
template class TAO_ESF_Proxy_Collection<TAO_Notify_UpdateListener>;

template class TAO_ESF_Copy_On_Write_Read_Guard<TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener*>, TAO_SYNCH_MUTEX>;

template class TAO_ESF_Copy_On_Write_Read_Guard<TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener*>, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_Worker<TAO_Notify_EventListener>
#pragma instantiate TAO_ESF_Worker<TAO_Notify_UpdateListener>

#pragma instantiate TAO_ESF_Immediate_Changes<TAO_Notify_EventListener, TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener *>, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_ESF_Immediate_Changes<TAO_Notify_UpdateListener, TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener *>, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_ESF_Immediate_Changes<TAO_Notify_UpdateListener, TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener *>, ACE_Null_Mutex>

#pragma instantiate TAO_ESF_Copy_On_Write<TAO_Notify_EventListener, TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener *>, ACE_SYNCH>
#pragma instantiate TAO_ESF_Copy_On_Write<TAO_Notify_UpdateListener, TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener *>, ACE_SYNCH>

#pragma instantiate TAO_ESF_Copy_On_Write_Write_Guard<
TAO_ESF_Proxy_List<TAO_Notify_EventListener>,
  TAO_Notify_EventListener_List_Iterator,
  ACE_SYNCH>
#pragma instantiate TAO_ESF_Copy_On_Write_Write_Guard<
TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>,
  TAO_Notify_UpdateListener_List_Iterator,
  ACE_SYNCH>

#pragma instantiate TAO_ESF_Copy_On_Write_Collection<TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener *> >

#pragma instantiate TAO_ESF_Copy_On_Write_Collection<TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener *> >

#pragma instantiate TAO_ESF_Proxy_List<TAO_Notify_EventListener>
#pragma instantiate TAO_ESF_Proxy_Collection<TAO_Notify_EventListener>

#pragma instantiate TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>
#pragma instantiate TAO_ESF_Proxy_Collection<TAO_Notify_UpdateListener>

#pragma instantiate TAO_ESF_Copy_On_Write_Read_Guard<TAO_ESF_Proxy_List<TAO_Notify_EventListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_EventListener*>, TAO_SYNCH_MUTEX>

#pragma instantiate TAO_ESF_Copy_On_Write_Read_Guard<TAO_ESF_Proxy_List<TAO_Notify_UpdateListener>, ACE_Unbounded_Set_Iterator<TAO_Notify_UpdateListener*>, TAO_SYNCH_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
