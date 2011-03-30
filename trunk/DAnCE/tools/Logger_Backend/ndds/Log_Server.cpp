// $Id$

/**
 * @file Log_Server.cpp
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * A simple server daemon that prints DAnCE log records received over DDS.
 */

#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/Get_Opt.h"
#include "ndds/ndds_cpp.h"
#include "Log_Record.h"
#include "Log_RecordSupport.h"
#include "Log_RecordPlugin.h"

#include <string>
#include <iostream>

struct Options
{
  std::string topic_;
  long domain_;
};

Options program_options;

bool parse_args (int argc, ACE_TCHAR **argv)
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
          program_options.topic_ = opts.opt_arg ();
          break;

        case 'd':
          program_options.domain_ = ACE_OS::atoi (opts.opt_arg ());
          break;

        default:
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unknown Option: %s\n"),
                      opts.last_option ()));
          return false;
        }

      return true;
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{

  if (!parse_args (argc, argv))
    {
      ACE_ERROR ((LM_EMERGENCY, ACE_TEXT ("Unable to parse commad line options\n")));
      return -1;
    }

  DDSDomainParticipant *participant_ = 0;
  DDSTopic *topic_ = 0;
  DDSSubscriber *subscriber_ = 0;
  DDSDataReader *datareader_ = 0;
  DAnCE::Log_RecordDataReader * log_record_reader_ = 0;

  try
    {
      participant_ = DDSDomainParticipantFactory::get_instance ()->
        create_participant (program_options.domain_,
                            DDS_PARTICIPANT_QOS_DEFAULT,
                            0,
                            DDS_STATUS_MASK_NONE);

      if (participant_ == 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to create participant\n")));
          return -1;
        }

      DDS_ReturnCode_t retval;

      retval = DAnCE::Log_RecordTypeSupport::register_type (participant_,
                                                            DAnCE::Log_RecordTypeSupport::get_type_name ());

      if (retval != DDS_RETCODE_OK)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to register type\n")));
          throw 1;
        }

      topic_ = participant_->create_topic (program_options.topic_.c_str (),
                                           DAnCE::Log_RecordTypeSupport::get_type_name (),
                                           DDS_TOPIC_QOS_DEFAULT,
                                           0,
                                           DDS_STATUS_MASK_NONE);

      if (topic_ == 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to create topic\n")));
          throw 1;
        }

      subscriber_ = participant_->create_subscriber (DDS_SUBSCRIBER_QOS_DEFAULT,
                                                     0,
                                                     DDS_STATUS_MASK_NONE);

      if (subscriber_ == 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to create publisher\n")));
          throw 1;
        }

      datareader_ = subscriber_->create_datareader (topic_,
                                                   DDS_DATAREADER_QOS_DEFAULT,
                                                   0,
                                                   DDS_STATUS_MASK_NONE);
      if (datareader_ == 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::open - Failed to open the datawriter\n")));
          throw 1;
        }

      log_record_reader_ = DAnCE::Log_RecordDataReader::narrow (datareader_);


      for (;;)
        {
          DAnCE::Log_RecordSeq records;
          DDS_SampleInfoSeq recordInfos;

          DDS_ReturnCode_t result = log_record_reader_->take (records,
                                                              recordInfos,
                                                              DDS_LENGTH_UNLIMITED,
                                                              DDS_ANY_SAMPLE_STATE,
                                                              DDS_ANY_VIEW_STATE,
                                                              DDS_ANY_INSTANCE_STATE);

          if (result == DDS_RETCODE_NO_DATA)
            {
              // try again
              continue;
            }
          else
            {
              for (size_t i = 0; i < records.length (); ++i)
                {
                  DDS_SampleInfo &info = recordInfos[i];

                  if (!info.valid_data)
                    continue;

                  std::cerr << records[i].node.in () << ':'
                            << records[i].pid << " - "
                            << records[i].message.in () << std::cerr;
                }
            }

          log_record_reader_->return_loan (records, recordInfos);
        }
    }
  catch (...)
    {
      if (participant_ != 0)
        {
          DDS_ReturnCode_t retval =
            participant_->delete_contained_entities ();

          if (retval != DDS_RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::close - Unable to delete participant contained entities\n")));
            }

          retval = DDSDomainParticipantFactory::get_instance ()->delete_participant (participant_);

          if (retval != DDS_RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("NDDS_Log_Backend::close - Unable to delete participant\n")));
            }
        }

      participant_ = 0;
      topic_ = 0;
      subscriber_ = 0;
      datareader_ = 0;
      log_record_reader_ = 0;

      return 0;
    }
}
