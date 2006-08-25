#include <vector>
#include "sbp.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace SBP;
using namespace std;


namespace SBP{
  int nitems, nbins;
  int bins_need_sorting = 1;
  int debug = 1;
  int na;
  int bfwf = 0;                    // TOOD, this should be a "in" param to allocate()
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

  bool itemSortCriteria_dec (const item &one, const item &two) {
    return one.size > two.size;
  }

  bool itemSortCriteria_inc (const item &one, const item &two) {
    return one.size < two.size;
  }

  bool binSortCriteria (const bin &one, const bin &two) {
    double space1, space2;
    space1 = one.capacity - one.occupancy;
    space2 = two.capacity - two.occupancy;

    switch (bfwf) {
    case 0:
      if (space1 < space2) return true;
      if (space1 > space2) return false;
      if (one.bin_num < two.bin_num) return true;
      break;
    case 1:
      if (space1 > space2) return true;
      if (space1 < space2) return false;
      if (one.bin_num < two.bin_num) return true;
      break;
    default:
      cout << "Invalid value for bfwf = " << bfwf << endl;
      exit (1);
    }
    return false;
  }

  int pick_bin_best_fit(std::vector <bin> & bins, std::vector <item> & items) {
    int b2 = 0;
    for (b2 = 0; b2 < nbins; b2++) {
      if (bins[b2].size() > 0.0) {
        if ((bins[b2].size()  >= items[na].size) &&
            (bins[b2].bin_num >= items[na].allocated_to_bin)) {
          return b2;
        }
      }
    }
    return b2;
  }

  int pick_bin_worst_fit(std::vector <bin> & /* bins */, std::vector <item> & /* items */) {
    assert ("pick_bin_worst_fit not yet coded." == 0);
    return nbins; 
  }

  int pick_bin(std::vector <bin> & bins, std::vector <item> & items) {
    if (bfwf == 0)
      return pick_bin_best_fit(bins, items);
    if (bfwf == 1)
      return pick_bin_worst_fit(bins, items);
    assert("can not happend" == 0);
    return 0;
  }

  int allocate (std::vector <bin> &bins, std::vector <item> & items) {
    long long int steps, btracks;
    
    steps = btracks = 0;
    na = 0;
    int success = 0;
    nitems = items.size();
    nbins = bins.size();
    sort(items.begin(), items.end(), itemSortCriteria_dec);
    
    while (na < nitems) {
      if(bins_need_sorting) {
        sort(bins.begin(), bins.end(), binSortCriteria);
        bins_need_sorting = 0;
      }
      steps++;
      if (debug) {
        cout << "------------- Step " << steps << " ----------"
             << " btracks " << btracks << " --------" << endl;
        cout << "na = " << na << " Item has size = "
             << items[na].size << " allocated_to_bin " << items[na].allocated_to_bin
             << endl;

        for (int i = 0; i < nitems; i++) {
          print_item(items[i]);
        }
        cout << endl;
        for (int i = 0; i < nbins; i++) {
          print_bin(bins[i], items);
        }
        cout << endl;

        if (!bfwf && items[na].size > bins[nbins-1].size()) {
          cout << " ### will not fit in any bin ### ";
        }
        if (bfwf && items[na].size > bins[0].size()) {
          cout << " ### will not fit in any bin ### ";
        }
        cout << endl;
        //         output_bins_and_items();
      }
      int b2;
      b2 = pick_bin(bins, items);
      if (b2 >= nbins) {
      backtrack:
        if (debug) cout << "start of backtrack code. btracks " << btracks << endl;
        btracks++;
        na--;
        if (na < 0) {
          cout << "######### FAILURE !! ##############" << endl;
          goto done;
        }
        if (debug) {
          cout << "na reduced to " << na << endl;
        }
        assert (items[na].allocated_to_bin != -1); // sanity check
        bins[items[na].allocated_to_bin].occupancy -= items[na].size; // take item out of bin.
        items[na].allocated_to_bin = -1;

        bins_need_sorting = 1;     // we have changed bin space, mark for sorting 
        items[na].allocated_to_bin = -1;       // note that item is unallocated.
        items[na].next_bin_to_try++;       // next time we must try the next bin.
        if (items[na].next_bin_to_try >= nbins) { // if no more bins to try, must backtrack
          goto backtrack;
        }
      }
      else {
        bins[b2].occupancy += items[na].size; // put item in bin
        bins_need_sorting = 1;     // we changed bin space, mark for sorting
        items[na].allocated_to_bin = bins[b2].bin_num; // really, same as "b2"
        items[na].next_bin_to_try = items[na].allocated_to_bin; // will be incremented if we backtrack

        if (debug) {
          cout << "na(item_num) " << na
               << " allocated to bin " << items[na].allocated_to_bin << endl;
        }
        na++;
        if (debug) cout << "na increased to " << na << endl;
      }
    }
    success++;
  done:
    return success;
  }

  void print_item(item & item) {
    cout <<  " Item_Num " << item.item_num <<  " [ size "
         << item.size << ", ";
    if (item.allocated_to_bin >=0 ) {
      cout << "Allocated_to_bin ";
      cout << item.allocated_to_bin;
    }
    else         cout << "   Not_Allocated    ";
    cout <<  "]" << endl;
  }

  void print_bin(bin &b,  Items &items) {
    cout << " [  bin " << b.bin_num << ", capacity  "
         << b.capacity << " ,  occupancy " << b.occupancy
         << "  space " << b.size() << " ] Items: ";
    for (int i = 0; i < nitems; i++) {
      if (items[i].allocated_to_bin != -1 && items[i].allocated_to_bin == b.bin_num) {
        cout << "[" << i << " , " << items[i].size << " ] ";
      }
    }
    cout << endl;
  }

  void item::fill_item(int i, double s) {
    item_num = i;
    size = s;
    allocated_to_bin = -1;           // -1 indicates not allocated
    next_bin_to_try = 0;
  }

}
