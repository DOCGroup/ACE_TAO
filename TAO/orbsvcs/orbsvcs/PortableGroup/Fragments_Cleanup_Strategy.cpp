#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/PortableGroup/Fragments_Cleanup_Strategy.h"
#include "orbsvcs/PortableGroup/UIPMC_Transport_Recv_Packet.h"
#include <algorithm>
#include <memory>

#if !defined (__ACE_INLINE__)
# include "orbsvcs/PortableGroup/Fragments_Cleanup_Strategy.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  using HASH_MAP_ITER = TAO_UIPMC_Mcast_Transport::Packets_Map::iterator;
  using HASH_MAP_ENTRY = TAO_UIPMC_Mcast_Transport::Packets_Map::value_type;
  using DESCRIPTOR_SET = HASH_MAP_ENTRY**;

  void
  sort_set (DESCRIPTOR_SET &entries, int current_size)
  {
    if (current_size < 2)
      {
        return;
      }

    std::sort (entries, entries + current_size,
      [] (HASH_MAP_ENTRY const *left, HASH_MAP_ENTRY const *right)
      {
        return left->int_id_->started () < right->int_id_->started ();
      });
  }

}

namespace TAO_PG
{
  void
  Time_Bound_Fragments_Cleanup_Strategy::cleanup (
    TAO_UIPMC_Mcast_Transport::Packets_Map &packets)
  {
     // bound_ is in milliseconds.
     ACE_Time_Value const delay (0, 1000 * this->bound_);
     ACE_Time_Value const now = ACE_OS::gettimeofday ();

    for (HASH_MAP_ITER iter = packets.begin ();
         iter != packets.end ();)
      {
        // Move forward iter because what it was pointing to could be
        // unbound at the end of the loop leaving the iterator pointing
        // to removed entry.
        HASH_MAP_ITER cur_iter = iter++;

        if ((*cur_iter).item ()->started () != ACE_Time_Value::zero &&
            now <= (*cur_iter).item ()->started () + delay)
        {
          continue;
        }

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

        std::unique_ptr<TAO_PG::UIPMC_Recv_Packet> guard ((*cur_iter).item ());
        packets.unbind (cur_iter);
      }
  }

  void
  Number_Bound_Fragments_Cleanup_Strategy::cleanup (
    TAO_UIPMC_Mcast_Transport::Packets_Map &packets)
  {
    int const current_size = static_cast<int> (packets.current_size ());

    // bound_ is a number of packets.
    if (current_size <= this->bound_)
    {
      return;
    }

    DESCRIPTOR_SET sorted_set;
    ACE_NEW (sorted_set, HASH_MAP_ENTRY*[current_size]);
    std::unique_ptr<HASH_MAP_ENTRY*[]> owner (sorted_set);

    HASH_MAP_ITER iter = packets.begin ();

    for (int i {}; i < current_size; ++i)
      {
        sorted_set[i] = std::addressof (*iter);
        ++iter;
      }

    // Sort in ascending order.
    sort_set (sorted_set, current_size);

    // Since started() can return ACE_Time_Value::zero (lowest possible value)
    // then those broken packets will in the beginning of the sorted_set.
    for (int i {}; i < current_size; ++i)
      {
        if (current_size - i <= this->bound_)
        {
          break;
        }

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

        std::unique_ptr<TAO_PG::UIPMC_Recv_Packet> guard (sorted_set[i]->item ());
        packets.unbind (sorted_set[i]);
      }
  }

  void
  Memory_Bound_Fragments_Cleanup_Strategy::cleanup (
    TAO_UIPMC_Mcast_Transport::Packets_Map &packets)
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
              {
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("TAO (%P|%t) - MBFCS::cleanup, ")
                                ACE_TEXT ("cleaning broken %d bytes (hash %d)\n"),
                                (*cur_iter).item ()->data_length (),
                                (*cur_iter).key ()));
              }

            std::unique_ptr<TAO_PG::UIPMC_Recv_Packet> guard ((*cur_iter).item ());
            packets.unbind (cur_iter);
          }
        else
        {
          size += (*cur_iter).item ()->data_length ();
        }
      }

    // bound_ is a number of bytes.
    if (static_cast<int> (size) <= this->bound_)
    {
      return;
    }

    int const current_size = static_cast<int> (packets.current_size ());

    DESCRIPTOR_SET sorted_set;
    ACE_NEW (sorted_set, HASH_MAP_ENTRY*[current_size]);
    std::unique_ptr<HASH_MAP_ENTRY*[]> owner (sorted_set);

    iter = packets.begin ();

    for (int i {}; i < current_size; ++i)
      {
        sorted_set[i] = std::addressof (*iter);
        ++iter;
      }

    sort_set (sorted_set, current_size);

    // We have to clean all those packets above bound.
    for (int i {}; i < current_size; ++i)
      {
        if (static_cast<int> (size) <= this->bound_)
        {
          break;
        }

        if (TAO_debug_level >= 8)
          {
            ORBSVCS_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("TAO (%P|%t) - MBFCS::cleanup, ")
                            ACE_TEXT ("cleaning %d bytes (hash %d)\n"),
                            sorted_set[i]->item ()->data_length (),
                            sorted_set[i]->key ()));
          }

        size -= sorted_set[i]->item ()->data_length ();
        std::unique_ptr<TAO_PG::UIPMC_Recv_Packet> guard (sorted_set[i]->item ());
        packets.unbind (sorted_set[i]);
      }
  }

} // namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL
