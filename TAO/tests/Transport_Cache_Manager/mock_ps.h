class mock_ps
{
public:
  explicit mock_ps (int max) : maximum_ (max) {}
  void update_item (mock_transport&) {}
  int cache_maximum () const { return this->maximum_;}

  mock_ps (mock_ps const &) = delete;
  mock_ps (mock_ps &&) = delete;
  mock_ps &operator= (mock_ps const &) = delete;
  mock_ps &operator= (mock_ps &&) = delete;
private:
  int maximum_;
};


