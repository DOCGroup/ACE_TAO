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
  XML_Saver::backup_file_name (ACE_TCHAR * file_path, size_t nfile)
  {
    ACE_OS::snprintf(file_path, MAXPATHLEN, ACE_TEXT("%s.%3.3d"),
      this->base_name_.c_str (),
      static_cast<int> (nfile));
  }

  void
  XML_Saver::close (void)
  {
    if (this->close_out_ && this->output_ != 0)
    {
      this->end_object(0, "notification_service");

      ACE_OS::fclose(this->output_);
      this->output_ = 0;

      // delete the oldest backup file (if it exists)
      size_t nfile = this->backup_count_ - 1;
      ACE_TCHAR old_path [MAXPATHLEN + 1];
      backup_file_name (old_path, nfile);
      ACE_OS::unlink (old_path);

      while (nfile != 0)
      {
        ACE_TCHAR new_path [MAXPATHLEN + 1];
        nfile -= 1;
        backup_file_name (new_path, nfile);
        // this may fail, we don't care
        ACE_OS::rename (new_path, old_path);
        ACE_OS::strcpy (old_path, new_path);
      }
      // old_path now contains the name of the backup file
      ACE_TString xml_name = this->base_name_;
      xml_name += ACE_TEXT(".xml");

      ACE_OS::rename (xml_name.c_str (), old_path);

      ACE_TString new_name = this->base_name_;
      new_name += ACE_TEXT(".new");
      ACE_OS::rename (new_name.c_str (), xml_name.c_str ());
    }
    this->output_ = 0;
  }

  bool
  XML_Saver::open(const ACE_TString & base_name, size_t backup_count)
  {
    this->base_name_ = base_name;
    this->backup_count_ = backup_count;
    if (base_name ==  ACE_TEXT("cout"))
    {
      this->output_ = stdout;
      this->close_out_ = false;
    }
    else if (base_name ==  ACE_TEXT("cerr"))
    {
      this->output_ = stderr;
      this->close_out_ = false;
    }
    else
    {
      ACE_TString file_name = base_name;
      file_name += ACE_TEXT(".new");

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
      FILE * const out = this->output_;

      ACE_OS::fprintf (out, "<?xml version=\"1.0\"?>\n");

      try
      {
        bool changed = true;
        NVPList attrs;

        ACE_Time_Value const now = ACE_High_Res_Timer::gettimeofday();

        ACE_UINT64 nowus = now.usec();
        static const ACE_UINT64 USECSPERSEC = 1000 * 1000;
        ACE_UINT64 const tmpus = now.sec();
        nowus += tmpus * USECSPERSEC;

        ACE_TCHAR nowusstr[128];
#ifdef ACE_LACKS_LONGLONG_T
        nowus.as_string(nowusstr);
#else
        ACE_OS::sprintf(nowusstr, ACE_UINT64_FORMAT_SPECIFIER, nowus);
#endif /* ACE_LACKS_LONGLONG_T */

        attrs.push_back(NVP("version", "1.0"));
        if (this->timestamp_)
        {
          attrs.push_back(NVP("timestamp", ACE_TEXT_ALWAYS_CHAR(nowusstr)));
        }
        this->begin_object(0, "notification_service", attrs, changed);
      }
      catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception (
          ACE_TEXT (
            "(%P|%t) XML_Saver Unknown exception\n"));
        if (this->close_out_ && this->output_ != 0)
          {
            (void) ACE_OS::fclose (this->output_);
          }

        this->output_ = 0;
      }
    }
    return this->output_ != 0;
  }

  bool
  XML_Saver::begin_object(CORBA::Long id,
                          const ACE_CString& type,
                          const NVPList& attrs,
                          bool /* changed */)
  {
    ACE_ASSERT(this->output_ != 0);

    FILE * const out = this->output_;

    ACE_OS::fprintf (out, "%s%s%s", indent_.c_str(), "<", type.c_str());
    if (id != 0)
    {
      // not all ostreams know what to do with a CORBA::Long
      long const lid = id;
      ACE_OS::fprintf (out, " %s%s%ld%s", TOPOLOGY_ID_NAME, "=\"", lid, "\"");
    }

    ACE_TString::size_type const BUF_SIZE = 512;
    ACE_TString tmp(BUF_SIZE);
    for (size_t idx = 0; idx < attrs.size(); ++idx)
    {
      ACE_TString valuetmp (ACE_TEXT_CHAR_TO_TCHAR (attrs[idx].value.c_str()));
      ACEXML_escape_string(valuetmp, tmp);
      ACE_OS::fprintf (out, "%s%s%s%s%s"," ",
           attrs[idx].name.c_str (),
           "=\"", ACE_TEXT_ALWAYS_CHAR (tmp.c_str()), "\"");
    }
    ACE_OS::fprintf (out, ">\n");
    this->indent_ += "  ";
    return true;
  }

  void XML_Saver::end_object (CORBA::Long /* id */,
                              const ACE_CString& type)
  {
    ACE_ASSERT(this->output_ != 0);
    FILE * const out = this->output_;
    if (this->indent_.length() >= 2)
    {
      this->indent_ = this->indent_.substr(2);
    }
    ACE_OS::fprintf (out, "%s%s%s%s", indent_.c_str(), "</",
                     type.c_str(), ">\n");
  }
} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL
