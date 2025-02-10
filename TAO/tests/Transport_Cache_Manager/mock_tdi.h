class mock_tdi
{
public:
  mock_tdi () = default;
  u_long hash () {return static_cast<u_long> (reinterpret_cast<ptrdiff_t> (this));}
  mock_tdi *duplicate () {return 0;}
  CORBA::Boolean is_equivalent (const mock_tdi *) {return true;}
};

