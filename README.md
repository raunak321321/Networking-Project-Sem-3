# **Networking-Project**
## The project encompasses a derivation of my own AVL-tree-based algorithm for resource block allocation to the given LTE network, where the number of resource blocks (RBs) is allocated to User Equipment (UEs) based on the demanded application by each pair.
## Main Target - 
To Provide higher priority UEs more
RBs as compared to what they will get normally.
## AVL-Tree
AVL tree is a self-balancing Binary Search Tree (BST)
where the difference between heights of left and right subtrees
cannot be more than one for all nodes.
1) `Advantages` :<br>
  &emsp; i. The height of the AVL tree is always balanced.<br>
  &emsp; ii. It gives better search time complexity when compared to
simple Binary Search trees.<br> 
  &emsp; iii. AVL trees have self-balancing capabilities.<br>
  &emsp; iv. Using these advantages we can give more rbs to the right side of the tree as compared to left side of the tree.<br>
  &emsp; v. Means higher priorities UEs will get more RBs as compared to lower priorities.<br>
  
## How to `Run`?
      `./waf --run scratch/lte-priority` to run lte-priority which is there in scratch folder of ns3.
