//$Id$

#include "FP_DT_Creator.h"
#include "test.h"

FP_DT_Creator::FP_DT_Creator (void)
{
  DT_TEST::instance ()->dt_creator (this);
}

CORBA::Policy_ptr
FP_DT_Creator::sched_param (int importance)
{
  return DT_TEST::instance ()->scheduler ()->create_segment_scheduling_parameter (importance);
}

ACE_STATIC_SVC_DEFINE(FP_DT_Creator,
                      ACE_TEXT ("FP_DT_Creator"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (FP_DT_Creator),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (FP_DT_Creator, FP_DT_Creator)


