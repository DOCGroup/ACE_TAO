// -*- C++ -*-
// $Id$

/**
 * @file Softpkg_Commands.h
 *
 * @brief Controller commands implementations for creating and
 * removing component server for a single home in a softpkg.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_SOFTPKG_COMMANDS_H
#define CIAO_SOFTPKG_COMMANDS_H
#include /**/ "ace/pre.h"

#include "controller_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO
{
  namespace Daemon_i
  {
    /**
     * @class CMD_Start_Home
     *
     * @brief Daemon command to start up a component server for hosting
     * a component home as specified in a softpkg descriptor.
     */
    class CMD_Start_Home : public Command_Base
    {
    public:
      CMD_Start_Home (controller *c);

      virtual ~CMD_Start_Home ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

      void print_usage (const char *name) const;

    private:
      CORBA::String_var softpkg_filename_;

      CORBA::String_var home_ior_filename_;

      CORBA::String_var com_server_ior_filename_;
    };

    /**
     * @class CMD_End_Home
     *
     * @brief Daemon command to end a component server that hosts a component home.
     */
    class CMD_End_Home : public Command_Base
    {
    public:
      CMD_End_Home (controller *c);

      virtual ~CMD_End_Home ();

      virtual int parse_args (int argc,
                              char *argv[]
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

      void print_usage (const char *name) const;

    private:
      CORBA::String_var com_server_ior_;
    };
  }
}


#if defined (__ACE_INLINE__)
# include "Softpkg_Commands.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_SOFTPKG_COMMANDS_H */
