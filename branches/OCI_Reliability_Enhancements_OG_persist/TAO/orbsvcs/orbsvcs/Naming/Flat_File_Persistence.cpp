// $Id$

//-----------------------------------------------------------------------------
// Flat File class implementations
//-----------------------------------------------------------------------------
#include "orbsvcs/Naming/Flat_File_Persistence.h"

#include "ace/Log_Msg.h"
#include "ace/Numeric_Limits.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NS_FlatFileStream::TAO_NS_FlatFileStream (const ACE_CString & file,
                                              const char * mode)
  : fl_ (0)
{
  ACE_TRACE("TAO_NS_FlatFileStream");
  file_ = file;
  mode_ = mode;
}

TAO_NS_FlatFileStream::~TAO_NS_FlatFileStream ()
{
  ACE_TRACE("~TAO_NS_FlatFileStream");
  if ( fl_ != 0 )
    this->close();
}

void
TAO_NS_FlatFileStream::remove ()
{
  ACE_TRACE("remove");
  ACE_OS::unlink(ACE_TEXT_CHAR_TO_TCHAR(file_.c_str()));
}

int
TAO_NS_FlatFileStream::exists ()
{
  ACE_TRACE("exists");
  // We could check the mode for this file, but for now just check exists
  return ! ACE_OS::access(file_.c_str(), F_OK);
}

int
TAO_NS_FlatFileStream::open()
{
  ACE_TRACE("open");
  // For now, three flags exist "r", "w",  and "c"
  int flags = 0;
  const char *fdmode = 0;
  if( ACE_OS::strchr(mode_.c_str(), 'r') )
    if( ACE_OS::strchr(mode_.c_str(), 'w') )
      flags = O_RDWR, fdmode = "r+";
    else
      flags = O_RDONLY, fdmode = "r";
  else
    flags = O_WRONLY, fdmode = "w";
  if( ACE_OS::strchr(mode_.c_str(), 'c') )
    flags |= O_CREAT;
#ifndef ACE_WIN32
  if( ACE_OS::flock_init (&filelock_, flags, ACE_TEXT_CHAR_TO_TCHAR(file_.c_str()), 0666) != 0 )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       errno, ACE_OS::strerror(errno)),
                      -1);
#else
  if( (filelock_.handle_= ACE_OS::open (ACE_TEXT_CHAR_TO_TCHAR(file_.c_str()), flags, 0)) == ACE_INVALID_HANDLE )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       ACE_ERRNO_GET, ACE_OS::strerror(ACE_ERRNO_GET)),
                      -1);
#endif
  this->fl_ = ACE_OS::fdopen(filelock_.handle_, ACE_TEXT_CHAR_TO_TCHAR(fdmode));
  if (this->fl_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot fdopen file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       ACE_ERRNO_GET, ACE_OS::strerror(ACE_ERRNO_GET)),
                      -1);
  return 0;
}

int
TAO_NS_FlatFileStream::close()
{
  ACE_TRACE("close");
  ACE_OS::fflush(fl_);
#ifndef ACE_WIN32
  ACE_OS::flock_destroy (&filelock_, 0);
#endif
  ACE_OS::fclose (fl_);  // even though flock_destroy closes the handle
                         // we still need to destroy the FILE*

  fl_ = 0;
  return 0;
}

int
TAO_NS_FlatFileStream::flock (int whence, int start, int len)
{
  ACE_TRACE("flock");
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
#else
  if( ACE_OS::strcmp(mode_.c_str(), "r") == 0 )
    ACE_OS::flock_rdlock(&filelock_, whence, start, len);
  else
    ACE_OS::flock_wrlock(&filelock_, whence, start, len);
#endif
  return 0;
}

int
TAO_NS_FlatFileStream::funlock (int whence, int start, int len)
{
  ACE_TRACE("funlock");
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
#else
  ACE_OS::flock_unlock(&filelock_, whence, start, len);
#endif
  return 0;
}

time_t
TAO_NS_FlatFileStream::last_changed(void)
{
  ACE_TRACE("TAO_NS_FlatFileStream::last_changed");
  ACE_stat st;
  ACE_OS::fstat(filelock_.handle_, &st);
  return st.st_mtime;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator <<(
                                const TAO_NS_Persistence_Header &header)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator <<");
  ACE_OS::rewind(this->fl_);
  ACE_OS::fprintf(this->fl_, "%d\n%d\n", header.size(), header.destroyed());
  ACE_OS::fflush(this->fl_);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator >>(
                      TAO_NS_Persistence_Header &header)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator >>");
  unsigned int size;
  int destroyed;

  ACE_OS::rewind(this->fl_);
  switch (fscanf(fl_, "%u\n", &size))
    {
    case 0:
      this->setstate (badbit);
      return *this;
    case EOF:
      this->setstate (eofbit);
      return *this;
    }
  header.size(size);

  switch (fscanf(fl_, "%d\n", &destroyed))
    {
    case 0:
      this->setstate (badbit);
      return *this;
    case EOF:
      this->setstate (eofbit);
      return *this;
    }
  header.destroyed(destroyed);

  return *this;

}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator <<(
                                const TAO_NS_Persistence_Record &record)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator <<");
  TAO_NS_Persistence_Record::Record_Type type = record.type();
  ACE_OS::fprintf(this->fl_, "%d\n", type);

  *this << record.id();
  *this << record.kind();
  *this << record.ref();

  ACE_OS::fflush(this->fl_);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator >>(TAO_NS_Persistence_Record &record)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator >>");

  int temp_type_in;
  switch (fscanf(fl_, "%d\n", &temp_type_in))
    {
    case 0:
      this->setstate (badbit);
      return *this;
    case EOF:
      this->setstate (eofbit);
      return *this;
    }
  TAO_NS_Persistence_Record::Record_Type type =
    (TAO_NS_Persistence_Record::Record_Type) temp_type_in;
  record.type (type);

  ACE_CString record_id;
  *this >> record_id;
  record.id (record_id);

  ACE_CString record_kind;
  *this >> record_kind;
  record.kind (record_kind);

  ACE_CString record_ref;
  *this >> record_ref;
  record.ref (record_ref);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator <<(
                                const TAO_NS_Persistence_Global &global)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator <<");
  ACE_OS::rewind(this->fl_);
  ACE_OS::fprintf(this->fl_, "%d\n", global.counter());
  ACE_OS::fflush(this->fl_);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator >>(
                      TAO_NS_Persistence_Global &global)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator >>");
  unsigned int counter = 0;

  ACE_OS::rewind(this->fl_);
  switch (fscanf(fl_, "%u\n", &counter))
    {
    case 0:
      this->setstate (badbit);
      break; // Still set the global.counter (to 0)
    case EOF:
      this->setstate (eofbit);
      break; // Still set the global.counter (to 0)
    }
  global.counter(counter);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator <<(
                      const ACE_CString& str)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator <<");
  ACE_OS::fprintf(this->fl_, ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT("\n%s\n"),
                  str.length(), str.c_str());

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator >>(
                      ACE_CString& str)
{
  ACE_TRACE("TAO_NS_FlatFileStream::operator >>");
  int bufSize = 0;
  ACE_CString::size_type const max_buf_len =
    ACE_Numeric_Limits<ACE_CString::size_type>::max ();
  switch (fscanf(fl_, "%d\n", &bufSize))
    {
    case 0:
      this->setstate (badbit);
      return *this;
    case EOF:
      this->setstate (eofbit);
      return *this;
    }

  if (bufSize < 0
      || static_cast<ACE_CString::size_type> (bufSize) >= max_buf_len)
    {
      this->setstate (badbit);
      return *this;
    }
  {
    ACE_Auto_Basic_Array_Ptr<char> str_array (new char[bufSize + 1]);
    str_array[0] = '\0';
    if (ACE_OS::fgets (ACE_TEXT_CHAR_TO_TCHAR (str_array.get ()),
                       bufSize + 1,
                       this->fl_) == 0
        && bufSize != 0)
      {
        this->setstate (badbit);
        return *this;
      }
    str = ACE_CString (str_array.get (), 0, false);
  }

  return *this;
}

TAO_Storable_Base *
TAO_NS_FlatFileFactory::create_stream (const ACE_CString & file,
                                       const ACE_TCHAR * mode)
{
  ACE_TRACE("TAO_NS_FlatFileFactory::create_stream");
  TAO_Storable_Base *stream = 0;

  ACE_NEW_RETURN (stream,
                  TAO_NS_FlatFileStream(file, ACE_TEXT_ALWAYS_CHAR (mode)),
                  0);
  return stream;
}

TAO_END_VERSIONED_NAMESPACE_DECL
