// $Id$

/**
 * @file Log_Backend.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Provides a backend useable by the ACE logging infrastructure that transmits via NDDS.
 */

#ifndef DAnCE_LOG_BACKEND_H
#define DAnCE_LOG_BACKEND_H

#include <string>
#include <ndds/ndds_cpp.h>

#include "ace/Log_Msg_Backend.h"
#include "ace/Service_Object.h"

#include "DAnCE_NDDS_Log_Export.h"

#include "Log_RecordSupport.h"
#include "Log_RecordPlugin.h"

namespace DAnCE
{
  /**
   * @class NDDS_Log_Backend
   * @breif Provides a logging backend that transmits records over DDS/.
   */
  class DAnCE_NDDS_Log_Export NDDS_Log_Backend :
    public virtual ACE_Log_Msg_Backend,
    public virtual ACE_Service_Object
  {
  public:
    NDDS_Log_Backend (void);

    virtual ~NDDS_Log_Backend (void);

    // Methods from ACE_Log_Msg_Backend
    virtual int open (const ACE_TCHAR *logger_key);

    virtual int reset (void);

    virtual int close (void);

    virtual ssize_t log (ACE_Log_Record  &log_record);

    // Methods from ACE_Service_Object

    virtual int init (int argc, ACE_TCHAR **argv);

    virtual int fini (void);

    virtual int info (ACE_TCHAR **str, size_t len) const;

  private:
    int get_configuration (const ACE_TCHAR *logger_key);
    int configure_dds (void);

    std::string topic_name_;
    std::string node_;
    long domain_;

    DDSDomainParticipant *participant_;
    DDSTopic *topic_;
    DDSPublisher *publisher_;
    DDSDataWriter *datawriter_;
    Log_RecordDataWriter * log_record_writer_;
  };

  ACE_FACTORY_DECLARE (DAnCE_NDDS_Log, NDDS_Log_Backend);
}



#endif
