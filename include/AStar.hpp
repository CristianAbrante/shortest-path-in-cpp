#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <functional>
#include <limits>
#include <set>
#include <vector>
#include <algorithm>

#include "Node.hpp"

using HeuristicFunction = std::function<double(int, int, int, int)>;

const std::vector<HeuristicFunction> heuristicFunctions = {
    [](int x, int y, int endX, int endY){ return 0; }
};


class NodeSet
{
  private:
    std::vector<Node> nodes_;

  private:
    std::vector<Node>::iterator find( const Node& toFind )
    {
        return std::find( nodes_.begin(), nodes_.end(), toFind );        
    }
 
  public:
    NodeSet(): nodes_(){}
    
    bool empty()const
    { 
        return nodes_.empty(); 
    }
    
    bool contains( const Node& toFind )
    {
        return( find(toFind) != nodes_.end() );
    }    
    
    // Returns the node with the minimum value
    Node getLowest()const
    {
        if( !empty() )
            return *(
                std::min_element( nodes_.begin(), nodes_.end() )
            );
    }
    
    // Returns whether the element was inserted or not
    bool insert( const Node& toInsert )
    {
        if( contains(toInsert) )
            return false;
        
        nodes_.push_back( toInsert );
        return true;
    }
    
    void erase( const Node& toErase )
    {
        for( auto n = nodes_.begin();  n != nodes_.end();  ++n )
            if( *n == toErase )
            {
                nodes_.erase(n);
                break;
            }
    }
    
    // Returns whether the element was already in the set or not
    bool insertAndKeepMinimum( const Node& n ) 
    {
        auto oldElement = find( n );
        
        // If it is a new element we insert it and return true
        if( oldElement == nodes_.end() )
        {
            nodes_.push_back( n );
            return true;
        }
        // If it was already in the set we keep the minimum
        if( n < *oldElement )
        {
            nodes_.erase( oldElement );
            nodes_.push_back( n );
        }

        return false;        
    }
    
};


class AStar
{
  private:
    NodeSet openSet_
          , closeSet_
          , obstacles_;
    int h_;  // Position of the heuristic function array
    bool finished_;
    Node startNode_
       , endNode_;
    unsigned N_
           , M_;

  public:
    std::vector<sf::Vector2u> lastAdditionsToOpen;
    sf::Vector2u lastAdditionToClose;

    
  public:
    AStar(
        unsigned N, unsigned M,
        unsigned startX, unsigned startY, 
        unsigned endX, unsigned endY,
        const NodeSet& obstacles,
        unsigned h
    ):
      N_( N ), M_( M ), 
      h_( (h < heuristicFunctions.size()) ? h : 0 ),
      finished_( false ),
      startNode_(
        startX, startY,
        heuristicFunctions[h](startX, startY, endX, endY)
      ),
      endNode_( endX, endY, 0 ),
      obstacles_( obstacles )
    {
        startNode_.updateCost( 0.0 );        
    }
      
    bool nextIteration()
    {
        lastAdditionToClose = {};
        lastAdditionsToOpen.clear();

        // We will add as maximum Node::NEIGHBOURS.size() elements to the vector
        // so lets make sure that we have the space needed.
        lastAdditionsToOpen.reserve( Node::NEIGHBOURS.size() );
        
        if( finished_ ) 
            return true;
        
        // Check if the open set has no elements
        if( openSet_.empty() )
        {
            finished_ = true;
            return true;
        }
        
        // Get node with lowest f value
        auto current = openSet_.getLowest();
        
        // Check if current node is the goal
        if( current == endNode_ )
        {
            // TODO: build shortest path to goal
            finished_ = true;
            return true;
        }
        
        // Erase current node from open set and add it to the close set
        openSet_.erase( current );
        closeSet_.insert( current );
        lastAdditionToClose = current.pos();
        
        // Check current node neighbours
        for( int i = 0; i < Node::NEIGHBOURS.size(); ++i )
        {
            const auto& pos = current.pos();
            int posX = pos.x + Node::NEIGHBOURS[i].x
              , posY = pos.y + Node::NEIGHBOURS[i].y;

            // Checking boundaries. If this is not a valid node we skip it
            if( posX <= 0  ||  posX >= M_  ||  posY <= 0  ||  posY >= N_ ) 
                continue;

            // Construct neighbour node
            auto neighbour = Node( posX, posY, current.g() + 1, &current );

            // Checking obstacles. If this node is an obstacle we skip it
            if( obstacles_.contains(neighbour) )
                continue;
                        
            // If the neighbour is already in the close set we do nothing
            // because it is already optimal
            if( closeSet_.contains(neighbour) )
                continue;
  
            // Try to add neighbour to the open set. If it is already in the
            // open set we update the cost of it to the minimum one.
            // If it was a new node this will return true and we 
            // have to keep track of it in the last additions
            if( openSet_.insertAndKeepMinimum( neighbour ) )
                lastAdditionsToOpen.push_back( neighbour.pos() );
       }
    }
    
};


#endif
