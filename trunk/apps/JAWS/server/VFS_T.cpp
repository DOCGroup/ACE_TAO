#if !defined (VFS_T_C)
#define VFS_T_C

template <class LOCK>
JAWS_VFS_Node_Bucket<LOCK>::JAWS_VFS_Node_Bucket (int size) 
  : bucket_ (size) 
{
}

template <class LOCK>
JAWS_VFS_Node *
JAWS_VFS_Node_Bucket<LOCK>::find (char *URI)
{
  ACE_Guard<LOCK> g (this->lock_);
  int found = -1;
  
  if (! this->bucket_.IsEmpty ())
    for (int i = 0; i < this->bucket_.Size (); i++) {
      if (ACE_OS::strcmp (URI, this->bucket_[i]->URI ()) != 0) continue;
      found = i;
      break;
    }
  
  if (found == -1) 
    {
      JAWS_VFS_Node *new_node = new JAWS_VFS_Node (URI);
      if (new_node == 0)
	ACE_ERROR ((LM_ERROR, "%p.\n", "JAWS_VFS_Node"));
      this->bucket_.Insert (new_node);
      found = this->bucket_.Size () - 1;
    }
  
  return this->bucket_[found];
}

#endif /* VFS_T_C */

