// VFS.h

// Associate URI's to real path.
// Hash on URI.
// Maintain a table of files which are opened.
// Return a handle to the file, and provide I/O mechanisms for it.

#if !defined (VFS_H)
#define VFS_H

#include "JAWS/server/JXH_List.h"

#include "ace/Mem_Map.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"

class JAWS_VFS_Node
{
public:
  JAWS_VFS_Node (char *uri);
  ~JAWS_VFS_Node (void);

  char const * URI (void) const;
  int status (void) const;

  void open (void);
  // This if for TransmitFile.

  int map_read (void);
  // HTTP GET: File transfer without TransmitFile

  int map_write (u_long size);
  // HTTP PUT

  void *addr (void) const;
  // Address of memory mapped region

  ACE_HANDLE get_handle (void) const;
  // HTTP GET: TransmitFile
  
  unsigned long size (void) const;
  // File size

private:
  void uritopath (void);

  int open (int flags);

private:
  ACE_Mem_Map file_mapping_;

  char *uri_;
  char path_[MAXPATHLEN+1];

  //  void *file_;
  //  char buf_[BUFSIZ];

  struct stat stat_;
  ACE_HANDLE handle_;

  int status_;

  enum { NOT_OPEN, OPENED, MAPPED_READ, MAPPED_READWRITE };
  u_long map_state_;
};

class JAWS_VFS_Node_List : public JXH_List<JAWS_VFS_Node *>
{
public:
  JAWS_VFS_Node_List (int sz);
};

template <class LOCK>
class JAWS_VFS_Node_Bucket
{
public:
  JAWS_VFS_Node_Bucket (int size = 1);

  JAWS_VFS_Node * find (char *URI);
  
private:
  JAWS_VFS_Node_List bucket_;
  
  LOCK lock_;
};

class JAWS_VFS_Hash_Table
{
public:
  JAWS_VFS_Hash_Table ();

  ~JAWS_VFS_Hash_Table ();

  JAWS_VFS_Node * operator[] (char *URI);

private:
  int hashfunction (char *key) const;

private:
  JAWS_VFS_Node_Bucket<ACE_Thread_Mutex> *ht_;
};

class JAWS_VFS
{
public:
  int open (char *URI, JAWS_VFS_Node * &handle);
  int close (JAWS_VFS_Node * &handle);
  
private:
  JAWS_VFS_Hash_Table hash_;
};

typedef ACE_Singleton <JAWS_VFS, ACE_Thread_Mutex> VFS;

#endif /* VFS_H */

/// Local Variables:
/// mode: c++
/// End:
