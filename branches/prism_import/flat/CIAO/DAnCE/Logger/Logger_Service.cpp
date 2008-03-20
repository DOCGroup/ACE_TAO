
#include "Logger_Service.h"
#include "ace/Get_Opt.h"
#include "ace/CORBA_macros.h"
#include "tao/SystemException.h"

namespace DAnCE
  {

  int
  Logger_Service::init (int argc, ACE_TCHAR * argv[])
  {
    //DANCE_DEBUG((LM_DEBUG, "[%M] Changing the logging backend.\n"));
    ACE_Get_Opt get_opts (argc, argv, "f:", 1, 0, ACE_Get_Opt::RETURN_IN_ORDER);
    get_opts.long_option ("log-file", 'f', ACE_Get_Opt::ARG_REQUIRED);
    int c;
    ACE_CString filename;
    while ( (c = get_opts ()) != -1)
      {
        switch (c)
          {
          case 'f':
            this->filename_ = get_opts.opt_arg ();
            break;
          }
      }
    if (0 == this->filename_.length())
      {
        this->filename_ = "dance.log";
      }

//    ACE_NEW_THROW_EX (this->my_backend_,
//                    File_Logger_Backend(filename.c_str()),
//                    CORBA::NO_MEMORY());
//    this->my_backend_->open(0);
//    this->prev_backend_ = ACE_Log_Msg::msg_backend (this->my_backend_);
//    ACE_Log_Msg * ace = ACE_Log_Msg::instance();
//    this->prev_flags_ = ace->flags();
//    ace->clr_flags (this->prev_flags_);
//    ace->set_flags (ACE_Log_Msg::CUSTOM);
    return 0;
  }

  ACE_Log_Msg_Backend *
  Logger_Service::get_logger_backend (CORBA::ORB_ptr)
  {
    File_Logger_Backend * the_backend;
    ACE_NEW_THROW_EX (the_backend,
                      File_Logger_Backend (this->filename_.c_str()),
                      CORBA::NO_MEMORY());
    return the_backend;
  }

} // DAnCE

using namespace DAnCE;
ACE_FACTORY_DEFINE (DAnCE_Logger, Logger_Service);

