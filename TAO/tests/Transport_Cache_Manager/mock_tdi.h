class mock_tdi
{
public:
  mock_tdi () = default;
  u_long hash () const
  {
    return static_cast<u_long> (reinterpret_cast<ptrdiff_t> (this));
  }

  mock_tdi *duplicate () {return nullptr;}
  CORBA::Boolean is_equivalent (mock_tdi const *) const {return true;}
};

