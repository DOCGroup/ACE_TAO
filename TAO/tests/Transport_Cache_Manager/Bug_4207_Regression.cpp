#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"

#include "tao/Transport_Cache_Manager_T.h"
#include "tao/ORB.h"

class mock_transport;
class mock_tdi;
class mock_ps;

static int global_purged_count = 0;

typedef TAO::Transport_Cache_Manager_T<mock_transport, mock_tdi, mock_ps> TCM;

#include "mock_ps.h"

class mock_transport
{
public:
  mock_transport () : id_(0), is_connected_(true), purging_order_ (0), purged_count_ (0) {memset(&entry_,0,sizeof(entry_));}
  size_t id (void) const {return id_;}
  void id (size_t id) { this->id_ = id;}
  unsigned long purging_order (void) const {return purging_order_;}
  void purging_order (unsigned long purging_order) { this->purging_order_ = purging_order;}
  bool is_connected (void) const {return is_connected_;}
  void is_connected (bool is_connected) { this->is_connected_ = is_connected;}
  ACE_Event_Handler::Reference_Count add_reference (void) {return 0;}
  ACE_Event_Handler::Reference_Count remove_reference (void) {return 0;}
  void cache_map_entry (TCM::HASH_MAP_ENTRY_REF& entry) {this->entry_ = entry;}
  const TCM::HASH_MAP_ENTRY_REF& cache_map_entry (void) {return this->entry_;}
  void close_connection (void) { purged_count_ = ++global_purged_count;};
  int purged_count (void) { return this->purged_count_;}
  bool can_be_purged (void) { return true;}

  struct mock_ws
  {
    bool non_blocking() const { return true; }
    void is_registered (bool) {}
  };
  struct mock_cf
  {
    bool use_cleanup_options() const { return false; }
  };
  struct mock_oc
  {
    mock_cf* client_factory() { return 0; }
    ACE_Reactor* reactor() { return 0; }
  };
  ACE_Event_Handler* event_handler_i () const { return 0; }
  mock_oc* orb_core() { return 0; }
  mock_ws mock_ws_;
  mock_ws *wait_strategy () { return &mock_ws_; }
private:
  size_t id_;
  bool is_connected_;
  TCM::HASH_MAP_ENTRY_REF entry_;
  unsigned long purging_order_;
  /// When did we got purged
  int purged_count_;
};

struct mock_tdi
{
  u_long hash (void) const {return 10;}
  mock_tdi *duplicate (void)
  {
    return new mock_tdi;
  }
  // simulate same IP:PORT
  CORBA::Boolean is_equivalent (const mock_tdi *) {return true;}
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  try
    {
      // We need an ORB to get an ORB core
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      const size_t transport_max = 2;
      mock_transport mytransport[transport_max];
      mock_tdi mytdi[transport_max];
      mock_ps* myps = new mock_ps (10);
      TCM my_cache (20, myps, 20, false, 0);

      mytransport[0].id(10);
      mytransport[1].id(11);

      // Cache all transports in the cache
      for (size_t i = 0; i < transport_max; i++)
        {
          my_cache.cache_transport (&mytdi[i], &mytransport[i]);
        }

      if (my_cache.current_size () != transport_max)
        {
          ACE_ERROR ((LM_ERROR, "ERROR Incorrect cache size %d\n", my_cache.current_size ()));
          ++result;
        }

      mock_transport* found_transport = 0;
      size_t busy_count;
      TCM::Find_Result fr = my_cache.find_transport (
        &mytdi[0],
        found_transport, busy_count);
      if (fr != TCM::CACHE_FOUND_AVAILABLE)
        {
          ACE_ERROR ((LM_ERROR, "ERROR Failed to find transport #0:%d\n", fr));
          ++result;
        }
      // transport #0 is busy

      fr = my_cache.find_transport (
        &mytdi[0],
        found_transport, busy_count);
      if (fr != TCM::CACHE_FOUND_AVAILABLE)
        {
          ACE_ERROR ((LM_ERROR, "ERROR Failed to find transport #1:%d\n", fr));
          ++result;
        }
      // transport #1 is busy

      mytransport[0].cache_map_entry().int_id_->recycle_state (TAO::ENTRY_IDLE_AND_PURGABLE);
      mytransport[1].cache_map_entry().int_id_->recycle_state (TAO::ENTRY_IDLE_AND_PURGABLE);
      // both transports are idle
      fr = my_cache.find_transport (
        &mytdi[0],
        found_transport, busy_count);
      if (fr != TCM::CACHE_FOUND_AVAILABLE)
        {
          ACE_ERROR ((LM_ERROR, "ERROR Failed to find transport #2:%d\n", fr));
          ++result;
        }

      // release transport #0
      TCM::HASH_MAP_ENTRY_REF entry0 = mytransport[0].cache_map_entry();
      if (my_cache.purge_entry (entry0) || my_cache.current_size() != 1)
        {
          ACE_ERROR ((LM_ERROR, "ERROR Failed to remove entry %d\n", my_cache.current_size ()));
          ++result;
        }

      // transport #1 should be available now
      fr = my_cache.find_transport (
        &mytdi[1], found_transport, busy_count);
      if (fr != TCM::CACHE_FOUND_AVAILABLE)
        {
          ACE_ERROR ((LM_ERROR, "ERROR Failed to find transport #1 after purging #0:%d\n", fr));
          ++result;
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions..
    }
  return result;
}
