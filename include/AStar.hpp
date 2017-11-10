#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <functional>
#include <limits>
#include <vector>
#include <algorithm>

#include "Node.hpp"

using HeuristicFunction = std::function<double(int, int, int, int)>;

const std::vector<HeuristicFunction> heuristicFunctions = {
    [](int x, int y, int endX, int endY){ return 0; },
    [](int x, int y, int endX, int endY){ return ( std::max( std::abs(endX - x), std::abs(endY - y) ) ) ; },
    [](int x, int y, int endX, int endY){ return ( std::abs(endX - x) + std::abs(endY - y) ); },
    [](int x, int y, int endX, int endY){ return std::sqrt( std::pow(endX - x, 2) + std::pow(endY - y, 2) ); }    
};


class AStar
{
  private:
    PathSet openSet_
          , closeSet_;
    std::vector<bool> obstacles_;
    int h_;  // Position of the heuristic function array
    bool finished_;
    Path startNode_
       , endNode_;
    unsigned N_
           , M_;
    std::vector<sf::Vector2u> shortestPath_;

  public:
    std::vector<sf::Vector2u> lastAdditionsToOpen;
    sf::Vector2u lastAdditionToClose;

  public:
    AStar(
        unsigned M, unsigned N,
        unsigned startX, unsigned startY, 
        unsigned endX, unsigned endY,
        const std::vector<bool>& obstacles,
        unsigned h
    ):
      M_( M ), N_( N ),
      h_( (h < heuristicFunctions.size()) ? h : 0 ),
      finished_( false ),
      startNode_(),
      endNode_(),
      obstacles_( obstacles )
    {
        startNode_.update(
            {startX, startY},
            0,
            heuristicFunctions[h](startX, startY, endX, endY)
        );
        endNode_.update(
            {endX, endY},
            0,
            0            
        );

        openSet_.insert( startNode_ );
    }
      
    const std::vector<sf::Vector2u>& getShortestPath()const{ return shortestPath_; }

    bool nextIteration( bool debugInfo = true )
    {
        lastAdditionToClose = {};
        lastAdditionsToOpen.clear();

        // We will add as maximum Node::NEIGHBOURS.size() elements to the vector
        // so lets make sure that we have the space needed.
        lastAdditionsToOpen.reserve( Node::NEIGHBOURS.size() );
        
        // If for some reason this method is called when the algorithm is already done
        if( finished_ )
            return true;
        
        // Check if the open set has no elements -> no solution
        if( openSet_.empty() )
        {
            finished_ = true;
            return true;
        }
        
        // Get node with lowest f value
        Path current = openSet_.getLowest();
        
        // Info for debugging
        if( debugInfo )
        {
            std::cerr << "Current pos: (" << current.pos().x << ',' << current.pos().y << ")\n";
            std::cerr << "Cost: " << current.g() << ", h: " << current.h() << ", f: " << current.f() << '\n';            
        }
        
        // Check if current node is the goal -> finished with solution
        if( current == endNode_ )
        {
            // Build shortest path and finish
            shortestPath_ = current.getPath();
            finished_ = true;
            return true;
        }

        // Erase current node from open set and add it to the close set
        openSet_.remove( current );
        closeSet_.insertAndKeepMinimum( current );
        lastAdditionToClose = current.pos();

        // Check current node neighbours
        for( int i = 0; i < Node::NEIGHBOURS.size(); ++i )
        {
            const auto& pos = current.pos();
            int posX = pos.x + Node::NEIGHBOURS[i].x
              , posY = pos.y + Node::NEIGHBOURS[i].y;

            // Checking boundaries. If this is not a valid node we skip it
            if( posX < 0  ||  posX >= M_  ||  posY < 0  ||  posY >= N_ ) 
                continue;

            // Construct new path
            double heuristicVal = heuristicFunctions[h_]( posX, posY, endNode_.pos().x, endNode_.pos().y );
            Path newPath = current;
            newPath.update(
                {(unsigned)posX, (unsigned)posY},
                1, heuristicVal 
            );

            // Checking obstacles. If this node is an obstacle we skip it            
            if( obstacles_[ posX * N_ + posY ] )
                continue;
                        
            // If the new path is already in the close set and
            // the one there is better, we do nothing with this path
            Path pathInCloseSet = closeSet_.get(newPath);
            if( !pathInCloseSet.empty()  &&  pathInCloseSet < newPath )
                continue;
  
            // Try to add the new path to the open set. If it is already in the
            // open set we update the cost of it to the minimum one.
            // If the node was inserted this method will return true
            // If the old node remains because is better, this will return false
            if( openSet_.insertAndKeepMinimum( newPath ) )
                lastAdditionsToOpen.push_back( newPath.pos() );
       }
    }
    
};


#endif
