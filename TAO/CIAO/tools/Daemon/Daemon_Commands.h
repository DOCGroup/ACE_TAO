// -*- C++ -*-
// $Id$

/**
 * @file Daemon_Commands.h
 *
 * @brief Controller command implementations for general Daemon command
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_DAEMON_COMMANDS_H
#define CIAO_DAEMON_COMMANDS_H
#include "ace/pre.h"

#include "controller_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO
{
  namespace Daemon_i
  {
    /**
     * @class CMD_Install
     *
     * @brief Daemon command to install a component implementation.
     */
    class CMD_Install : public Command_Base
    {
    public:
      CMD_Install (controller *c);

      virtual ~CMD_Install ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    private:
      CORBA::String_var component_name_;
      CORBA::String_var location_;
    };

    /**
     * @class CMD_Get_IOR
     *
     * @brief Daemon command that returns the IOR of some service
     * available on a daemon.
     */
    class CMD_Get_IOR : public Command_Base
    {
    public:
      CMD_Get_IOR (controller *c);

      virtual ~CMD_Get_IOR ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    private:
      CORBA::String_var service_name_;
      CORBA::String_var filename_;
    };

    /**
     * @class CMD_Uninstall
     *
     * @brief Daemon command to uninstall a component implementation.
     */
    class CMD_Uninstall : public Command_Base
    {
    public:
      CMD_Uninstall (controller *c);

      virtual ~CMD_Uninstall ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    private:
      CORBA::String_var component_name_;
    };

    /**
     * @class CMD_Shutdown
     *
     * @brief Daemon command to shutdown the daemon process.
     */
    class CMD_Shutdown : public Command_Base
    {
    public:
      CMD_Shutdown (controller *c);

      virtual ~CMD_Shutdown ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    };

    /**
     * @class CMD_Shutdown
     *
     * @brief Daemon command to replace an component implementation with another one.
     */
    class CMD_Replace : public Command_Base
    {
    public:
      CMD_Replace (controller *c);

      virtual ~CMD_Replace ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    private:
      CORBA::String_var component_name_;
      CORBA::String_var location_;
    };

    /**
     * @class CMD_Shutdown
     *
     * @brief Daemon command for quering the location of an installed component.
     */
    class CMD_Query : public Command_Base
    {
    public:
      CMD_Query (controller *c);

      virtual ~CMD_Query ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    private:
      CORBA::String_var component_name_;
    };
  }
}


#if defined (__ACE_INLINE__)
# include "Daemon_Commands.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_DAEMON_COMMAND_H */
