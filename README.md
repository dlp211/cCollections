# cCollections
cCollections is a disjoint set of data structure containers for the C
programming language. It focuses on alleviating the burden of managing memory
within the data structures.

The data structures are NOT thread safe.

## sda
sda is a vector like structure that acts like a dynamically allocated array by
storing header information in front of the array and accessing that information
through function calls.

## rbtree
rbtree is a red-black tree based on the code presented in Introduction to
Algorithms by Cormen et al.  It has been modified to handle key collisions with
the option to operate on a value during an insert. It support custom keys and
values and acts a full container, not a pointer to the key/value.
