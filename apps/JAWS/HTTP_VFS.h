// HTTP_VFS.h

// Associate URI's to real path.
// Hash on URI.
// Maintain a table of files which are opened.
// Return a handle to the file, and provide I/O mechanisms for it.

#include "HTTP_Helpers.h"
#include "JXH_List.h"

#include "ace/Mem_Map.h"
#include "ace/Synch.h"

class HTTP_VFS_Node
{
public:
  HTTP_VFS_Node (char *uri);

  ~HTTP_VFS_Node (void);

  char const * URI (void) const { return this->uri_; }
  int const Status (void) const { return this->status_; }

  void * File (void) const
  { if (this->status_ != HTTP_Status_Code::STATUS_OK) return 0;
    else if (this->map_) return this->map_;
    return this->file_; }

  unsigned long Size (void) const { return this->stat_.st_size; }

private:
  void uritopath(void);

private:
  char *uri_;
  char path_[MAXPATHLEN+1];
  char buf_[BUFSIZ];

  struct stat stat_;
  ACE_HANDLE handle_;
  void *map_;
  void *file_;

  int status_;
};

class HTTP_VFS_Node_List : public JXH_List<HTTP_VFS_Node *>
{
public:
  HTTP_VFS_Node_List (int sz) : JXH_List<HTTP_VFS_Node *>(sz) {}
};

template <class LOCK>
class HTTP_VFS_Node_Bucket
{
public:
  HTTP_VFS_Node_Bucket (int size = 1) : bucket_(size) {}

  HTTP_VFS_Node * Find (char *URI)
  {
    ACE_Guard<LOCK> g(this->lock_);
    int found = -1;

    if (! this->bucket_.IsEmpty())
      for (int i = 0; i < this->bucket_.Size(); i++) {
        if (strcmp(URI, this->bucket_[i]->URI()) != 0) continue;
        found = i;
        break;
      }

    if (found == -1) {
      this->bucket_.Insert(new HTTP_VFS_Node(URI));
      found = this->bucket_.Size() - 1;
    }

    return this->bucket_[found];
  }

private:
  HTTP_VFS_Node_List bucket_;

  LOCK lock_;
};

class HTTP_VFS_Hash_Table
{
public:
  HTTP_VFS_Hash_Table ()
  { ht_ = new HTTP_VFS_Node_Bucket<ACE_Thread_Mutex>[256]; }

  ~HTTP_VFS_Hash_Table () { delete [] ht_; }

  HTTP_VFS_Node * operator[] (char *URI)
  {
    int index = this->hashfunction(URI);
    return ht_[index].Find(URI);
  }

private:
  int hashfunction (char *key) const;

private:
  HTTP_VFS_Node_Bucket<ACE_Thread_Mutex> *ht_;
};

class HTTP_VFS
{
public:
  static int open(char *URI, HTTP_VFS_Node * &handle);
  static int close(HTTP_VFS_Node * &handle);

private:
  static HTTP_VFS_Hash_Table hash_;
};

/// Local Variables:
/// mode: c++
/// End:
