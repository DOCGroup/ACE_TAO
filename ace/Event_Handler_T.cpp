// Event_Handler_T.cpp
// $Id$

#ifndef EVENT_HANDLER_T_C
#define EVENT_HANDLER_T_C

#include "ace/Event_Handler_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Event_Handler_T, "$Id$")

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

#if !defined (__ACE_INLINE__)
#include "ace/Event_Handler_T.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Event_Handler_T)

template <class T> void
ACE_Event_Handler_T<T>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Event_Handler_T<T>::dump");
#endif /* ACE_HAS_DUMP */
}

template<class T>
ACE_Event_Handler_T<T>::~ACE_Event_Handler_T (void)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::~ACE_Event_Handler_T");
  if (this->delete_handler_)
    delete this->op_handler_;
}

template <class T>
ACE_Event_Handler_T<T>::ACE_Event_Handler_T (T *op_handler, int delete_handler,
                                             GET_HANDLE get_handle,
                                             IO_HANDLER input_h,
                                             CL_HANDLER close_h,
                                             SIG_HANDLER sig_h,
                                             TO_HANDLER timeout_h,
                                             IO_HANDLER output_h,
                                             SET_HANDLE set_handle,
                                             IO_HANDLER except_h)
  : op_handler_ (op_handler),
    input_handler_ (input_h),
    output_handler_ (output_h),
    except_handler_ (except_h),
    to_handler_ (timeout_h),
    cl_handler_ (close_h),
    sig_handler_ (sig_h),
    delete_handler_ (delete_handler),
    set_handle_ (set_handle),
    get_handle_ (get_handle)
{
  ACE_TRACE ("ACE_Event_Handler_T<T>::ACE_Event_Handler_T");
}

#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

#endif /* EVENT_HANDLER_T_C */
