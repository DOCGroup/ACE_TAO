// -*- C++ -*-
// $Id$

/**
 * @file controller_i.h
 *
 * @brief Controller command implementations for CIAO's daemon controller.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_CONTROLLER_I_H
#define CIAO_CONTROLLER_I_H
#include "ace/pre.h"

#include "DaemonC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Daemon_i
  {
    class Command_Base;

    /**
     * @class controller
     *
     * @brief Daemon Controller.
     */
    class controller
    {
    public:
      controller (void);

      ~controller (void);

      int parse_args (int argc, char *argv[] ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      int init (int argc,
                char *argv[]
                ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

      int fini (void);

      void print_usage (void);

      int write_IOR (const char *ior,
                     const char *filename);

      /// Does not increase reference count.
      Daemon_ptr daemon ();
      CORBA::ORB_ptr orb ();

    protected:
      CORBA::ORB_var orb_;

      Command_Base *cmd_;

      Daemon_var daemon_;
    };

    /**
     * @class Command
     *
     * @brief Abstract base for CIAO daemon controller supported commands.
     */
    class Command
    {
    public:
      Command (controller *c);

      virtual ~Command () = 0;

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) = 0;

    protected:
      controller *controller_;
    };

    /**
     * @class Command_Base
     *
     * @brief Abstract base for CIAO daemon controller supported commands.
     */
    class Command_Base
    {
    public:
      Command_Base (controller *c);

      virtual ~Command_Base () = 0;

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) = 0;

    protected:
      controller *controller_;
    };
  }
}


#if defined (__ACE_INLINE__)
# include "controller_i.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_CONTROLLER_I_H */
