// $Id$

//-----------------------------------------------------------------------------
// Flat File class implementations
//-----------------------------------------------------------------------------
#include "Flat_File_Persistence.h"

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
  ACE_OS::unlink(file_.c_str());
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
  if( strchr(mode_.c_str(), 'r') )
    if( strchr(mode_.c_str(), 'w') )
      flags = O_RDWR, fdmode = "r+";
    else
      flags = O_RDONLY, fdmode = "r";
  else
    flags = O_WRONLY, fdmode = "w";
  if( strchr(mode_.c_str(), 'c') )
    flags |= O_CREAT;
#ifndef ACE_WIN32
  if( ACE_OS::flock_init (&filelock_, flags, file_.c_str(), 0666) != 0 )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       errno, ACE_OS::strerror(errno)),
                      -1);
#else
  if( (filelock_.handle_= ACE_OS::open (file_.c_str(), flags, 0)) == ACE_INVALID_HANDLE )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       errno, ACE_OS::strerror(errno)),
                      -1);
#endif  
  this->fl_ = ACE_OS::fdopen(filelock_.handle_, fdmode);
  if (this->fl_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot fdopen file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       errno, ACE_OS::strerror(errno)),
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
#else
  ACE_OS::fclose (fl_);
#endif  
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
  ACE_TRACE("last_changed");
  ACE_stat st;
  ACE_OS::fstat(filelock_.handle_, &st);
  return st.st_mtime;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator <<(
                                const TAO_NS_Persistence_Header &header)
{
  ACE_TRACE("<<header");
  ACE_OS::rewind(this->fl_);
  ACE_OS::fprintf(this->fl_, "%d\n%d\n", header.size(), header.destroyed());
  ACE_OS::fflush(this->fl_);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator >>(
                      TAO_NS_Persistence_Header &header)
{
  ACE_TRACE(">>header");
  unsigned int size;
  int destroyed;

  ACE_OS::rewind(this->fl_);
  fscanf(fl_, "%d\n", &size);
  header.size(size);

  fscanf(fl_, "%d\n", &destroyed);
  header.destroyed(destroyed);

  return *this;

}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator <<(
                                const TAO_NS_Persistence_Record &record)
{
  ACE_TRACE("<<record");
  TAO_NS_Persistence_Record::Record_Type type = record.type();
  ACE_OS::fprintf(this->fl_, "%d\n", type);

  ACE_CString id = record.id();
  ACE_OS::fprintf(this->fl_, "%d\n%s\n", id.length(), id.c_str());

  ACE_CString kind = record.kind();
  ACE_OS::fprintf(this->fl_, "%d\n%s\n", kind.length(), kind.c_str());

  ACE_CString ref = record.ref();
  ACE_OS::fprintf(this->fl_, "%d\n%s\n", ref.length(), ref.c_str());

  ACE_OS::fflush(this->fl_);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator >>(
                       TAO_NS_Persistence_Record &record)
{
  ACE_TRACE(">>record");
  TAO_NS_Persistence_Record::Record_Type type;
  int temp_type_in;
  fscanf(fl_, "%d\n", &temp_type_in);
  type = (TAO_NS_Persistence_Record::Record_Type) temp_type_in;
  record.type(type);

  int bufSize = 0;

  //id
  fscanf(fl_, "%d\n", &bufSize);
  char *id = new char[bufSize+1];
  //char *id;
  //ACE_NEW_RETURN (id, char[bufSize+1], 1);
  ACE_OS::fgets(id, bufSize+1, fl_);
  ACE_CString newId(id);
  record.id(newId);
  delete [] id;

  //kind
  fscanf(fl_, "%d\n", &bufSize);
  char *kind = new char[bufSize+1];
  //char *kind;
  //ACE_NEW (kind, char[bufSize+1]);
  ACE_OS::fgets(kind, bufSize+1, fl_);
  kind[bufSize] = '\0';
  ACE_CString newKind(kind);
  record.kind(newKind);
  delete [] kind;

   //ref
  fscanf(fl_, "%d\n", &bufSize);
  char *ref = new char[bufSize+1];
  //char *ref;
  //ACE_NEW(ref, char[bufSize+1]);
  ACE_OS::fgets(ref, bufSize+1, fl_);
  ACE_CString newRef(ref);
  record.ref(newRef);
  delete [] ref;

  return *this;

}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator <<(
                                const TAO_NS_Persistence_Global &global)
{
  ACE_TRACE("<<global");
  ACE_OS::rewind(this->fl_);
  ACE_OS::fprintf(this->fl_, "%d\n", global.counter());
  ACE_OS::fflush(this->fl_);

  return *this;
}

TAO_Storable_Base &
TAO_NS_FlatFileStream::operator >>(
                      TAO_NS_Persistence_Global &global)
{
  ACE_TRACE(">>global");
  unsigned int counter = 0;

  ACE_OS::rewind(this->fl_);
  fscanf(fl_, "%d\n", &counter);
  global.counter(counter);

  return *this;

}


TAO_Storable_Base *TAO_NS_FlatFileFactory::create_stream(
                                                       const ACE_CString & file, 
                                                       const char * mode)
{
  ACE_TRACE("create_stream");
  TAO_Storable_Base *stream = 0;

  ACE_NEW_RETURN (stream,
                  TAO_NS_FlatFileStream(file, mode),
                  0);
  return stream;
}
