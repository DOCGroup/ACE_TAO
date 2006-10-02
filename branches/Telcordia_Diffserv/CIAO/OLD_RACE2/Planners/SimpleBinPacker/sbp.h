#include <vector>
#include "Allocation_Utils.h"

namespace SBP {

  struct bin  // state of one bin in the resource allocation problem
  {
    typedef int capacity_type;

    bin (int _bin_num, int _capacity)
    {
      fill_bin (_bin_num, _capacity);
    }

    int bin_num;
    int capacity;      // number of processors
    double occupancy;  // between 0 and capacity

    void fill_bin(int _bin_num, int _capacity) {
      bin_num = _bin_num;
      capacity = _capacity;
      occupancy = 0;
    };

    double bin::size() {
      return capacity - occupancy;
    }
  };

  struct item  // the item to be allocated
  {
    item (int i, double s)
    {
      fill_item (i, s);
    }

    int item_num;
    double size;   // size of this item
    int allocated_to_bin;   // the index of the bin to which
                     // this item has been allocated,
                     // or -1 if this item is not yet
                     // allocated.
    int next_bin_to_try;
    void fill_item(int i, double s);
  };

  /**
   *  The allocation function.
   *
   *  @return  1 on success.  The fields bin.occupancy and
   *           and item.bin_index have been updated to reflect
   *           the allocation.
   *
   *  @return  0 on failure.  The state of reference parameters
   *           is undefined.
   */

  typedef std::vector<bin> Bins;
  typedef std::vector<item> Items;

  int allocate (Bins &,
                Items &);


  void print_item(item & item);
  void print_bin(bin &b, Items &);

  typedef CIAO::RACE::Allocation_Utils::Format_Converter < Bins, Items > SBP_Converter;

  typedef CIAO::RACE::Allocation_Utils::Allocation_Thread_Adapter < Bins, Items, &allocate > SBP_Task;

} // namespace SBP
