// $Id$

#if !defined (VFS_T_H)
#define VFS_T_H

template <class LOCK>
class JAWS_VFS_Node_Bucket
{
public:
  JAWS_VFS_Node_Bucket (int size = 1);

  JAWS_VFS_Node * find (const char *URI);
  
private:
  JAWS_VFS_Node_List bucket_;
  
  LOCK lock_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "JAWS/server/VFS_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("VFS_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* VFS_T_H */
