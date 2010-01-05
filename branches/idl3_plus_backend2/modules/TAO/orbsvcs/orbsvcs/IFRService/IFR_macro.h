/* -*- C++ -*- */
// $Id$

#ifndef TAO_IFR_MACROS_H
#define TAO_IFR_MACROS_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Constants.h"

#define TAO_IFR_WRITE_GUARD \
  ACE_WRITE_GUARD_THROW_EX (ACE_Lock, \
                            monitor, \
                            this->repo_->lock (), \
                            CORBA::INTERNAL ( \
                              CORBA::SystemException::_tao_minor_code ( \
                                TAO_GUARD_FAILURE, \
                                0), \
                              CORBA::COMPLETED_NO)); \

#define TAO_IFR_WRITE_GUARD_RETURN(RETURN) \
  ACE_WRITE_GUARD_THROW_EX (ACE_Lock, \
                            monitor, \
                            this->repo_->lock (), \
                            CORBA::INTERNAL ( \
                              CORBA::SystemException::_tao_minor_code ( \
                                TAO_GUARD_FAILURE, \
                                0), \
                              CORBA::COMPLETED_NO)); \

#define TAO_IFR_READ_GUARD_RETURN(RETURN) \
  ACE_READ_GUARD_THROW_EX (ACE_Lock, \
                           monitor, \
                           this->repo_->lock (), \
                           CORBA::INTERNAL ( \
                             CORBA::SystemException::_tao_minor_code ( \
                               TAO_GUARD_FAILURE, \
                               0), \
                             CORBA::COMPLETED_NO)); \

#include /**/ "ace/post.h"

#endif /* TAO_IFR_MACROS_H */
