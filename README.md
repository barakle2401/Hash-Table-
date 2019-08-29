# Hash-Table-
A hash table is a data structure used to implement an associative array, a structure that can map keys to values. A hash table uses a hash function to compute an index into an array of buckets or slots. This program will implement hash table where at each array index, we will create a Linked List, to prevent key collision. Key having same index will be stored in Linked List as it can store multiple data.  
The hash table support these three operations:
1. Adding an element
2. Removing an element
3. Searching for an element 
All of the operations listed above are performed in an average time of O(1)

Handling Collisions
The size of an array is generally smaller than the number of possible keys given. Thus, more than one
key can be mapped by a specific hash function to a single location in the array which forms the hash
table. There are various ways to deal with this issue. In this program i will use two simple methods
together: linked lists and dynamic expansion of the hash table. 

a. Linked Lists:
In each element in the hash table there is a pointer to the head of a list. Any new object that maps to
this element in the hash table, will be added to the end of this list.

b. Table Duplication:
I will build an implementation in which no more than t elements can be mapped to the same entry in
the hash table. 
When need to add an element to an index i where there are already t elements, go to
the first location after i where there is room.
If no place was found before the next original location in the table, or the end of the table was reached,
the entire table size will be cloned (doubling the size of the table) in the following manner:
All existed entries of the table 0-(n-1) will be mapped to the respective even entries in the new
table (this is accomplished by multiplying the original index by 2).
If an additional expansion of this table is required, this process is repeated.
In this manner the distance between entries of the original table are always a power of two, e.g.
after one expansion the original entries are at location 0,2,4,6… after two expansions
0,4,8,12,16… after three expansions 0,8,16,24… etc. 
