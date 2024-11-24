# Maze Router using Lee's Algorithm 

## Introduction
This project aims to develop a maze router—a type of routing algorithm designed to establish paths between multiple points on a grid-based layout while navigating obstacles and minimizing routing costs. This maze router is specifically tailored for a two-layer routing environment, where it will manage multi-point connection (nets) and optimize routes across two layers, M0 and M1, which have distinct directional preferences: M0 is optimized for horizontal routing, and M1 for vertical.

The routing area is organized as an NxM grid of uniformly sized cells, each with an associated cost. Deviating from the preferred directions on each layer incurs a bend penalty, while transitions between layers (vias) come with an additional via penalty. This router will intelligently account for these penalties, generating optimized paths for each net. As a result, it serves as a foundational tool for IC layout design, addressing both physical constraints and cost considerations integral to the routing proces. 

## Lee's Algorithm 
Lee's Algorithm is a breadth-first search algorithm used for maze routing. It works by systematically exploring all possible paths from the start point until it reaches the end point, ensuring the shortest path is found. The algorithm fills the maze grid with distance values from the start point and then backtracks from the end point to construct the path.

We chose to implement this algorithm specifically because: 
* It is easy to implement. 
* It always finds the shortest path.
* We are well versed in how it works.

## Implementation
### Reading the Input 
To begin we read the file line by line so that we can:
1. Store the dimensions and the penalties for the grid.
2. If the line is an obstruction, we validated its coordinated and add it to the grid.
3. If the line is a net, we validate and save each coordinate in a vector which is then saved in a list of nets

### Routing
The following steps are repeated per net
#### Mapping Net to Grid
- loop over the coordinates of the net
- The first coordinate gets set as the source on the grid
- Any other coordinate is set as a terget

#### Filling
- We begin with a vector of our sources
- For each source we fill the surrounding cells accordingly
- If an adjacent cell is found as a target we signal a hit and move on
#### Backpropagation 
- Takes the coordinate of the hit tagert
- It will observe the surroundong cells and choose the next cell as the one with the lowest cost
- If the cell is marked as a Via we automatically switch layers
- These steps are repeated until we find a source

#### Route
- This is the top level module that brings together the functions above
- After backpropogation we display the route and mark all these cells as sources so that we can route for the next target
- When we come to route a new netlist we mark all the coordiates from the previous routing as obstructions

## Contributions
@useframi1 - Inititalization and File Handling

@mariam-elg & @NadSafwat - Filling and Backpropagation

@NKasaby - Test case generation and validation
