// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/PortableGroup/Fragments_Cleanup_Strategy.h"
#include "orbsvcs/PortableGroup/UIPMC_Transport_Recv_Packet.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/PortableGroup/Fragments_Cleanup_Strategy.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  typedef TAO_UIPMC_Mcast_Transport::Packets_Map::iterator HASH_MAP_ITER;
  typedef TAO_UIPMC_Mcast_Transport::Packets_Map::value_type HASH_MAP_ENTRY;
  typedef HASH_MAP_ENTRY** DESCRIPTOR_SET;

  // The two functions below are stolen from Transport_Cache_Manager.
#if !defined (ACE_LACKS_QSORT)
  int
  cpscmp (void const *a, void const *b)
  {
    HASH_MAP_ENTRY * const * left  =
      reinterpret_cast<HASH_MAP_ENTRY * const *> (a);
    HASH_MAP_ENTRY * const * right =
      reinterpret_cast<HASH_MAP_ENTRY * const *> (b);

    if ((*left)->int_id_->started () < (*right)->int_id_->started ())
      return -1;

    if ((*left)->int_id_->started () > (*right)->int_id_->started ())
      return 1;

    return 0;
  }
#endif /* ACE_LACKS_QSORT */

  void
  sort_set (DESCRIPTOR_SET &entries, int current_size)
  {
#if defined (ACE_LACKS_QSORT)
    // Use insertion sort if we don't have qsort
    for(int i = 1; i < current_size; ++i)
      {
        if (entries[i]->int_id_->started () <
            entries[i - 1]->int_id_->started ())
          {
            HASH_MAP_ENTRY* entry = entries[i];

            for(int j = i; j > 0 &&
                  entries[j - 1]->int_id_->started () >
                  entry->int_id_->started (); --j)
              {
                HASH_MAP_ENTRY* holder = entries[j];
                entries[j] = entries[j - 1];
                entries[j - 1] = holder;
              }
          }
      }
#else
    ACE_OS::qsort (entries, current_size,
                   sizeof (HASH_MAP_ENTRY *),
                   reinterpret_cast<ACE_COMPARE_FUNC> (cpscmp));
#endif /* ACE_LACKS_QSORT */
  }

}

namespace TAO_PG
{

  Fragments_Cleanup_Strategy::~Fragments_Cleanup_Strategy (void)
  {
  }

  void
  Time_Bound_Fragments_Cleanup_Strategy::cleanup (
    TAO_UIPMC_Mcast_Transport::Packets_Map &packets
  )
  {
    for (HASH_MAP_ITER iter = packets.begin ();
         iter != packets.end ();)
      {
        // Move forward iter because what it was pointing to could be
        // unbound at the end of the loop leaving the iterator pointing
        // to removed entry.
        HASH_MAP_ITER cur_iter = iter++;

        // bound_ is in milliseconds.
        ACE_Time_Value const delay (0, 1000 * this->bound_);
        ACE_Time_Value const now = ACE_OS::gettimeofday ();

        if ((*cur_iter).item ()->started () != ACE_Time_Value::zero &&
            now <= (*cur_iter).item ()->started () + delay)
          continue;

        if (TAO_debug_level >= 8)
          {
            ACE_TCHAR const *b =
              (*cur_iter).item ()->started () == ACE_Time_Value::zero ?
              ACE_TEXT ("broken ") : ACE_TEXT ("");

            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - TBFCS::cleanup, ")
                        ACE_TEXT ("cleaning %s%d bytes (hash %d)\n"),
                        b, (*cur_iter).item ()->data_length (),
                        (*cur_iter).key ()));
          }

        ACE_Auto_Ptr<TAO_PG::UIPMC_Recv_Packet> guard ((*cur_iter).item ());
        packets.unbind (cur_iter);
      }
  }

  void
  Number_Bound_Fragments_Cleanup_Strategy::cleanup (
    TAO_UIPMC_Mcast_Transport::Packets_Map &packets
  )
  {
    int const current_size = static_cast<int> (packets.current_size ());

    // bound_ is a number of packets.
    if (current_size <= this->bound_)
      return;

    DESCRIPTOR_SET sorted_set;
    ACE_NEW (sorted_set, HASH_MAP_ENTRY*[current_size]);
    ACE_Auto_Array_Ptr<HASH_MAP_ENTRY*> owner (sorted_set);

    HASH_MAP_ITER iter = packets.begin ();

    for (int i = 0; i < current_size; ++i)
      {
        sorted_set[i] = &(*iter);
        ++iter;
      }

    // Sort in ascending order.
    sort_set (sorted_set, current_size);

    // Since started() can return ACE_Time_Value::zero (lowest possible value)
    // then those broken packets will in the beginning of the sorted_set.
    for (int i = 0; i < current_size; ++i)
      {
        if (current_size - i <= this->bound_)
          break;

        if (TAO_debug_level >= 8)
          {
            ACE_TCHAR const *b =
              sorted_set[i]->item ()->started () == ACE_Time_Value::zero ?
              ACE_TEXT ("broken ") : ACE_TEXT ("");

            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - NBFCS::cleanup, ")
                        ACE_TEXT ("cleaning %s%d bytes (hash %d)\n"),
                        b, sorted_set[i]->item ()->data_length (),
                        sorted_set[i]->key ()));
          }

        ACE_Auto_Ptr<TAO_PG::UIPMC_Recv_Packet> guard (sorted_set[i]->item ());
        packets.unbind (sorted_set[i]);
      }
  }

  void
  Memory_Bound_Fragments_Cleanup_Strategy::cleanup (
    TAO_UIPMC_Mcast_Transport::Packets_Map &packets
  )
  {
    // First we need to calculate the size of packets. Since we anyway run
    // this loop we can also cleanup broken packets.
    CORBA::ULong size = 0;
    HASH_MAP_ITER iter = packets.begin ();

    while (iter != packets.end ())
      {
        // Move forward iter because what it was pointing to could be
        // unbound at the end of the loop leaving the iterator pointing
        // to removed entry.
        HASH_MAP_ITER cur_iter = iter++;

        if ((*cur_iter).item ()->started () == ACE_Time_Value::zero)
          {
            if (TAO_debug_level >= 8)
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - MBFCS::cleanup, ")
                          ACE_TEXT ("cleaning broken %d bytes (hash %d)\n"),
                          (*cur_iter).item ()->data_length (),
                          (*cur_iter).key ()));

            ACE_Auto_Ptr<TAO_PG::UIPMC_Recv_Packet> guard ((*cur_iter).item ());
            packets.unbind (cur_iter);
          }
        else
          size += (*cur_iter).item ()->data_length ();
      }

    // bound_ is a number of bytes.
    if (static_cast<int> (size) <= this->bound_)
      return;

    int const current_size = static_cast<int> (packets.current_size ());

    DESCRIPTOR_SET sorted_set;
    ACE_NEW (sorted_set, HASH_MAP_ENTRY*[current_size]);
    ACE_Auto_Array_Ptr<HASH_MAP_ENTRY*> owner (sorted_set);

    iter = packets.begin ();

    for (int i = 0; i < current_size; ++i)
      {
        sorted_set[i] = &(*iter);
        ++iter;
      }

    sort_set (sorted_set, current_size);

    // We have to clean all those packets above bound.
    for (int i = 0; i < current_size; ++i)
      {
        if (static_cast<int> (size) <= this->bound_)
          break;

        if (TAO_debug_level >= 8)
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - MBFCS::cleanup, ")
                      ACE_TEXT ("cleaning %d bytes (hash %d)\n"),
                      sorted_set[i]->item ()->data_length (),
                      sorted_set[i]->key ()));

        size -= sorted_set[i]->item ()->data_length ();
        ACE_Auto_Ptr<TAO_PG::UIPMC_Recv_Packet> guard (sorted_set[i]->item ());
        packets.unbind (sorted_set[i]);
      }
  }

} // namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL
