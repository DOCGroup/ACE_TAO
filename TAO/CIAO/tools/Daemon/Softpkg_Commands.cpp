// $Id$

#include "Softpkg_Commands.h"
#include "ace/Get_Opt.h"
#include "CCM_DeploymentC.h"

#if !defined (__ACE_INLINE__)
# include "Softpkg_Commands.inl"
#endif /* __ACE_INLINE__ */

//=================================================================

CIAO::Daemon_i::CMD_Start_Home::~CMD_Start_Home ()
{
}

int
CIAO::Daemon_i::CMD_Start_Home::parse_args (int ,
                                            char *[]
                                            ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
CIAO::Daemon_i::CMD_Start_Home::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->controller_->daemon ()->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}

//=================================================================

CIAO::Daemon_i::CMD_End_Home::~CMD_End_Home ()
{
}

int
CIAO::Daemon_i::CMD_End_Home::parse_args (int ,
                                          char *[]
                                          ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
CIAO::Daemon_i::CMD_End_Home::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->controller_->daemon ()->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}

//=================================================================
