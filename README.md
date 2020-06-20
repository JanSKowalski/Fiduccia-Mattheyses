# Fiduccia-Mattheyses
An implementation of the FM partitioning algorithm

### The notion of a cells and nets are essential to understanding this code.

Cells represent VLSI circuits that would occupy area on a chip. Nets represent the interconnect between cells, connecting inputs and outputs.
In graph theory terms, cells are nodes and nets are hypergraphs (generalized edges). Cells and nets exist in a many-to-many relationship; that is, nets have at least one cell and cells are a part of at least one net. 



### The partitioning problem looks to efficiently divide a collection of cells into two collections such that the number of connections/nets between the two partitions is minimized. 
F and M also introduced the notion of a balance ratio, specified by the user. Within a certain tolerance, the final partioned area should match this ratio.

This code is then tested on the ISPD98 suite of testbenching circuits, stored in .are and .netD format. 
(https://vlsicad.ucsd.edu/UCLAWeb/cheese/ispd98.html)


### The main objective is to show a linear increase in time with the number of cell-net connections
This would be evidence of an improvement from the O(2^n) results of a naive approach.

As a footnote, this is not necessarily how circuits are stored in modern industry. Moreover, it is assumed that there is only one connection between a cell and any particular net (this may not hold for real circuits).
