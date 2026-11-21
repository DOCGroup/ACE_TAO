/* -*- c++ -*- */
#ifndef JAWS_OPTIONS_H
#define JAWS_OPTIONS_H

#include "ace/Singleton.h"
#include "ace/Synch.h"

#include "jaws3/Export.h"
#include "jaws3/Config_File.h"
#include <memory>

#define JAWS_DEFAULT_MIN_THYBRID_THREADS "1"
#define JAWS_DEFAULT_MAX_THYBRID_THREADS "-1"
#define JAWS_DEFAULT_TPOOL_THREADS "5"
#define JAWS_DEFAULT_IO "SYNCH"
#define JAWS_DEFAULT_CONCURRENCY "TPR"

class JAWS_Options;

class JAWS_Export JAWS_Options
{
public:
  JAWS_Options ();
  JAWS_Options (JAWS_Options const &) = delete;
  JAWS_Options (JAWS_Options &&) = delete;
  JAWS_Options &operator= (JAWS_Options const &) = delete;
  JAWS_Options &operator= (JAWS_Options &&) = delete;

  const char *getenv (const char *key);

  static JAWS_Options * instance ()
  {
    return ACE_Singleton<JAWS_Options, ACE_SYNCH_MUTEX>::instance ();
  }

private:
  std::unique_ptr<JAWS_Config_File> cf_;
};

#endif /* JAWS_OPTIONS_H */
