class KEY
// ============================================================================
// = TITLE
//    Define a key for use with the Map_Manager_Test.
//
// = DESCRIPTION
//    This class gets its own header file to work around AIX C++
//    compiler "features" related to template instantiation...  It is
//    only used by Map_Manager_Test.cpp  
// ============================================================================
{
public:
  KEY (size_t v = 0): value_ (v)
  { }

  size_t hash (void) const { return this->value_; }
  operator size_t () const { return this->value_; }

private:
  size_t value_;
};

