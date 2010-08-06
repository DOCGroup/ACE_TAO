// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Implementation: AppSideMonitor_Handler
 *
 *  @brief Defines implementation of AppSideMonitor_Handler.
 *
 */

#include "AppSideMonitor_Handler.h"

AppSideMonitor_Handler::AppSideMonitor_Handler (void)
  : acceptor_ (0)
{
}

int AppSideMonitor_Handler::handle_input (ACE_HANDLE)
{
  char ch;

  if (this->peer ().recv (&ch, sizeof (ch)) <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("It looks like the ")
                  ACE_TEXT ("monitor failed!\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("It looks like the monitor ")
                  ACE_TEXT ("is misbehaving!\n")));
    }

  return -1;
}

int AppSideMonitor_Handler::open (void *factory)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("AppSideMonitor_Handler::open\n")));
  
  acceptor_ = static_cast <FactoryAcceptor *> (factory);

  return super::open (factory);
}

