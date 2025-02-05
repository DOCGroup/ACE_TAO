// -*- C++ -*-
#ifndef TAO_IMR_ACTIVATOR_LOADER_H
#define TAO_IMR_ACTIVATOR_LOADER_H

#include "ImR_Activator_i.h"
#include "Activator_Options.h"

#include "tao/Object_Loader.h"

#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ImR_Activator_ORB_Runner;

class Activator_Export ImR_Activator_Loader : public TAO_Object_Loader
{
public:
  ImR_Activator_Loader();

  virtual int init (int argc, ACE_TCHAR *argv[]);

  virtual int fini ();

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[]);

 // Unlike other service objects, we have our own orb.
 int run();

private:
  ImR_Activator_i service_;
  Activator_Options opts_;
  std::unique_ptr<ImR_Activator_ORB_Runner> runner_;

private:
  ImR_Activator_Loader (const ImR_Activator_Loader &) = delete;
  ImR_Activator_Loader &operator = (const ImR_Activator_Loader &) = delete;
  ImR_Activator_Loader (ImR_Activator_Loader &&) = delete;
  ImR_Activator_Loader &operator = (ImR_Activator_Loader &&) = delete;
};

ACE_FACTORY_DECLARE (Activator, ImR_Activator_Loader)

#endif
