// $Id$

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
ACE_INLINE
ACE_PI_Control_Block::ACE_Name_Node::~ACE_Name_Node (void)
{
}

ACE_INLINE void
ACE_PI_Control_Block::ACE_Malloc_Header::init_ptr
  (MALLOC_HEADER_PTR *ptr, ACE_Malloc_Header *init, void *base_addr)
{
  new ((void *) ptr) MALLOC_HEADER_PTR (base_addr, 0);
  *ptr = init;
}

ACE_INLINE void
ACE_PI_Control_Block::ACE_Name_Node::init_ptr
  (NAME_NODE_PTR *ptr, ACE_Name_Node *init, void *base_addr)
{
  new ((void *) ptr) NAME_NODE_PTR (base_addr, 0);
  *ptr = init;
}
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

