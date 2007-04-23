#include <climits>

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

class Live_P_Tree : public ACE_RB_Tree<int, AnnotationNode, ACE_Equal_To<int>, ACE_ThreadMutex> {

public:
   Live_P_Tree(int maxThreads);
   virtual ~Live_P_Tree();
   int bind(const int& ext_id, 
            const AnnotationNode& int_id, 
            ACE_RB_Tree_Node<int, AnnotationNode>*& entry);
   int unbind (const EXT_ID &ext_id);
   int calc_max() const;
private:
   void recalculate_augmentation(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr);
   void recalculate_augmentation_up(ACE_RB_Tree_Node<int, AnnotationNode>* x);
   int calc_max_i(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr, int extra) const;
   static int MIN(int a, int b) { return (a<b)?a:b; }
   static int MIN_THREE(int a, int b, int c) {
     return (a<b)?MIN(a,c):MIN(b,c);
   }
   int T_;
};

ACE_INLINE
Live_P_Tree::Live_P_Tree(int maxThreads) 
:ACE_RB_Tree(),
 T_(maxThreads) {

}

ACE_INLINE
Live_P_Tree::~Live_P_Tree() {
}

ACE_INLINE 
Live_P_Tree::bind(const int& ext_id) 
{
  ACE_RB_Tree_Node<int, AnnotationNode>* entry = 0;
  RB_SearchResult result = LEFT;
  entry = find_node (ext_id, result);
  // If there is a matching node, don't add a new one, just mod the existing one
  if (entry && result == EXACT) {
    entry->item.count++;
  } else {
    RB_Tree::bind(ext_id, AnnotationNode(), entry);
  }
  recalculate_augmentation_up(entry);

}

ACE_INLINE 
Live_P_Tree::unbind(const int& ext_id) 
{
  ACE_RB_Tree_Node<int, AnnotationNode>* entry = 0;
  RB_SearchResult result = LEFT;
  entry = find_node (ext_id, result);
  // If there is a matching node, don't add a new one, just mod the existing one
  if (entry && result == EXACT) {
    entry->item.count--;
    if (count == 0) {
      entry = entry->parent;
      RB_Tree::unbind(ext_id);
    }
  } else {
    //exception?  probably bad if we try to unbind something not in the tree 
  } 
  if (entry) {
    recalculate_augmentation_up(entry);
  }

}


ACE_INLINE  void
Live_P_Tree::recalculate_augmentation(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr) {

  AnnotationNode& node = n->item;
  AnnotationNode& left =  nodePtr->left ? AnnotationNode() : nodePtr->left->item;
  AnnotationNode& right = nodePtr->right ? AnnotationNode() : nodePtr->right->item;

  // (1) size
  node.size = left.size + right.size + node.count;

  // (2) larger_me                                            
  node.larger_me = ProcessCount(T_) - (node.count + right.size/* + node.alpha*/);
                                                              //^^^^^^^^^^^^^ not sure what this is for
  // (3) larger_right
  Node.larger_right = right.larger;

  // (4) larger_left
  Node.larger_left = left.larger - (right.size  + node.count);

  //(5) larger
  Node.larger = MIN_THREE(x.larger_me, x.larger_left, x.larger_right);
}

ACE_INLINE void
Live_P_Tree::recalculate_augmentation_up(ACE_RB_Tree_Node<int, AnnotationNode>* x) {
  while (x) {
    recalculate_augmentation(x);
    x = x->parent;
  }
}

ACE_INLINE  int
Live_P_Tree::calc_max() const {
   //note: need to add get_root method to RB_Tree
   return calc_max_i(get_root(), 0);
}

ACE_INLINE  int
Live_P_Tree::calc_max_i(ACE_RB_Tree_Node<int, AnnotationNode>* nodePtr, int extra) const {
  AnnotationNode n = nodePtr->item;

  if ( n.larger_left - extra==0) { 
      return calc_max_i(nodePtr->left, extra + nodePtr->right->item.size + n.count); }
  else if (n->larger_me   - extra==0) { return (nodePtr->key); }
  else if (n->larger_right - extra==0) { return calc_max_i(nodePtr->right, extra); }
  else {   return T_; }
}

template <typename AnnotationId>
ACE_INLINE 
Live_P_Strategy<AnnotationId>::Live_P_Strategy(int maxThreads)
:DA_Strategy_Base(maxThreads)
{
}

template <typename AnnotationId>
ACE_INLINE 
Live_P_Strategy<AnnotationId>::~Live_P_Strategy()
{
}



template <typename AnnotationId>
ACE_INLINE 
bool 
Live_P_Strategy<AnnotationId>::is_deadlock_potential(AnnotationId handle)
{
    int min_illegal = getMaxThreads();
    int annotation = getAnnotation(handle);

    if (!min_ilegal_is_computed_) 
    {
      if (tree_->current_size() > 1) 
      {	    
	      min_ilegal_ = calc_max();
      }
      min_ilegal_is_computed_ = true;
    }

    return annotation >= min_ilegal_;
}

template <typename AnnotationId>
ACE_INLINE 
void 
Live_P_Strategy<AnnotationId>::grant(AnnotationId handle)
{
  int annotation = getAnnotation(handle);
  tree_pimpl_->bind(annotation);
  min_ilegal_is_computed_ = false;
}

template <typename AnnotationId>
ACE_INLINE 
void 
Live_P_Strategy<AnnotationId>::release(AnnotationId handle)
{
  min_ilegal_is_computed_ = false;
  int annotation = getAnnotation(handle);
  tree_pimpl_->unbind(annotation);
}