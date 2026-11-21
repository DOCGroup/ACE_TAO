ACE_INLINE
Database_i::Database_i (CORBA::ORB_ptr orb,
                        Test::Index max_range)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    max_range_ (max_range),
    elements_ (std::make_unique<Test::OctetSeq[]> (max_range))
{}
