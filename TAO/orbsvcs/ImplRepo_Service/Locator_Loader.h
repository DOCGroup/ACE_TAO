// -*- C++ -*-
#ifndef TAO_IMR_LOCATOR_LOADER_H
#define TAO_IMR_LOCATOR_LOADER_H

#include "ImR_Locator_i.h"

#include "tao/Object_Loader.h"

#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ImR_Locator_ORB_Runner;

class Locator_Export ImR_Locator_Loader : public TAO_Object_Loader
{
public:
  ImR_Locator_Loader() = default;
  ~ImR_Locator_Loader();

  int init (int argc, ACE_TCHAR *argv[]) override;

  int fini () override;

  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc,
                                   ACE_TCHAR *argv[]) override;

 // Unlike other service objects, we have our own orb.
 int run();

private:
  ImR_Locator_i service_;
  Options opts_;
  std::unique_ptr<ImR_Locator_ORB_Runner> runner_;
private:
  ImR_Locator_Loader (const ImR_Locator_Loader &) = delete;
  ImR_Locator_Loader &operator = (const ImR_Locator_Loader &) = delete;
  ImR_Locator_Loader (ImR_Locator_Loader &&) = delete;
  ImR_Locator_Loader &operator = (ImR_Locator_Loader &&) = delete;
};

ACE_FACTORY_DECLARE (Locator, ImR_Locator_Loader)

#endif
