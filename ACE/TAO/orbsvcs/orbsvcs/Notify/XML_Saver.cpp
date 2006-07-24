// $Id$

#include "orbsvcs/Notify/XML_Saver.h"

#include "ACEXML/common/XML_Util.h"

#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  extern const char TOPOLOGY_ID_NAME[];

  XML_Saver::XML_Saver(bool timestamp)
    : output_ (0)
    , close_out_ (false)
    , backup_count_ (1)
    , timestamp_ (timestamp)
  {
  }

  XML_Saver::~XML_Saver()
  {
    if (this->output_ != 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) XML_Saver warning close not called or failed\n")
        ));
    }
  }

  void
  XML_Saver::backup_file_name (char * file_path, int nfile)
  {
    ACE_OS::snprintf(file_path, MAXPATHLEN, "%s.%3.3d",
      this->base_name_.c_str (),
      nfile);
  }

  void
  XML_Saver::close (ACE_ENV_SINGLE_ARG_DECL)
  {
    if (this->close_out_ && this->output_ != 0)
    {
      this->end_object(0, "notification_service" ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_OS::fclose(this->output_);
      this->output_ = 0;

      // delete the oldest backup file (if it exists)
      size_t nfile = this->backup_count_ - 1;
      char old_path [MAXPATHLEN + 1];
      backup_file_name (old_path, nfile);
      ACE_OS::unlink (old_path);

      while (nfile != 0)
      {
        char new_path [MAXPATHLEN + 1];
        nfile -= 1;
        backup_file_name (new_path, nfile);
        // this may fail, we don't care
        ACE_OS::rename (new_path, old_path);
        ACE_OS::strcpy (old_path, new_path);
      }
      // old_path now contains the name of the backup file
      ACE_CString xml_name = this->base_name_;
      xml_name += ".xml";

      ACE_OS::rename (xml_name.c_str (), old_path);

      ACE_CString new_name = this->base_name_;
      new_name += ".new";
      ACE_OS::rename (new_name.c_str (), xml_name.c_str ());
    }
    this->output_ = 0;
  }

  bool
  XML_Saver::open(const ACE_CString & base_name, size_t backup_count)
  {
    this->base_name_ = base_name;
    this->backup_count_ = backup_count;
    if (base_name ==  "cout")
    {
      this->output_ = stdout;
      this->close_out_ = false;
    }
    else if (base_name ==  "cerr")
    {
      this->output_ = stderr;
      this->close_out_ = false;
    }
    else
    {
      ACE_CString file_name = base_name;
      file_name += ".new";

      this->output_ = ACE_OS::fopen (file_name.c_str(), ACE_TEXT("wb"));
      if (this->output_) {
        this->close_out_ = true;
      } else {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("(%P|%t) XML_Saver unable to open %s\n"),
            base_name.c_str()));
      }
    }
    if (this->output_ != 0)
    {
      FILE *out = this->output_;

      ACE_OS::fprintf (out, "<?xml version=\"1.0\"?>\n");

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
      {
        bool changed = true;
        NVPList attrs;

        ACE_Time_Value now = ACE_High_Res_Timer::gettimeofday();

        ACE_UINT64 nowus = now.usec();
        static const ACE_UINT64 USECSPERSEC = 1000 * 1000;
        ACE_UINT64 tmpus = now.sec();
        nowus += tmpus * USECSPERSEC;

        char nowusstr[128];
#ifdef ACE_LACKS_LONGLONG_T
        nowus.as_string(nowusstr);
#else
        ACE_OS::sprintf(nowusstr, ACE_UINT64_FORMAT_SPECIFIER, nowus);
#endif /* ACE_LACKS_LONGLONG_T */

        attrs.push_back(NVP("version", "1.0"));
        if (this->timestamp_)
        {
          attrs.push_back(NVP("timestamp", nowusstr));
        }
        this->begin_object(0, "notification_service", attrs, changed ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      ACE_CATCHANY
      {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
          ACE_TEXT ("(%P|%t) XML_Saver Unknown exception\n"));
        delete this->output_;
        this->output_ = 0;
      }
      ACE_ENDTRY;
    }
    return this->output_ != 0;
  }

  bool XML_Saver::begin_object(CORBA::Long id,
    const ACE_CString& type,
    const NVPList& attrs,
    bool /* changed */
    ACE_ENV_ARG_DECL_NOT_USED)
  {
    ACE_ASSERT(this->output_ != 0);

    FILE *out = this->output_;

    ACE_OS::fprintf (out, "%s%s%s", indent_.c_str(), "<", type.c_str());
    if (id != 0)
    {
      // not all ostreams know what to do with a CORBA::Long
      long lid = id;
      ACE_OS::fprintf (out, " %s%s%ld%s", TOPOLOGY_ID_NAME, "=\"", lid, "\"");
    }

    const size_t BUF_SIZE = 512;
    ACE_CString tmp(BUF_SIZE);
    for (size_t idx = 0; idx < attrs.size(); idx++)
    {
      ACEXML_escape_string(attrs[idx].value, tmp);
      ACE_OS::fprintf (out, "%s%s%s%s%s", " ",
        attrs[idx].name.c_str (), "=\"", tmp.c_str(), "\"");
    }
    ACE_OS::fprintf (out, ">\n");
    this->indent_ += "  ";
    return true;
  }

  void XML_Saver::end_object (CORBA::Long /* id */,
                              const ACE_CString& type
                              ACE_ENV_ARG_DECL_NOT_USED)
  {
    ACE_ASSERT(this->output_ != 0);
    FILE *out = this->output_;
    if (this->indent_.length() >= 2)
    {
      this->indent_ = this->indent_.substr(2);
    }
    ACE_OS::fprintf (out, "%s%s%s%s", indent_.c_str(), "</",
                     type.c_str(), ">\n");
  }
} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL
