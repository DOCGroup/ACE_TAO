// HTTP_VFS.cpp

// Associate URI's to real path.
// Hash on URI.
// Maintain a table of files which are opened.
// Return a handle to the file, and provide I/O mechanisms for it.

#include "HTTP_VFS.h"

HTTP_VFS_Node::HTTP_VFS_Node (char *uri)
{
  // This is where all the important stuff happens.
  this->uri_ = ACE_OS::strdup(uri);
  uritopath();

  this->handle_ = ACE_OS::open(this->path_, O_RDONLY);
  if (this->handle_ == ACE_INVALID_HANDLE) {
    switch (errno) {
    case EACCES:
    case EAGAIN:
      this->status_ = HTTP_Status_Code::STATUS_UNAUTHORIZED;
      break;
    case EFAULT:
    case ENOTDIR:
      this->status_ = HTTP_Status_Code::STATUS_BAD_REQUEST;
      ACE_ERROR((LM_ERROR, "%p\n", "HTTP_VFS_Node::ctr"));
      break;
    case ENOENT:
    case ENOLINK:
      this->status_ = HTTP_Status_Code::STATUS_NOT_FOUND;
      break;
    default:
      this->status_ = HTTP_Status_Code::STATUS_INTERNAL_SERVER_ERROR;
      ACE_ERROR((LM_ERROR, "%p\n", "HTTP_VFS_Node::ctr"));
    }
    return;
  }

  ACE_OS::fstat(this->handle_, &(this->stat_));
  if (this->stat_.st_mode & S_IFDIR) {
    if ((this->stat_.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH))
        == (S_IXUSR|S_IXGRP|S_IXOTH))
      this->status_ = HTTP_Status_Code::STATUS_FORBIDDEN;
    else this->status_ = HTTP_Status_Code::STATUS_UNAUTHORIZED;
  }
  else if ((this->stat_.st_mode & (S_IRUSR|S_IRGRP|S_IROTH))
           != (S_IRUSR|S_IRGRP|S_IROTH))
    this->status_ = HTTP_Status_Code::STATUS_UNAUTHORIZED;
  else this->status_ = HTTP_Status_Code::STATUS_OK;
    
  if (this->status_ != HTTP_Status_Code::STATUS_OK) {
    ACE_OS::close(this->handle_);
    return;
  }

  this->map_ = ACE_OS::mmap(0, this->stat_.st_size, PROT_READ, MAP_SHARED,
                            this->handle_, 0);
  if (this->map_ == MAP_FAILED) {
    this->map_ = 0;
    this->file_ = new char[this->stat_.st_size];
    int count = 0;
    char *p = (char *)this->file_;
    while (count < this->stat_.st_size) {
      int n = ACE_OS::read(this->handle_, p, this->stat_.st_size-count);
      if (n == -1) break;
      p += n;
      count += n;
    }
    if (count == 0) {
      delete [] this->file_;
      this->file_ = 0;
    }
    ACE_OS::close(this->handle_);
  }
  else this->file_ = 0;
}

HTTP_VFS_Node::~HTTP_VFS_Node (void)
{
  free(this->uri_);
  if (this->status_ == HTTP_Status_Code::STATUS_OK) {
    if (this->map_) {
      ACE_OS::munmap(this->map_, this->stat_.st_size);
      ACE_OS::close(this->handle_);
    }
    else if (this->file_) delete [] this->file_;
  }
}

void
HTTP_VFS_Node::uritopath(void)
{
  char const *file_name = this->uri_;
 
  if (*file_name == '/') file_name++;
  if (*file_name == '~') {
    char *ptr = this->buf_;
 
    while (*++file_name && *file_name != '/')
      *ptr++ = *file_name;
 
    *ptr = '\0';
 
    if (ptr == this->buf_)
      ACE_OS::strcpy (this->buf_, ACE_OS::getenv ("HOME"));
    else {
      char pw_buf[BUFSIZ];
      struct passwd pw_struct;
      if (::getpwnam_r(this->buf_, &pw_struct, pw_buf, sizeof(pw_buf)) == 0)
        return;
      ACE_OS::strcpy (this->buf_, pw_struct.pw_dir);
    }
 
    ACE_OS::strcat (this->buf_, "/.www-docs");
    ACE_OS::strcat (this->buf_, file_name);
 
    ACE_OS::strcpy (this->path_, this->buf_);
  }
}


int
HTTP_VFS_Hash_Table::hashfunction (char *key) const
{
  unsigned long sum = 0;
  int j = 0;
    
  for (int i = ACE_OS::strlen(key)-1; i >= 0; i-=3) {
    sum *= 2;
    sum += key[i]%2;
    if (++j == 32) break;
  }
  return sum % 256U;
}

int
HTTP_VFS::open(char *URI, HTTP_VFS_Node * &handle)
{
  handle = HTTP_VFS::hash_[URI];

  return handle->Status();
}

int
HTTP_VFS::close(HTTP_VFS_Node * &handle)
{
  // In the future, do something intelligent here.
  return 0;
}

HTTP_VFS_Hash_Table HTTP_VFS::hash_;
