#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {

  clear();

  
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {

if(p == NULL){

p = new Node(ndata);
p->next = head_;

if ( head_ != NULL){
  head_->prev = p;
}

head_ = p;

length_ +=1;

return p;

} else { // general case

Node *m = new Node(ndata);
Node *pnext = p->next;

m->next = pnext;
if ( pnext != NULL ) {
  pnext->prev = m;
}

p->next = m;
m->prev = p;

length_ +=1;

return m;
}

}



/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */


void Chain::swap(Node *p, Node *q) {
  if ( p==NULL || q==NULL || p==q) {
    return;
  }
  else {
   
    if(q->next == p){

      Node * qprev = q->prev;
      Node * pnext = p->next;

      q->next = pnext;
      q->prev = p;
     

       if(pnext != NULL){
        pnext->prev = q;
      }

      p->next = q;
      p->prev = qprev;

      if(qprev != NULL){
        qprev->next = p;
      }

      qprev = NULL;
      pnext= NULL;


    } else if (p->next == q){

      Node * pprev = p->prev;
      Node * qnext = q->next;

      p->next = qnext;
      p->prev = q;

       if(qnext != NULL){
        qnext->prev = p;
      }

      q->next = p;
      q->prev = pprev;

      if(pprev != NULL){
        pprev->next = q;
      }

      pprev = NULL;
      qnext = NULL;


    } else {

    Node* pnext = p->next;
    Node* pprev = p->prev;
    Node* qnext = q->next;
    Node* qprev = q->prev;

    p->next = qnext;
    p->prev = qprev;

    q->next = pnext;
    q->prev = pprev;

    if (qnext != NULL){
      qnext->prev = p;
    }
    if (qprev != NULL){
      qprev->next = p;
    }
    if (pprev != NULL){
      pprev->next = q;
    }
    if (pnext != NULL){
      pnext->prev = q;
    }

     
    }

     if ( p == head_ ){

      head_ = q;
      
    } else if ( q == head_){

      head_ = p;

    }

  }

}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {

  Node* curr = head_;

  while( curr != NULL){

    Node* nextNode = curr->next;

    delete(curr);

    curr =  nextNode;

  }
  head_ = NULL;
  length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
 void Chain::copy(Chain const &other) {

  Node* originalPointer = other.head_ ;

  Node* curr = NULL;

  while(originalPointer != NULL){
  curr = insertAfter(curr,originalPointer->data);

  originalPointer = originalPointer->next;}

}
/* Modifies the current chain: 
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block 
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {

  unsigned int size = length_;
  Node* counter1 = head_;
  vector<double> allMin;

  for (unsigned int i = 0 ; i < size ; i++ ){

    Block b1 = counter1->data;

    
    double minimumvalue = 1.1 ;
    Node* counter2 = head_;  

    for (unsigned int j = 0 ; j < size ; j++){
      
      if ( i != j ){

        Block b2 = counter2->data;
        double dval = b2.distanceTo(b1);
        
        if (minimumvalue > dval) {
          minimumvalue = dval;          
        }
// 
        cout << dval << endl;
        cout << minimumvalue << endl;
        cout << " " << endl;
// 
      }

      counter2=counter2->next;

    }    
// 
    cout << minimumvalue << endl;
    cout << " " << endl;
// 
    allMin.push_back(minimumvalue); 
    counter1 = counter1->next;   

  }
// 
  int i = 0 ;
  while (i < (int) allMin.size()){
  cout << allMin[i] << endl;  
  i++;
  }

i = 0;
double rsf = 1.1;
int rsfIndex;

 while (i < (int) allMin.size()){

  if(rsf == 1.1) {rsf = allMin[i]; rsfIndex = i; }

  else if ( rsf < allMin[i]){
    rsf = allMin[i];
    rsfIndex = i;

  } 
//
    cout << "" <<endl;
    cout << rsf <<endl;
    cout << rsfIndex <<endl;
    cout << "" <<endl;
//

  i++;
  }

//
  cout << rsfIndex << endl;
//

this->testSwap(rsfIndex,0);

recursiveMatch (head_ , 1);



}


void Chain::recursiveMatch(Node* p, int initialPoint){

   int size = length_;

//
    if(initialPoint == size- 1){ return ; }


    Node* mainPointer = head_;
    for( int a=0; a < initialPoint; ++a ){
    mainPointer = mainPointer->next;
  }

   double minValue = 1.1;
   int minIndex = initialPoint;
   
   Node* iteratingPointer = mainPointer;
   Block sortedBlock = p->data;

   for ( int j = initialPoint ; j < size ; j++){

    Block rightBlock = iteratingPointer->data;      
   

    double dval = sortedBlock.distanceTo(rightBlock);

    if (minValue == 1.1) {minValue = dval; minIndex=j;}

    else if (dval < minValue){

      minValue = dval;
      minIndex = j;  

  }    
// 
    cout << dval << endl;
    cout << minValue << endl;
    cout << " " << endl;
//     

  iteratingPointer= iteratingPointer->next;
  }

  testSwap(initialPoint,minIndex);

  Node* newSorted = p->next;
  int nextStartPoint = initialPoint+1;

  recursiveMatch(newSorted, nextStartPoint); 

} 





 
 
