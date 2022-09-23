class mock_transport
{
public:
  mock_transport () : id_(0), is_connected_(false), entry_(0), purging_order_ (0), purged_count_ (0) {}
  size_t id () const {return id_;}
  void id (size_t id) { this->id_ = id;}
  unsigned long purging_order () const {return purging_order_;}
  void purging_order (unsigned long purging_order) { this->purging_order_ = purging_order;}
  bool is_connected () const {return is_connected_;}
  void is_connected (bool is_connected) { this->is_connected_ = is_connected;}
  ACE_Event_Handler::Reference_Count add_reference () {return 0;}
  ACE_Event_Handler::Reference_Count remove_reference () {return 0;}
  void cache_map_entry (TCM::HASH_MAP_ENTRY *entry) {this->entry_ = entry;}
  TCM::HASH_MAP_ENTRY *cache_map_entry () {return this->entry_;}
  void close_connection () { purged_count_ = ++global_purged_count;};
  int purged_count () { return this->purged_count_;}
  bool can_be_purged () { return true;}
private:
  size_t id_;
  bool is_connected_;
  TCM::HASH_MAP_ENTRY *entry_;
  unsigned long purging_order_;
  /// When did we got purged
  int purged_count_;
};


