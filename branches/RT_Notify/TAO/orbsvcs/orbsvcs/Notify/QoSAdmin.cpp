// $Id$

#include "QoSAdmin.h"
#include "Container.h"
#include "ThreadPool_Task.h"

#if ! defined (__ACE_INLINE__)
#include "QoSAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_QoSAdmin, "$Id$")

TAO_NS_QoSAdmin::TAO_NS_QoSAdmin (void)
{
}

TAO_NS_QoSAdmin::~TAO_NS_QoSAdmin ()
{
}

void
TAO_NS_QoSAdmin::apply_qos (TAO_NS_Container* container, const CosNotification::QoSProperties & qos  ACE_ENV_ARG_DECL)
{
  for (CORBA::ULong index = 0; index < qos.length (); ++index)
    {
      ACE_CString property_name (qos[index].name);

      if (property_name.compare (NotifyExt::ThreadPool) == 0)
      {
          // check if ThreadPool is required.
          NotifyExt::ThreadPoolParams* tp_params = 0;

          qos[index].value >>= tp_params;

          this->apply_threadpool_qos (container, tp_params ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else if (property_name.compare (NotifyExt::ThreadPoolLanes) == 0)
        {
          NotifyExt::ThreadPoolLanesParams* tpl_params = 0;

          qos[index].value >>= tpl_params;

          this->apply_threadpool_lane_qos (container, tpl_params ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
TAO_NS_QoSAdmin::apply_threadpool_qos (TAO_NS_Container* container, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL)
{
  // @@ TODO??: check if number of threads is 0, if so, set as reactive.

  TAO_NS_ThreadPool_Task* worker_task;

  ACE_NEW_THROW_EX (worker_task,
                    TAO_NS_ThreadPool_Task (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  worker_task->init (tp_params ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  container->worker_task_own (worker_task);
}

void
TAO_NS_QoSAdmin::apply_threadpool_lane_qos (TAO_NS_Container* container, NotifyExt::ThreadPoolLanesParams* tpl_params ACE_ENV_ARG_DECL)
{
  // No lane support
  ACE_THROW ((CORBA::NO_IMPLEMENT ()));
}
