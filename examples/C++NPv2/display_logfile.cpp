/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/CDR_Stream.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Message_Block.h"
#include "ace/Module.h"
#include "ace/OS_String.h"
#include "ace/SString.h"
#include "ace/Stream.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"

template <class TASK>
class Logrec_Module : public ACE_Module<ACE_MT_SYNCH>
{
public:
  Logrec_Module (ACE_TCHAR *name)
    : ACE_Module (name, 
                  &task_, // Initialize writer-side task.
                  0,      // Ignore reader-side task.
                  0, ACE_Module::M_DELETE_READER) {}
private:
  TASK task_;
};
#define LOGREC_MODULE(NAME) \
  typedef Logrec_Module<NAME> NAME##_Module (#NAME)

class Logrec_Reader : public ACE_Task<ACE_MT_SYNCH>
{
private:
  ACE_TString filename_; // Name of logfile. 
  ACE_FILE_IO logfile_;  // File containing log records.

public:
  enum {MB_CLIENT = ACE_Message_Block::MB_USER,
        MB_TYPE, MB_PID, MB_TIME, MB_TEXT};

  Logrec_Reader (const ACE_TString &file): filename_ (file) {}

  virtual int open (void *) {
    ACE_FILE_Addr name (filename_.c_str ());
    ACE_FILE_Connector connector;
    if (connector.connect (logfile_, name) == -1)
      return -1;
    return activate ();
  }

  virtual int svc () {
    // Steve, can we please use a macro, const, or enum here?
    ACE_Message_Block mblk (8*1024);
 
    for (;; mblk.crunch ()) {
      // Read as much as will fit in the message block.
      ssize_t bytes_read = logfile_.recv (mblk.wr_ptr (),
                                          mblk.space ());
      if (bytes_read <= 0)
        break;

      mblk.wr_ptr (ACE_static_cast (size_t, bytes_read));

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
        size_t name_len =
          ACE_OS_String::strnlen (mblk.rd_ptr (), mblk.length ());
        if (name_len == mblk.length ()) break;

        ACE_Message_Block *rec, *head, *temp;

        ACE_NEW_RETURN (head, ACE_Message_Block (name_len, MB_CLIENT), 0);
        head->copy (mblk.rd_ptr (), name_len);
        mblk.rd_ptr (name_len + 1);   // Skip nul also

        size_t need = mblk.length () + ACE_CDR::MAX_ALIGNMENT;
        ACE_NEW_RETURN (rec, ACE_Message_Block (need), 0);
        ACE_CDR::mb_align (rec);
        rec->copy (mblk.rd_ptr (), mblk.length ());

        // Now rec contains the remaining data we've read so far from
        // the file. Create an ACE_InputCDR to start demarshaling the
        // log record, header first to find the length, then the data.
        // The cdr 'read' methods return 0 on failure, 1 on success.
        ACE_InputCDR cdr (rec);
        ACE_CDR::Boolean byte_order;
        if (!cdr.read_boolean (byte_order)) {
          head->release (); rec->release (); break;
        }
        cdr.reset_byte_order (byte_order);

        // Now read the length of the record. From there, we'll know
        // if rec contains the complete record or not.
        ACE_CDR::ULong length;
        if (!cdr.read_ulong (length)) {
          head->release (); rec->release (); break;
        }

        if (length > rec->length ()) {
          head->release (); rec->release (); break;
        }

        // The complete record is in rec... grab all the fields into
        // separate, chained message blocks.
        ACE_NEW_RETURN (temp, \
          ACE_Message_Block (length, MB_TEXT), 0);
        ACE_NEW_RETURN (temp, \
          ACE_Message_Block (2 * sizeof (ACE_CDR::Long), MB_TIME, temp), 0);
        ACE_NEW_RETURN (temp, \
          ACE_Message_Block (sizeof (ACE_CDR::Long), MB_PID, temp), 0);
        ACE_NEW_RETURN (temp, \
          ACE_Message_Block (sizeof (ACE_CDR::Long), MB_TYPE, temp), 0);

        // Demarshal the type
        head->cont (temp);
        ACE_CDR::Long *lp = ACE_reinterpret_cast (ACE_CDR::Long*, temp->wr_ptr ());
        cdr >> *lp;
        temp->wr_ptr (sizeof (ACE_CDR::Long));
        temp = temp->cont ();

        // Demarshal the pid
        ACE_CDR::Long *lp = ACE_reinterpret_cast (ACE_CDR::Long*, temp->wr_ptr ());
        cdr >> *lp;
        temp->wr_ptr (sizeof (ACE_CDR::Long));
        temp = temp->cont ();

        // Demarshal the time (2 Longs)
        ACE_CDR::Long *lp = ACE_reinterpret_cast (ACE_CDR::Long*, temp->wr_ptr ());
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
        // was just processed. This works because ACE_InputCDR has
        // been moving rec's rd_ptr to reflect demarshaled content, so
        // it's length now reflects what is left.
        mblk.rd_ptr (mblk.length () - rec->length ());
      }
   }

  return 0;
  }
};

class Logrec_Reader_Module : public ACE_Module<ACE_MT_SYNCH>
{
public:
  Logrec_Reader_Module (const ACE_TString &filename)
    : task_ (filename),
      ACE_Module (ACE_TEXT ("Logrec Reader"),
                  &task_, // Initialize writer-side.
                  0,      // Ignore reader-side.
                  0, ACE_Module::M_DELETE_READER) {}
private:
  Logrec_Reader task_;
};

class Logrec_Writer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  // Initialization hook method.
  virtual int open (void *) { return activate (); }

  virtual int put (ACE_Message_Block *mblk, ACE_Time_Value &to) 
  { return putq (mblk, to); }

  virtual int svc () {
    for (ACE_Message_Block *mb; getq (mb) != -1; mb->release ())
      ACE::write_n (ACE_STDOUT, mb);
  }
};

LOGREC_MODULE (Logrec_Writer);

class Logrec_Formatter : public ACE_Task<ACE_MT_SYNCH>
{
private:
  typedef void (*FORMATTER[5])(ACE_Message_Block *,
                               ACE_CDR::Boolean);
  static FORMATTER format_; // Array of format static methods.

public:
  virtual int put (ACE_Message_Block *mblk, ACE_Time_Value &) {
    // Steve, if we convert stuff into host byte order in the
    // Logrec_Reader class can we simplify the code here?!
    if (mblk->type () != Logrec_Reader::MB_CLIENT) return -1;
    ACE_InputCDR cdr (mblk); ACE_CDR::Boolean byte_order;
    cdr >> ACE_InputCDR::to_boolean (byte_order);

    for (ACE_Message_Block *temp = mblk;
         temp != 0;
         temp = temp->cont ()) {
      int mb_type = temp->msg_type () - ACE_Message_Block::MB_USER;
      (*format_[mb_type])(temp, byte_order);
    }
    return put_next (mblk);
  }

  // Steve, is it valid to define static methods inline?!
  static void format_client (ACE_Message_Block *mblk,
                             ACE_CDR::Boolean byte_order) {
    ACE_InputCDR cdr (mblk); cdr.reset_byte_order (byte_order);
    ACE_CDR::ULong length; cdr >> length;
    mblk->size (11); // Max size in ASCII of 32-bit word.
    mblk->reset ();
    mblk->wr_ptr (size_t (sprintf (mblk->wr_ptr (), "%ld", length)));
  }

  static void format_type (ACE_Message_Block *mblk,
                           ACE_CDR::Boolean byte_order) {
    ACE_InputCDR cdr (mblk); cdr.reset_byte_order (byte_order);
    ACE_CDR::ULong type; cdr >> type;
    mblk->size (11); // Max size in ASCII of 32-bit word.
    mblk->reset ();
    mblk->wr_ptr (sprintf (mblk->wr_ptr (), "%ld", type));
  }

  static void format_pid (ACE_Message_Block *mblk,
                          ACE_CDR::Boolean byte_order) {
    ACE_InputCDR cdr (mblk); cdr.reset_byte_order (byte_order);
    ACE_CDR::ULong pid; cdr >> pid;
    mblk->size (11); // Max size in ASCII of 32-bit word.
    mblk->reset ();
    mblk->wr_ptr (sprintf (mblk->wr_ptr (), "%ld", pid));
  }

  static void format_time (ACE_Message_Block *mblk,
                           ACE_CDR::Boolean byte_order) {
    ACE_InputCDR cdr (mblk); cdr.reset_byte_order (byte_order);
    ACE_CDR::ULong secs; cdr >> secs;
    ACE_CDR::ULong usecs; cdr >> usecs;
    char timestamp[26]; // Max size of ctime_r() string.
    ACE_OS::ctime_r (secs, timestamp, sizeof timestamp);
    mblk->size (26); // Max size of ctime_r() string.
    mblk->reset ();
    timestamp[19] = '\0'; // NUL-terminate after the time.
    timestamp[24] = '\0'; // NUL-terminate after the date.
    size_t fmt_len (sprintf (mblk->wr_ptr (),
                             "%s.%03ld %s",
                             timestamp + 4,
                             usecs / 1000,
                             timestamp + 20));
    mblk->wr_ptr (fmt_len);
  }

  static void format_data (ACE_Message_Block *mblk) {
    mblk->rd_ptr (4); 
  }

Logrec_Formatter::FORMATTER Logrec_Formatter::format_[] = {
  format_client, format_type, format_pid, format_time, format_data
};

LOGREC_MODULE (Logrec_Formatter);

class Logrec_Separator : public ACE_Task<ACE_MT_SYNCH>
{
private:
  ACE_Lock_Adapter<ACE_Thread_Mutex> lock_strategy_;

public:
  virtual int put (ACE_Message_Block *mblk,
                   ACE_Time_Value &) {
    ACE_Message_Block *separator;
    ACE_NEW_RETURN (separator,
                    ACE_Message_Block (ACE_OS::strlen ("|") + 1,
                                       ACE_Message_Block::MB_DATA,
                                       0, 0, 0, &lock_strategy_),
                    -1);
    separator->copy ("|");
   
    for (ACE_Message_Block *temp = mblk; temp != 0; ) {
      ACE_Message_Block *dup = separator->duplicate ();
      dup->cont (temp->cont ());
      temp->cont (dup);
      temp = dup->cont ();
    }

    int result = put_next (mblk);
    separator->release ();
    return result;
  }
};

LOGREC_MODULE (Logrec_Separator);

int main (int argc, char *argv[])
{
  if (argc != 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage: %s logfile\n", argv[0]),
                      1);
  ACE_TString logfile (ACE_TEXT_CHAR_TO_TCHAR (argv[1]));
  ACE_Stream<ACE_MT_SYNCH> stream;

  if (stream.push (new Logrec_Writer_Module) != -1
      && stream.push (new Logrec_Separator_Module) != -1
      && stream.push (new Logrec_Formatter_Module) != -1
      && stream.push (new Logrec_Reader_Module (logfile) != -1)
    return ACE_Thread_Manager::instance ()->wait () == 0 ? 0 : 1;
  return 1;
}
