#include <climits>
#include "ace/RB_Tree.h"
/*
   Much of this is credited to "Efficient Distrubuted Deadlock
   Avoidance with Liveness Guarentees" by Sanchez, Sipma, and Manna,
   EMSOFT 2006
*/

struct AnnotationNode {
  AnnotationNode() 
    :count(0), size(0), larger(0), larger_me(0), larger_left(INT_MAX), larger_right(INT_MAX)
  {
  }
  int count;  //number of processes with this annotation
  int size;  //total number of processes in subtree including this node
  int larger;  //minimum of larger_left, larger_me, and, larger_right
  int larger_me;
  int larger_left;
  int larger_right;
};

namespace {

   int min(int a, int b) {
     return (a < b)? a : b;
   }
   
   int MIN_THREE(int a, int b, int c) {
     return (a < b) ? min(a,c) : min(b,c);
   }
  
}

class Live_P_Tree : public ACE_RB_Tree<int, AnnotationNode, ACE_Equal_To<int>, ACE_Thread_Mutex> {

public:
   Live_P_Tree(int maxThreads);
   virtual ~Live_P_Tree();
   int bind(const int& ext_id);
   int unbind (const int &ext_id);
   int calc_max() const;
protected:
   void RB_rotate_right(ACE_RB_Tree_Node<int, AnnotationNode> *x);
   void RB_rotate_left(ACE_RB_Tree_Node<int, AnnotationNode> *x);
private:
   void recalculate_augmentation(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr);
   void recalculate_augmentation_up(ACE_RB_Tree_Node<int, AnnotationNode>* x);
   int calc_max_i(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr, int extra) const;
   int T_;
};

ACE_INLINE
Live_P_Tree::Live_P_Tree(int maxThreads) 
:ACE_RB_Tree<int, AnnotationNode, ACE_Equal_To<int>, ACE_Thread_Mutex>(),
 T_(maxThreads) {

}

ACE_INLINE
Live_P_Tree::~Live_P_Tree() {
}

ACE_INLINE 
int
Live_P_Tree::bind(const int& ext_id) 
{
  ACE_RB_Tree_Node<int, AnnotationNode>* entry = 0;
  int returnVal = -1;  //return error unless we return
                       //something else from the parent unbind
  RB_SearchResult result = LEFT;
  entry = find_node (ext_id, result);
  // If there is a matching node, don't add a new one, just mod the existing one
  if (entry && result == EXACT) {    
    entry->item().count++;    
  } else {
    returnVal = ACE_RB_Tree<int, 
	                        AnnotationNode, 
							ACE_Equal_To<int>, 
							ACE_Thread_Mutex>::bind(ext_id, 
							                        AnnotationNode(), 
													entry);
  }
  recalculate_augmentation_up(entry);
  return returnVal;
}

void 	
Live_P_Tree::RB_rotate_right (ACE_RB_Tree_Node<int, AnnotationNode> *x)
{
  ACE_RB_Tree<int, AnnotationNode, ACE_Equal_To<int>, ACE_Thread_Mutex>::RB_rotate_right(x);
  recalculate_augmentation_up(x);

}

void 	
Live_P_Tree::RB_rotate_left (ACE_RB_Tree_Node<int, AnnotationNode> *x)
{
  ACE_RB_Tree<int, AnnotationNode, ACE_Equal_To<int>, ACE_Thread_Mutex>::RB_rotate_left(x);
  recalculate_augmentation_up(x);
}

ACE_INLINE 
int
Live_P_Tree::unbind(const int& ext_id) 
{
  ACE_RB_Tree_Node<int, AnnotationNode>* entry = 0;
  RB_SearchResult result = LEFT;
  int returnVal = -1;  //return error unless we return
                       //something else from the parent unbind
  entry = find_node (ext_id, result);
  // If there is a matching node, don't add a new one, just mod the existing one
  if (entry && result == EXACT) {    
    if (--(entry->item().count) == 0) {
      entry = entry->parent();
      returnVal = ACE_RB_Tree<int, AnnotationNode, ACE_Equal_To<int>, ACE_Thread_Mutex>::unbind(ext_id);
    }
  } else {
    //exception?  probably bad if we try to unbind something not in the tree 
  } 
  if (entry) {
    recalculate_augmentation_up(entry);
  }
  return returnVal;
}


ACE_INLINE  void
Live_P_Tree::recalculate_augmentation(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr) {

  AnnotationNode placeholderNode;
  AnnotationNode& node = nodePtr->item();
  AnnotationNode& left =  nodePtr->left() ? placeholderNode : nodePtr->left()->item();
  AnnotationNode& right = nodePtr->right() ? placeholderNode : nodePtr->right()->item();

  // (1) size
  node.size = left.size + right.size + node.count;

  // (2) larger_me                                            
  node.larger_me = T_ - (node.count + right.size + nodePtr->key());
                                                              
  // (3) larger_right
  node.larger_right = right.larger;

  // (4) larger_left
  node.larger_left = left.larger - (right.size  + node.count);

  //(5) larger
  node.larger = MIN_THREE(node.larger_me, node.larger_left, node.larger_right);
}

ACE_INLINE void
Live_P_Tree::recalculate_augmentation_up(ACE_RB_Tree_Node<int, AnnotationNode>* x) {
  while (x) {
    recalculate_augmentation(x);
    x = x->parent();
  }
}

ACE_INLINE  int
Live_P_Tree::calc_max() const {
//   //note: need to add get_root method to RB_Tree
   return 0;//calc_max_i(get_root(), 0);
}

ACE_INLINE  int
Live_P_Tree::calc_max_i(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr, int extra) const {
  AnnotationNode& n = nodePtr->item();

  if ( n.larger_left - extra==0) { 
      return calc_max_i(nodePtr->left(), extra + nodePtr->right()->item().size + n.count); }
  else if (n.larger_me   - extra==0) { return (nodePtr->key()); }
  else if (n.larger_right - extra==0) { return calc_max_i(nodePtr->right(), extra); }
  else {   return T_; }
}

template <typename AnnotationId>
ACE_INLINE 
Live_P_Strategy<AnnotationId>::Live_P_Strategy(int maxThreads)
:DA_Strategy_Base<AnnotationId>(maxThreads),
 min_illegal_is_computed_(false),
 min_illegal_(0)
{
}

template <typename AnnotationId>
ACE_INLINE 
Live_P_Strategy<AnnotationId>::~Live_P_Strategy()
{
}



template <typename AnnotationId>
ACE_INLINE 
int
Live_P_Strategy<AnnotationId>::is_deadlock_potential(AnnotationId handle)
{
    int annotation = get_annotation(handle);
    computation_mutex_.acquire();
    if (!min_illegal_is_computed_) 
    {
      if (tree_pimpl_->current_size() > 1) 
      {	    
	      min_illegal_ = tree_pimpl_->calc_max();
      }
      min_illegal_is_computed_ = true;
    }
    computation_mutex_.release();

    if (annotation >= min_illegal_)
    {   
       return annotation - min_illegal_ + 1;
    }
   
   return 0;
}

template <typename AnnotationId>
ACE_INLINE 
void 
Live_P_Strategy<AnnotationId>::grant(AnnotationId handle)
{
  int annotation = get_annotation(handle);
  //since the state of the tree is involved in calculation
  //of max, we must aquire the lock before changing the 
  //structure of the tree
  computation_mutex_.acquire();
  tree_pimpl_->bind(annotation);  
  min_illegal_is_computed_ = false;
  computation_mutex_.release();
}

template <typename AnnotationId>
ACE_INLINE 
void 
Live_P_Strategy<AnnotationId>::release(AnnotationId handle)
{
  //since the state of the tree is involved in calculation
  //of max, we must aquire the lock before changing the 
  //structure of the tree
  computation_mutex_.acquire();
  min_illegal_is_computed_ = false;
  int annotation = get_annotation(handle);
  tree_pimpl_->unbind(annotation);
  computation_mutex_.release();
}
