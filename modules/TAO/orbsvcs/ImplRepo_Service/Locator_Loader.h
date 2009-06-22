// -*- C++ -*-
//
// $Id$

#ifndef TAO_IMR_LOCATOR_LOADER_H
#define TAO_IMR_LOCATOR_LOADER_H

#include "ImR_Locator_i.h"

#include "tao/Object_Loader.h"

#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ImR_Locator_ORB_Runner;

class Locator_Export ImR_Locator_Loader : public TAO_Object_Loader
{
public:
  ImR_Locator_Loader();

  virtual int init (int argc, ACE_TCHAR *argv[]);

  virtual int fini (void);

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[]);

 // Unlike other service objects, we have our own orb.
 int run(void);

private:
  ImR_Locator_i service_;
  Options opts_;
  ACE_Auto_Ptr<ImR_Locator_ORB_Runner> runner_;
private:
  // Disallow copying and assignment.
  ImR_Locator_Loader (const ImR_Locator_Loader &);
  ImR_Locator_Loader &operator = (const ImR_Locator_Loader &);
};

ACE_FACTORY_DECLARE (Locator, ImR_Locator_Loader)

#endif
