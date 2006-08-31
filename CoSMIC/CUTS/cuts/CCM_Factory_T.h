// -*- C++ -*-

//=============================================================================
/**
 * @file      CCM_Factory_T.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_CCM_FACTORY_T_H_
#define _CUTS_CCM_FACTORY_T_H_

#include "cuts/config.h"
#include "tao/LocalObject.h"

//=============================================================================
/**
 * @class CUTS_CCM_Factory_T
 *
 * Template factory that is parameterized by a home executor. This
 * simplifies the code generation of a CoWorkEr.
 */
//=============================================================================

template <typename HOME, typename EXEC>
class CUTS_CCM_Factory_T :
  virtual public HOME,
  virtual public TAO_Local_RefCounted_Object
{
public:
  /// Type definition of the home type.
  typedef HOME Home_Type;

  /// Type definition of the executor type.
  typedef EXEC Executor_Type;

  /// Default constructor.
  CUTS_CCM_Factory_T (void);

  /// Destructor.
  virtual ~CUTS_CCM_Factory_T (void);

  /**
   * Default creation method for the factory.
   *
   * @return    Reference to a component executor.
   */
  virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException));
};

//=============================================================================
/**
 * @def CUTS_FACTORY_EXPORT_DECLARE(export, func)
 *
 * Declare the export method for loading the executor factory.
 *
 * @param[in]     export      Name of the export marco.
 * @param[in]     func        Function to export.
 */
//=============================================================================

#define CUTS_FACTORY_EXPORT_DECLARE(export, func) \
  extern "C" export ::Components::HomeExecutorBase_ptr func (void)

//=============================================================================
/**
 * @def CUTS_FACTORY_EXPORT_DECLARE(export, func)
 *
 * Implement the export method for loading the executor factory.
 *
 * @param[in]     export      Name of the export marco.
 * @param[in]     func        Function to export.
 */
//=============================================================================

#define CUTS_FACTORY_EXPORT_IMPLEMENT(exec, func) \
::Components::HomeExecutorBase_ptr func (void) \
  { \
    ::Components::HomeExecutorBase_ptr retval = \
      ::Components::HomeExecutorBase::_nil (); \
    ACE_NEW_RETURN (retval, exec, ::Components::HomeExecutorBase::_nil ()); \
    return retval; \
  }

#include "cuts/CCM_Factory_T.cpp"

#endif  // !defined _CUTS_CCM_FACTORY_T_H_
