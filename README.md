# First-Fit Memory Allocation with Explicit Free List
 This repository contains a C implementation of memory allocation and deallocation using the First-Fit strategy with explicit free list maintenance.
 <br>
 <h3>Features : </h3>
   First-Fit Strategy: The algorithm ensures that the first free bock is assigned as per user request<br>
   Explicit Free List: The algorithm makes use of two different lists of metadata regarding free and allocated blocks to keep a track of it<br>
   Block Merging: Adjacent free blocks are merged together during deallocation to reduce fragmentation and maximize memory utilization.<br>
