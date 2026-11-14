// Define the DC_Singleton here

#ifndef __TEST_SINGLETON_H
#define __TEST_SINGLETON_H

#include "ace/Synch_Traits.h"

#include <memory>

class DC_Singleton
{
public:
  DC_Singleton () = default;
  DC_Singleton (DC_Singleton const&) = delete;
  DC_Singleton& operator= (DC_Singleton const&) = delete;
  DC_Singleton (DC_Singleton&&) = delete;
  DC_Singleton& operator= (DC_Singleton&&) = delete;
  void svc () {}
  static DC_Singleton *instance ();

private:
  static ACE_SYNCH_MUTEX lock_;
  static std::unique_ptr<DC_Singleton> instance_;
};

#endif /* __TEST_SINGLETON_H */
