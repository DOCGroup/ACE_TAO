/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/ACE.h"
#include "ace/CDR_Stream.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Message_Block.h"
#include "ace/Module.h"
#include "ace/SString.h"
#include "ace/Stream.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"

template <class TASK>
class Logrec_Module : public ACE_Module<ACE_SYNCH>
{
public:
  Logrec_Module (const ACE_TCHAR *name)
  {
    this->open (name,
                &task_, // Initialize writer-side task.
                0,      // Ignore reader-side task.
                0,
                ACE_Module<ACE_SYNCH>::M_DELETE_READER);
  }
private:
  TASK task_;
};
#define LOGREC_MODULE(NAME) \
  typedef Logrec_Module<NAME> NAME##_Module

class Logrec_Reader : public ACE_Task<ACE_SYNCH>
{
private:
  ACE_TString filename_; // Name of logfile.
  ACE_FILE_IO logfile_;  // File containing log records.

public:
  enum {MB_CLIENT = ACE_Message_Block::MB_USER,
        MB_TYPE, MB_PID, MB_TIME, MB_TEXT};

  Logrec_Reader (const ACE_TString &file): filename_ (file) {}

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *) {
  //FUZZ: enable check_for_lack_ACE_OS

    ACE_FILE_Addr name (filename_.c_str ());
    ACE_FILE_Connector connector;
    if (connector.connect (logfile_, name) == -1)
      return -1;
    return activate ();
  }

  virtual int svc () {
    const size_t FileReadSize = 8 * 1024;
    ACE_Message_Block mblk (FileReadSize);

    for (;; mblk.crunch ()) {
      // Read as much as will fit in the message block.
      ssize_t bytes_read = logfile_.recv (mblk.wr_ptr (),
                                          mblk.space ());
      if (bytes_read <= 0)
        break;
      mblk.wr_ptr (static_cast<size_t> (bytes_read));

      // We have a bunch of data from the log file. The data is
      // arranged like so:
      //    hostname\0
      //    CDR-encoded log record
      // So, first we scan for the end of the host name, then
      // initialize another ACE_Message_Block aligned for CDR
      // demarshaling and copy the remainder of the block into it. We
      // can't use duplicate() because we need to be sure the data
      // pointer is aligned properly for CDR demarshaling.  If at any
      // point, there's not enough data left in the message block to
      // extract what's needed, crunch the block to move all remaining
      // data to the beginning and read more from the file.
      for (;;) {
        size_t name_len = ACE_OS::strnlen
                             (mblk.rd_ptr (), mblk.length ());
        if (name_len == mblk.length ()) break;

        char *name_p = mblk.rd_ptr ();
        ACE_Message_Block *rec, *head, *temp;
        ACE_NEW_RETURN
          (head, ACE_Message_Block (name_len, MB_CLIENT), 0);
        head->copy (name_p, name_len);
        mblk.rd_ptr (name_len + 1);   // Skip nul also

        size_t need = mblk.length () + ACE_CDR::MAX_ALIGNMENT;
        ACE_NEW_RETURN (rec, ACE_Message_Block (need), 0);
        ACE_CDR::mb_align (rec);
        rec->copy (mblk.rd_ptr (), mblk.length ());

        // Now rec contains the remaining data we've read so far from
        // the file. Create an ACE_InputCDR to start demarshaling the
        // log record, header first to find the length, then the data.
        // Since the ACE_InputCDR constructor increases the reference count
        // on rec, we release it upon return to prevent leaks.
        // The cdr 'read' methods return 0 on failure, 1 on success.
        ACE_InputCDR cdr (rec); rec->release ();
        ACE_CDR::Boolean byte_order;
        if (!cdr.read_boolean (byte_order)) {
          head->release (); rec->release (); break;
        }
        cdr.reset_byte_order (byte_order);

        // Now read the length of the record. From there, we'll know
        // if rec contains the complete record or not.
        ACE_CDR::ULong length;
        if (!cdr.read_ulong (length)) {
          head->release (); mblk.rd_ptr (name_p); break;
        }
        if (length > cdr.length ()) {
          head->release (); mblk.rd_ptr (name_p); break;
        }

        // The complete record is in rec... grab all the fields into
        // separate, chained message blocks.
        ACE_NEW_RETURN (temp,
                        ACE_Message_Block (length, MB_TEXT),
                        0);
        ACE_NEW_RETURN
          (temp,
           ACE_Message_Block (2 * sizeof (ACE_CDR::Long),
                              MB_TIME, temp),
           0);
        ACE_NEW_RETURN
          (temp,
           ACE_Message_Block (sizeof (ACE_CDR::Long),
                              MB_PID, temp),
           0);
        ACE_NEW_RETURN
          (temp,
           ACE_Message_Block (sizeof (ACE_CDR::Long),
                              MB_TYPE, temp),
           0);
        head->cont (temp);

        // Extract the type
        ACE_CDR::Long *lp;
        lp = reinterpret_cast<ACE_CDR::Long*> (temp->wr_ptr ());
        cdr >> *lp;
        temp->wr_ptr (sizeof (ACE_CDR::Long));
        temp = temp->cont ();

        // Extract the pid
        lp = reinterpret_cast<ACE_CDR::Long*> (temp->wr_ptr ());
        cdr >> *lp;
        temp->wr_ptr (sizeof (ACE_CDR::Long));
        temp = temp->cont ();

        // Extract the timestamp (2 Longs)
        lp = reinterpret_cast<ACE_CDR::Long*> (temp->wr_ptr ());
        cdr >> *lp; ++lp; cdr >> *lp;
        temp->wr_ptr (2 * sizeof (ACE_CDR::Long));
        temp = temp->cont ();

        // Demarshal the length of the message text, then demarshal
        // the text into the block.
        ACE_CDR::ULong text_len;
        cdr >> text_len;
        cdr.read_char_array (temp->wr_ptr (), text_len);
        temp->wr_ptr (text_len);

        // Forward the whole lot to the next module.
        if (put_next (head) == -1) break;

        // Move the file-content block's read pointer up past whatever
        // was just processed. Although the mblk's rd_ptr has not been
        // moved, cdr's has.  Therefore, use its length() to determine
        // how much is left.
        mblk.rd_ptr (mblk.length () - cdr.length ());
      }
    }

    // Now that the file is done, send a block down the stream to tell
    // the other modules to stop.
    ACE_Message_Block *stop;
    ACE_NEW_RETURN
      (stop, ACE_Message_Block (0, ACE_Message_Block::MB_STOP),
       0);
    put_next (stop);
    return 0;
  }
};

class Logrec_Reader_Module : public ACE_Module<ACE_SYNCH>
{
public:
  Logrec_Reader_Module (const ACE_TString &filename)
    : task_ (filename)
  {
    this->open (ACE_TEXT ("Logrec Reader"),
                &task_, // Initialize writer-side.
                0,      // Ignore reader-side.
                0,
                ACE_Module<ACE_SYNCH>::M_DELETE_READER);
  }
private:
  Logrec_Reader task_;
};

class Logrec_Writer : public ACE_Task<ACE_SYNCH>
{
public:
  //FUZZ: disable check_for_lack_ACE_OS
  // Initialization hook method.
  virtual int open (void *) { return activate (); }
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int put (ACE_Message_Block *mblk, ACE_Time_Value *to)
  { return putq (mblk, to); }

  virtual int svc () {
    int stop = 0;
    for (ACE_Message_Block *mb = 0; !stop && getq (mb) != -1; ) {
      if (mb->msg_type () == ACE_Message_Block::MB_STOP)
        stop = 1;
      else
        ACE::write_n (ACE_STDOUT, mb);
      put_next (mb);
    }
    return 0;
  }
};

LOGREC_MODULE (Logrec_Writer);

class Logrec_Formatter : public ACE_Task<ACE_SYNCH>
{
public:
  typedef void (*FORMATTER[5])(ACE_Message_Block *);
private:
  static FORMATTER format_; // Array of format static methods.

public:
  virtual int put (ACE_Message_Block *mblk, ACE_Time_Value *) {
    if (mblk->msg_type () == Logrec_Reader::MB_CLIENT)
      for (ACE_Message_Block *temp = mblk;
           temp != 0;
           temp = temp->cont ()) {
        int mb_type =
          temp->msg_type () - ACE_Message_Block::MB_USER;
        (*format_[mb_type])(temp);
      }
    return put_next (mblk);
  }

  static void format_client (ACE_Message_Block *) {
    return;
  }

  static void format_type (ACE_Message_Block *mblk) {
    ACE_CDR::Long type = * (ACE_CDR::Long *)mblk->rd_ptr ();
    mblk->size (11); // Max size in ASCII of 32-bit word.
    mblk->reset ();
    mblk->wr_ptr ((size_t) ACE_OS::sprintf (mblk->wr_ptr (), "%d", type));
  }

  static void format_pid (ACE_Message_Block *mblk) {
    ACE_CDR::Long pid = * (ACE_CDR::Long *)mblk->rd_ptr ();
    mblk->size (11); // Max size in ASCII of 32-bit word.
    mblk->reset ();
    mblk->wr_ptr ((size_t) ACE_OS::sprintf (mblk->wr_ptr (), "%d", pid));
  }

  static void format_time (ACE_Message_Block *mblk) {
    ACE_CDR::Long secs = *(ACE_CDR::Long *) mblk->rd_ptr ();
    mblk->rd_ptr (sizeof (ACE_CDR::Long));
    ACE_CDR::Long usecs = *(ACE_CDR::Long *) mblk->rd_ptr ();
    ACE_TCHAR timestamp_t[26];
    char timestamp[26]; // Max size of ctime_r() string.
    time_t time_secs (secs);
    ACE_OS::ctime_r (&time_secs,
                     timestamp_t,
                     sizeof timestamp_t / sizeof (ACE_TCHAR));
    ACE_OS::strcpy (timestamp, ACE_TEXT_ALWAYS_CHAR (timestamp_t));
    mblk->size (26); // Max size of ctime_r() string.
    mblk->reset ();
    timestamp[19] = '\0'; // NUL-terminate after the time.
    timestamp[24] = '\0'; // NUL-terminate after the date.
    size_t fmt_len (ACE_OS::sprintf (mblk->wr_ptr (),
                                     "%s.%03d %s",
                                     timestamp + 4,
                                     usecs / 1000,
                                     timestamp + 20));
    mblk->wr_ptr (fmt_len);
  }

  static void format_data (ACE_Message_Block *) {
    return;
  }
};

Logrec_Formatter::FORMATTER Logrec_Formatter::format_ = {
  format_client, format_type, format_pid, format_time, format_data
};

LOGREC_MODULE (Logrec_Formatter);

class Logrec_Separator : public ACE_Task<ACE_SYNCH>
{
private:
  ACE_Lock_Adapter<ACE_SYNCH_MUTEX> lock_strategy_;

public:
  virtual int put (ACE_Message_Block *mblk,
                   ACE_Time_Value *) {
    if (mblk->msg_type () != ACE_Message_Block::MB_STOP) {
      ACE_Message_Block *separator;
      ACE_NEW_RETURN
        (separator,
         ACE_Message_Block (ACE_OS::strlen ("|") + 1,
                            ACE_Message_Block::MB_DATA,
                            0, 0, 0, &lock_strategy_),
         -1);
      separator->copy ("|");

      ACE_Message_Block *dup = 0;
      for (ACE_Message_Block *temp = mblk; temp != 0; ) {
        dup = separator->duplicate ();
        dup->cont (temp->cont ());
        temp->cont (dup);
        temp = dup->cont ();
      }
      ACE_Message_Block *nl;
      ACE_NEW_RETURN (nl, ACE_Message_Block (2), 0);
      nl->copy ("\n");
      dup->cont (nl);
      separator->release ();
    }
    return put_next (mblk);
  }
};

LOGREC_MODULE (Logrec_Separator);

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc != 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage: %s logfile\n", argv[0]),
                      1);
  ACE_TString logfile (argv[1]);
  ACE_Stream<ACE_SYNCH> stream;

  if (stream.push
      (new Logrec_Writer_Module (ACE_TEXT ("Writer"))) != -1
      && stream.push
      (new Logrec_Separator_Module (ACE_TEXT ("Separator"))) != -1
      && stream.push
      (new Logrec_Formatter_Module (ACE_TEXT ("Formatter"))) != -1
      && stream.push
      (new Logrec_Reader_Module (logfile)) != -1)
    return ACE_Thread_Manager::instance ()->wait () == 0 ? 0 : 1;
  return 1;
}

