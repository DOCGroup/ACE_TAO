// $Id$

// Note: this file is here only until support for explicit template
// instantiations is removed from ACE, after ACE 5.5 is released.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \
    defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# include "ace/Array_Base.h"
# include "ace/Condition_T.h"
# include "ace/Functor_T.h"
# include "ace/Hash_Map_Manager.h"
# include "ace/INET_Addr.h"
# include "ace/Refcounted_Auto_Ptr.h"
# include "ace/Thread_Mutex.h"
# include "ace/Unbounded_Queue.h"
# include "ace/Vector_T.h"

# include "Acknowledge.h"
# include "Protocol.h"
# include "Retransmit.h"

#endif

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<unsigned char>;
template class ACE_Array_Base<ACE_RMCast::u64>;
template class ACE_Array_Base<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >;
template class ACE_Array_Base<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >;
template class ACE_Condition<ACE_Thread_Mutex>;
template class ACE_Equal_To<ACE_INET_Addr>;
template class ACE_Hash_Map_Const_Iterator_Base_Ex<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>, ACE_Hash<unsigned short>, ACE_Equal_To<unsigned short>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Const_Iterator_Base_Ex<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Const_Iterator_Base_Ex<ACE_INET_Addr, ACE_RMCast::u64, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, ACE_RMCast::Acknowledge::Queue, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr>;
template class ACE_Hash_Map_Entry<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr>;
template class ACE_Hash_Map_Entry<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex> >;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_RMCast::u64>;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Data, ACE_Thread_Mutex> >;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_RMCast::Acknowledge::Queue>;
template class ACE_Hash_Map_Manager<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Data, ACE_Thread_Mutex>, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_RMCast::Acknowledge::Queue, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_RMCast::u64, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>, ACE_Hash<unsigned short>, ACE_Equal_To<unsigned short>, ACE_Null_Mutex>;
template class ACE_Node<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >;
template class ACE_Node<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >;
template class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Data, ACE_Thread_Mutex>;
template class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex>;
template class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex>;
template class ACE_Refcounted_Auto_Ptr<ACE_RMCast::NAK, ACE_Thread_Mutex>;
template class ACE_Refcounted_Auto_Ptr<ACE_RMCast::NRTM, ACE_Thread_Mutex>;
template class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>;
template class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >;
template class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >;
template class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >;
template class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >;
template class ACE_Vector<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >;
template class ACE_Vector<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >;
template class ACE_Vector<unsigned char>;
template class ACE_Vector<ACE_RMCast::u64>;
template class ACE_Vector_Iterator<unsigned char>;
template class ACE_Vector_Iterator<ACE_RMCast::u64>;
template class ACE_Vector_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >;
template class ACE_Vector_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate class ACE_Array_Base<unsigned char>
# pragma instantiate class ACE_Array_Base<ACE_RMCast::u64>
# pragma instantiate class ACE_Array_Base<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >
# pragma instantiate class ACE_Array_Base<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >
# pragma instantiate class ACE_Condition<ACE_Thread_Mutex>
# pragma instantiate class ACE_Equal_To<ACE_INET_Addr>
# pragma instantiate class ACE_Hash_Map_Const_Iterator_Base_Ex<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Const_Iterator_Base_Ex<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>, ACE_Hash<unsigned short>, ACE_Equal_To<unsigned short>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Const_Iterator_Base_Ex<ACE_INET_Addr, ACE_RMCast::u64, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, ACE_RMCast::Acknowledge::Queue, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Iterator_Base_Ex<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Iterator_Base_Ex<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Entry<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr>
# pragma instantiate class ACE_Hash_Map_Entry<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr>
# pragma instantiate class ACE_Hash_Map_Entry<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex> >
# pragma instantiate class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_RMCast::u64>
# pragma instantiate class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Data, ACE_Thread_Mutex> >
# pragma instantiate class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_RMCast::Acknowledge::Queue>
# pragma instantiate class ACE_Hash_Map_Manager<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Data, ACE_Thread_Mutex>, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_RMCast::Acknowledge::Queue, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_RMCast::u64, ACE_RMCast::AddressHasher, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager_Ex<ACE_RMCast::u64, ACE_RMCast::Acknowledge::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager_Ex<ACE_RMCast::u64, ACE_RMCast::Retransmit::Descr, ACE_Hash<ACE_RMCast::u64>, ACE_Equal_To<ACE_RMCast::u64>, ACE_Null_Mutex>
# pragma instantiate class ACE_Hash_Map_Manager_Ex<unsigned short, ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>, ACE_Hash<unsigned short>, ACE_Equal_To<unsigned short>, ACE_Null_Mutex>
# pragma instantiate class ACE_Node<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >
# pragma instantiate class ACE_Node<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Data, ACE_Thread_Mutex>
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex>
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex>
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_RMCast::NAK, ACE_Thread_Mutex>
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_RMCast::NRTM, ACE_Thread_Mutex>
# pragma instantiate class ACE_Refcounted_Auto_Ptr<ACE_RMCast::Profile, ACE_Thread_Mutex>
# pragma instantiate class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >
# pragma instantiate class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >
# pragma instantiate class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >
# pragma instantiate class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >
# pragma instantiate class ACE_Vector<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >
# pragma instantiate class ACE_Vector<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >
# pragma instantiate class ACE_Vector<unsigned char>
# pragma instantiate class ACE_Vector<ACE_RMCast::u64>
# pragma instantiate class ACE_Vector_Iterator<unsigned char>
# pragma instantiate class ACE_Vector_Iterator<ACE_RMCast::u64>
# pragma instantiate class ACE_Vector_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Null_Mutex> >
# pragma instantiate class ACE_Vector_Iterator<ACE_Refcounted_Auto_Ptr<ACE_RMCast::Message, ACE_Thread_Mutex> >

#elif defined (__HP_aCC)
// Make aC++ stop complaining about an empty translation unit
static int shut_up_aCC = 0;
#endif
