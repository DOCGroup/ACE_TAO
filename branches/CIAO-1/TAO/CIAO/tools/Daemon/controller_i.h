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

#include "daemonC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO
{
  class Command;

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

    Command *cmd_;

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

  class CMD_Install : public Command
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

  class CMD_Get_IOR : public Command
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

  class CMD_Uninstall : public Command
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

  class CMD_Shutdown : public Command
  {
  public:
    CMD_Shutdown (controller *c);

    virtual ~CMD_Shutdown ();

    virtual int parse_args (int argc,
                            char *argv[]
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  };

  class CMD_Replace : public Command
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

  class CMD_Query : public Command
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


#if defined (__ACE_INLINE__)
# include "controller_i.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_CONTROLLER_I_H */
