// $Id$
#include "Log_Backend.h"

#include "ace/Env_Value_T.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"
#include "ace/Get_Opt.h"

namespace DAnCE
{
  NDDS_Log_Backend::NDDS_Log_Backend (void)
    : domain_ (0),
      participant_ (0),
      topic_ (0),
      publisher_ (0),
      datawriter_ (0),
      log_record_writer_ (0)
  {
  }

  NDDS_Log_Backend::~NDDS_Log_Backend (void)
  {
    (void) this->close ();
  }

  int
  NDDS_Log_Backend::init (int argc, ACE_TCHAR **argv)
  {
    ACE_Get_Opt opts (argc, argv, ACE_TEXT ("t:d:"), 1, 0,
                      ACE_Get_Opt::RETURN_IN_ORDER);
    opts.long_option (ACE_TEXT ("topic"), 't', ACE_Get_Opt::ARG_REQUIRED);
    opts.long_option (ACE_TEXT ("domain"), 'd', ACE_Get_Opt::ARG_REQUIRED);

    int c = 0;

    while ((c = opts ()) != -1)
      {
        switch (c)
          {
          case 't':
            this->topic_name_ = opts.opt_arg ();
            break;

          case 'd':
            this->domain_ = ACE_OS::atoi (opts.opt_arg ());
            break;

          default:
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unknown Option: %s\n"),
                        opts.last_option ()));
            return -1;
          }

        return 0;
      }
  }

  int
  NDDS_Log_Backend::fini (void)
  {
    return 0;
  }

  int
  NDDS_Log_Backend::info (ACE_TCHAR **, size_t) const
  {
    return 0;
  }

  int
  NDDS_Log_Backend::get_configuration (const ACE_TCHAR *)
  {
    // for now, we ignore logger key until I figure out what to do with it.
    ACE_Env_Value<long> env_domain (ACE_TEXT("DANCE_LOG_NDDS_DOMAIN"), this->domain_);
    this->domain_ = env_domain;

    ACE_Env_Value<const ACE_TCHAR *> env_topic (ACE_TEXT ("DANCE_LOG_NDDS_TOPIC"),
                                                ACE_TEXT_CHAR_TO_TCHAR (this->topic_name_.c_str ()));
    this->topic_name_ = ACE_TEXT_ALWAYS_CHAR (env_topic);

    ACE_Env_Value<const ACE_TCHAR *> env_node (ACE_TEXT ("DANCE_LOG_NDDS_NODE_NAME"),
                                               ACE_TEXT_CHAR_TO_TCHAR (this->node_.c_str ()));
    this->node_ = ACE_TEXT_ALWAYS_CHAR (env_node);

    return 0;
  }

  int
  NDDS_Log_Backend::configure_dds (void)
  {
    this->participant_ = DDSDomainParticipantFactory::get_instance ()->
      create_participant (this->domain_,
                          DDS_PARTICIPANT_QOS_DEFAULT,
                          0,
                          DDS_STATUS_MASK_NONE);

    if (this->participant_ == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to create participant\n")));
        return -1;
      }

    DDS_ReturnCode_t retval;

    retval = Log_RecordTypeSupport::register_type (this->participant_,
                                                   Log_RecordTypeSupport::get_type_name ());

    if (retval != DDS_RETCODE_OK)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to register type\n")));
        (void) this->close ();
        return -1;
      }

    this->topic_ = this->participant_->create_topic (this->topic_name_.c_str (),
                                                     Log_RecordTypeSupport::get_type_name (),
                                                     DDS_TOPIC_QOS_DEFAULT,
                                                     0,
                                                     DDS_STATUS_MASK_NONE);

    if (this->topic_ == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to create topic\n")));
        this->close ();
        return -1;
      }

    this->publisher_ = this->participant_->create_publisher (DDS_PUBLISHER_QOS_DEFAULT,
                                                             0,
                                                             DDS_STATUS_MASK_NONE);

    if (this->publisher_ == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to create publisher\n")));
        this->close ();
        return -1;
      }

    this->datawriter_ = this->publisher_->create_datawriter (this->topic_,
                                                             DDS_DATAWRITER_QOS_DEFAULT,
                                                             0,
                                                             DDS_STATUS_MASK_NONE);
    if (this->datawriter_ == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to open the datawriter\n")));
        this->close ();
        return -1;
      }

    log_record_writer_ = Log_RecordDataWriter::narrow (this->datawriter_);

    return 0;
  }

  int
  NDDS_Log_Backend::open (const ACE_TCHAR *logger_key)
  {
    if (this->get_configuration (logger_key) != 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - unable to get configuration\n")));
        return -1;
      }

    if (this->configure_dds () != 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - unable to configure DDS entities\n")));
        return -1;
      }

    return 0;
  }

  int
  NDDS_Log_Backend::reset (void)
  {
    return this->close ();
  }

  int
  NDDS_Log_Backend::close (void)
  {
    if (this->participant_ != 0)
      {
        DDS_ReturnCode_t retval =
          this->participant_->delete_contained_entities ();

        if (retval != DDS_RETCODE_OK)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::close - Unable to delete participant contained entities\n")));
          }

        retval = DDSDomainParticipantFactory::get_instance ()->delete_participant (this->participant_);

        if (retval != DDS_RETCODE_OK)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::close - Unable to delete participant\n")));
          }
      }

    this->participant_ = 0;
    this->topic_ = 0;
    this->publisher_ = 0;
    this->datawriter_ = 0;
    this->log_record_writer_ = 0;

    return 0;
  }

  ssize_t
  NDDS_Log_Backend::log (ACE_Log_Record  &log_record)
  {
    Log_Record *instance (0);

    instance = Log_RecordTypeSupport::create_data_ex (DDS_BOOLEAN_FALSE);

    if (instance == 0)
      {
        ACE_ERROR ((LM_EMERGENCY, ACE_TEXT ("Unable to create data sample for log record\n")));
        return 0;
      }

    instance->node = this->node_.c_str ();
    instance->pid = log_record.pid ();
    instance->pid = log_record.pid ();
    instance->message = ACE_TEXT_ALWAYS_CHAR (log_record.msg_data ());

    DDS_ReturnCode_t retval = this->log_record_writer_->write (*instance, DDS_HANDLE_NIL);

    if (retval != DDS_RETCODE_OK)
      {
        ACE_ERROR ((LM_EMERGENCY, ACE_TEXT ("Unable to write log record to DDS\n")));
        return 0;
      }

    if (instance != 0)
      {
        Log_RecordTypeSupport::delete_data_ex (instance,
                                               DDS_BOOLEAN_FALSE);
      }

    return log_record.msg_data_len ();
  }

  ACE_FACTORY_DEFINE (DAnCE_NDDS_Log, NDDS_Log_Backend);
}


