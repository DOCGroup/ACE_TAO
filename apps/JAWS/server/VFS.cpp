// $Id$

// Associate URI's to real path.
// Hash on URI.
// Maintain a table of files which are opened.
// Return a handle to the file, and provide I/O mechanisms for it.

#include "JAWS/server/VFS.h"
#include "JAWS/server/HTTP_Helpers.h"

#if defined (ACE_WIN32)
static const int READ_FLAGS
  = FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_OVERLAPPED | O_RDONLY;
static const int WRITE_FLAGS
  = FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_OVERLAPPED | O_RDWR | O_CREAT;
#else
static const int READ_FLAGS = O_RDONLY;
static const int WRITE_FLAGS = O_RDWR | O_CREAT;
#endif /* ACE_WIN32 */

JAWS_VFS_Node::JAWS_VFS_Node (char *uri)
  : uri_ (ACE_OS::strdup (uri)),
    map_state_ (NOT_OPEN)
{
  this->uritopath ();
}

JAWS_VFS_Node::~JAWS_VFS_Node (void)
{
  ACE_OS::free (this->uri_);
  if (handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::close (handle_);
      handle_ = ACE_INVALID_HANDLE;
    }
}

void
JAWS_VFS_Node::open (void)
{
  if (map_state_ == NOT_OPEN)
    this->open (READ_FLAGS);
}

int
JAWS_VFS_Node::open (int flags)
{
  if (map_state_ != NOT_OPEN)
    {
      ACE_OS::close (handle_);
      handle_ = ACE_INVALID_HANDLE;
    }

  this->handle_ = ACE_OS::open (this->path_, flags,
                                S_IRUSR|S_IRGRP|S_IROTH
                                |S_IWUSR|S_IWGRP|S_IWOTH);
  if (this->handle_ == ACE_INVALID_HANDLE) 
    {
      switch (errno) 
	{
	case EACCES:
	case EAGAIN:
	  this->status_ = HTTP_Status_Code::STATUS_UNAUTHORIZED;
	  ACE_ERROR ((LM_ERROR,
                      "%p unauthorized error.\n",
                      "JAWS_VFS_Node::open"));
	  break;
	case EFAULT:
	case ENOTDIR:
	  this->status_ = HTTP_Status_Code::STATUS_BAD_REQUEST;
	  ACE_ERROR ((LM_ERROR,
                      "%p bad request.\n",
                      "JAWS_VFS_Node::open"));
	  break;
	case ENOENT:
#if !defined (ACE_WIN32)
	case ENOLINK:
#endif /* ACE_WIN32 */
	  this->status_ = HTTP_Status_Code::STATUS_NOT_FOUND;
	  ACE_ERROR ((LM_ERROR,
                      "%p file not found error.\n",
                      "JAWS_VFS_Node::open"));
	  break;
	default:
	  this->status_ = HTTP_Status_Code::STATUS_INTERNAL_SERVER_ERROR;
	  ACE_ERROR ((LM_ERROR,
                      "%p internal server error.\n",
                      "JAWS_VFS_Node::open"));
	}
    }
  else if (ACE_OS::stat (this->path_, &this->stat_) == -1)
    {
      this->status_ = HTTP_Status_Code::STATUS_INTERNAL_SERVER_ERROR;
      ACE_ERROR ((LM_ERROR,
                  "%p stat failed.\n",
                  "JAWS_VFS_Node::open"));
    }
  else 
    {
      if (this->stat_.st_mode & S_IFDIR) 
	{
	  if ((this->stat_.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH))
              == (S_IXUSR|S_IXGRP|S_IXOTH))
	    {
	      this->status_ = HTTP_Status_Code::STATUS_FORBIDDEN;
	      ACE_ERROR ((LM_ERROR,
                          "%p file is forbidden.\n",
                          "JAWS_VFS_Node::open"));
	    }
	  else
	    {
	      this->status_ = HTTP_Status_Code::STATUS_UNAUTHORIZED;
	      ACE_ERROR ((LM_ERROR,
                          "%p file access is unauthorized.\n",
                          "JAWS_VFS_Node::open"));
	    }
	}
      else if ((this->stat_.st_mode & (S_IRUSR|S_IRGRP|S_IROTH))
               != (S_IRUSR|S_IRGRP|S_IROTH))
	{
	  this->status_ = HTTP_Status_Code::STATUS_UNAUTHORIZED;
	  ACE_ERROR ((LM_ERROR,
                      "%p file access is unauthorized.\n",
                      "JAWS_VFS_Node::open"));
	}
      else 
	this->status_ = HTTP_Status_Code::STATUS_OK;
    }
  
  if (this->status_ != HTTP_Status_Code::STATUS_OK) 
    {
      if (handle_ != ACE_INVALID_HANDLE)
	{
	  ACE_OS::close (handle_);
	  handle_ = ACE_INVALID_HANDLE;
	}
      return -1;
    }
  
  map_state_ = OPENED;
  return 0;
}

int
JAWS_VFS_Node::map_read (void)
{
  if (map_state_ != MAPPED_READ && 
      map_state_ != MAPPED_READWRITE)
    {
      if (this->open (READ_FLAGS) == -1)
	return -1;

      if (file_mapping_.map (handle_, -1, PROT_READ, MAP_PRIVATE) == 0)
	{
	  map_state_ = MAPPED_READ;
	  return 0;
	}
      else
	{
	  this->status_ = HTTP_Status_Code::STATUS_INTERNAL_SERVER_ERROR;
	  ACE_ERROR ((LM_ERROR,
                      "%p memory mapping for read.\n",
                      "JAWS_VFS_Node::map_write"));
	  return -1;
	}
    }
  else
    return 0;
}

int
JAWS_VFS_Node::map_write (u_long size)
{
  if (map_state_ != MAPPED_READWRITE)
    {
      if (this->open (WRITE_FLAGS) == -1)
	return -1;

      if (file_mapping_.map (handle_, 
			     size, 
			     PROT_RDWR, 
			     MAP_SHARED) == 0)
	{
	  map_state_ = MAPPED_READWRITE;
	  return 0;
	}
      else
	{
	  this->status_ = HTTP_Status_Code::STATUS_INTERNAL_SERVER_ERROR;
	  ACE_ERROR ((LM_ERROR,
                      "%p memory mapping for read.\n",
                      "JAWS_VFS_Node::map_write"));
	  return -1;
	}
    }
  else
    return 0;
}

void
JAWS_VFS_Node::uritopath (void)
{
  char const *file_name = this->uri_;
 
  char buf[MAXPATHLEN+1];
  buf[0] = '\0';

  if (*file_name == '/') file_name++;
  if (*file_name == '~') {
    char *ptr = buf;
 
    while (*++file_name && *file_name != '/')
      *ptr++ = *file_name;
 
    *ptr = '\0';
 
    if (ptr == buf)
      ACE_OS::strcpy (buf, ACE_OS::getenv ("HOME"));
    else {
#if !defined (ACE_WIN32)
      char pw_buf[BUFSIZ];
      struct passwd pw_struct;
      if (ACE_OS::getpwnam_r (buf, &pw_struct, pw_buf, sizeof (pw_buf)) == 0)
        return;
      ACE_OS::strcpy (buf, pw_struct.pw_dir);
#endif /* ACE_WIN32 */
    }
 
    ACE_OS::strcat (buf, "/.www-docs/");
    ACE_OS::strcat (buf, file_name);
  }
  else {
    ACE_OS::strcat (buf, "./");
    ACE_OS::strcat (buf, file_name);
  }

  ACE_OS::strcpy (this->path_, buf);
}

ACE_HANDLE
JAWS_VFS_Node::get_handle (void) const 
{
  return this->handle_; 
}

void *
JAWS_VFS_Node::addr (void) const
{
  return file_mapping_.addr ();
}

char const * 
JAWS_VFS_Node::URI (void) const
{ 
  return this->uri_; 
}

int
JAWS_VFS_Node::status (void) const 
{ 
  return this->status_; 
}

unsigned long 
JAWS_VFS_Node::size (void) const 
{ 
  return this->stat_.st_size; 
}

JAWS_VFS_Node_List::JAWS_VFS_Node_List (int sz) 
  : JXH_List<JAWS_VFS_Node *> (sz) 
{
}

JAWS_VFS_Hash_Table::JAWS_VFS_Hash_Table ()
{ 
  ACE_NEW (ht_, JAWS_VFS_Node_Bucket<ACE_SYNCH_MUTEX>[256]);
}

JAWS_VFS_Hash_Table::~JAWS_VFS_Hash_Table () 
{ 
  delete [] ht_; 
}

JAWS_VFS_Node *
JAWS_VFS_Hash_Table::operator[] (char *URI)
{
  int index = this->hashfunction (URI);
  return ht_[index].find (URI);
}

int
JAWS_VFS_Hash_Table::hashfunction (char *key) const
{
  unsigned long sum = 0;
  int j = 0;
    
  for (int i = ACE_OS::strlen (key)-1; i >= 0; i-=3) {
    sum *= 2;
    sum += key[i]%2;
    if (++j == 32) break;
  }
  return sum % 256U;
}

int
JAWS_VFS::open (char *URI, JAWS_VFS_Node * &handle)
{
  handle = this->hash_[URI];
  
  return handle->status ();
}

int
JAWS_VFS::close (JAWS_VFS_Node * &handle)
{
  // In the future, do something intelligent here.
  ACE_UNUSED_ARG (handle);
  return 0;
}

#if 0
inline void
JAWS_VFS_Node::map_read (void)
{
  this->map_ = ACE_OS::mmap (0, this->stat_.st_size, PROT_READ, MAP_SHARED,
			     this->handle_, 0);
  if (this->map_ == MAP_FAILED) 
    {
      this->map_ = 0;
      this->file_ = new char[this->stat_.st_size];
      int count = 0;
      char *p = (char *)this->file_;

      while (count < this->stat_.st_size) 
	{
	  int n = ACE_OS::read (this->handle_, p, this->stat_.st_size-count);
	  if (n == -1) break;
	  p += n;
	  count += n;
	}

      if (count == 0) 
	{
	  delete [] this->file_;
	  this->file_ = 0;
	}

      ACE_OS::close (this->handle_);
    }
  else 
    this->file_ = 0;

  map_state_ = MAPPED_READ;
}
#endif /* 0 */

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class JXH_List<JAWS_VFS_Node *>;
template class JAWS_VFS_Node_Bucket<ACE_SYNCH_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

