/* -*- C++ -*- $Id$ */
#include "orbsvcs/orbsvcs/Notify/Notify_FilterAdmin_i.h"

// Implementation skeleton constructor
TAO_Notify_FilterAdmin_i::TAO_Notify_FilterAdmin_i (void)
  {
  }

// Implementation skeleton destructor
TAO_Notify_FilterAdmin_i::~TAO_Notify_FilterAdmin_i (void)
  {
  }

CosNotifyFilter::FilterID TAO_Notify_FilterAdmin_i::add_filter (
    CosNotifyFilter::Filter_ptr new_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

void TAO_Notify_FilterAdmin_i::remove_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ))

  {
    //Add your implementation here
  }

CosNotifyFilter::Filter_ptr TAO_Notify_FilterAdmin_i::get_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyFilter::FilterIDSeq * TAO_Notify_FilterAdmin_i::get_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

void TAO_Notify_FilterAdmin_i::remove_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

{
  //Add your implementation here
}
