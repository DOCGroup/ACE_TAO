/* -*- C++ -*- */
/**
 *  @file Factories_Define.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_FACTORIES_DEFINE_H
#define TAO_FACTORIES_DEFINE_H
#include "ace/pre.h"

#include "Command_Factory_T.h"

#define TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(export, command, factory_name) \
\
typedef TAO_Notify_Tests_Command_Factory_T <command> command##_Factory;\
\
ACE_STATIC_SVC_DECLARE_EXPORT (export, command##_Factory) \
\
ACE_FACTORY_DECLARE (export, command##_Factory) \
\
ACE_STATIC_SVC_DEFINE(command##_Factory, \
                      factory_name, \
                      ACE_SVC_OBJ_T, \
                      &ACE_SVC_NAME (command##_Factory), \
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ, \
                      0) \
\
ACE_FACTORY_DEFINE (export, command##_Factory) \
\


#include "ace/post.h"
#endif /* TAO_FACTORIES_DEFINE_H */
