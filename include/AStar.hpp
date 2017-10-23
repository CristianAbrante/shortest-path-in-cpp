#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <functional>
#include <limits>
#include <set>
#include <vector>
#include <algorithm>

#include "Node.hpp"

using HeuristicFunction = std::function<double(int, int, int, int)>;
std::vector<HeuristicFunction> heuristicFunctions = {
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
    
    // Returns whether the element was inserted or not
    bool insertAndKeepMinimum( const Node& n ) 
    {
        auto oldElement = find( n );
        
        // If it is a new element we insert it
        if( oldElement == nodes_.end() )
        {
            nodes_.push_back(n);
            return true;
        }
        // Else we keep the minimum
        else if( n < *oldElement )
        {
            nodes_.erase( oldElement );
            nodes_.push_back( n );
            return true;
        }

        return false;        
    }
    
};


class AStar
{
  private:
    std::set<Node> openSet_
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
        const std::set<Node>& obstacles,
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
        auto current = *openSet_.begin();
        
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
            auto pos = current.pos();
            pos.x += Node::NEIGHBOURS[i].x;
            pos.y += Node::NEIGHBOURS[i].y;

            // Checking boundaries. If this is not a valid node we skip it
            if( pos.x <= 0  ||  pos.x >= M_  ||  pos.y <= 0  ||  pos.y >= N_ ) 
                continue;

            // Construct neighbour node
            auto neighbour = Node( pos, current.g() + 1, &current );

            // Checking obstacles. If this node is an obstacle we skip it
            if( obstacles_.count(neighbour) == 1 )
                continue;
                        
            // If the neighbour is already in the close set we do nothing
            // because it is already optimal
            if( closeSet_.count(neighbour) == 1 )
                continue;
            
            // If the neighbour is not in the open set we add it
            auto insertResult = openSet_.insert( neighbour );
            
            if( insertResult.second == true ) // True if the node was added to the set
            {
                lastAdditionsToOpen.push_back( neighbour.pos() );
            }
            // If the neighbour is already in the open set we take it
            // to compare it with the new node to see which one is better
            else
            {
                // Take the node that was in the set
                Node oldNode = *insertResult.first;
                
                // Compare the costs to see which one is lower
                // No need to add the heuristic because the nodes are the same
                // so the have the same heuristic value
                // If the new node is the best then we have to swap it
                // else we leave the set the same
                if( neighbour.g() < oldNode.g() )
                {
                    openSet_.erase( insertResult.first );
                    openSet_.insert( neighbour );
                    lastAdditionsToOpen.push_back( neighbour.pos() );
                }
            }
       }
    }
    
};


#endif
