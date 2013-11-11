// -*- C++ -*-
// $Id$

#ifndef CIAO_SHAPES_CONTROLLERR_EXEC_ETSDUL_H_
#define CIAO_SHAPES_CONTROLLERR_EXEC_ETSDUL_H_

#include /**/ "ace/pre.h"

#include "Shapes_ControllerREC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ControllerR_exec_export.h"
#include "tao/LocalObject.h"


namespace CIAO_Shapes_ShapesControllerR_Impl
{

  class ControllerR_exec_i;

  class shapes_aggregates_i
    : public virtual CCM_shapes_aggregates,
      public virtual ::CORBA::LocalObject
  {
  public:
    shapes_aggregates_i (ControllerR_exec_i& component)
            : component_ (component)
    {
    }

    virtual ShapeAggrSeq* get_aggr_shapes();

  private:
    ControllerR_exec_i& component_;
  };

  class pulse_Generator : public ACE_Event_Handler
  {
  public:
    pulse_Generator (ControllerR_exec_i &callback);

    virtual ~pulse_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                  const void *arg);
  private:
      ControllerR_exec_i &pulse_callback_;
  };

  /**
   * Component Executor Implementation Class: Controller_exec_i
   */

  class ControllerR_exec_i
    : public virtual ShapesControllerR_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    ControllerR_exec_i (void);
    virtual ~ControllerR_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::CORBA::UShort rate (void);

    virtual void rate (::CORBA::UShort rate);

    virtual CCM_shapes_aggregates* get_shapeAggrPub (void);

   //@}

    //@{
    /** Operations from Components::SessionComponent. */
    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    //@{
    /** User defined public operations. */
    virtual void get_received_shapes (void);
    virtual ShapeAggrSeq* get_shapes_list(void);
    virtual void clear_shape_list();
    //@}

  private:
    ::Shapes::CCM_ShapesControllerR_Context_var ciao_context_;
    ShapeAggrSeq shape_aggr_seq_;
    TAO_SYNCH_MUTEX mutex_;

    //@{
    /** Component attributes. */
    ::CORBA::UShort rate_;
    //@}

    //@{
    /** User defined members. */
    pulse_Generator * ticker_;
    //@}

    //@{
    /** User defined private operations. */
    /// Get the ACE_Reactor
     ACE_Reactor* reactor (void);
     void aggregate_samples(const ShapeInfoSeq &info_seq);
    //@}
  };
}

extern "C" CONTROLLERR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_ControllerR_Impl (void);


#include /**/ "ace/post.h"

#endif /* ifndef */
