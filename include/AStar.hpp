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

// For what we need, we don't have to have a super efficient algorithm, so we
// implemented the set with a simple vector right now.
class NodeSet
{
  private:
    std::vector<Node*> nodes_;

  private:
    // Returns the position of the element in nodes_. Or -1 if not found
    int find( Node *toFind )const
    {
        if( toFind == nullptr) return -1;

        int pos = -1;
        for( int i = 0;  i < nodes_.size()  &&  pos == -1;  ++i )
            if( nodes_[i] != nullptr  && (*(nodes_[i]) == *toFind) )
                pos = i;

        return pos;
    }
 
  public:
    NodeSet(): nodes_(){}
    NodeSet( const NodeSet& ) = delete;
    NodeSet* operator=( const NodeSet& ) = delete;

    ~NodeSet()
    {
        for( int i = 0;  i < nodes_.size();  ++i )
            if( nodes_[i] )
                delete nodes_[i];
    }
    
    bool empty()const
    { 
        return nodes_.empty(); 
    }
    
    bool contains( Node *toFind )const
    {
        return( find(toFind) != -1 );
    }
    
    // Returns the node with the minimum value
    Node* getLowest()const
    {
        if( empty() )
            return nullptr;

        Node *min = nodes_[0];
        for( int i = 1;  i < nodes_.size();  ++i )
            if( *(nodes_[i]) < *min )
                min = nodes_[i];

        return min;
    }
    
    // Returns whether the element was inserted or not
    bool insert( Node *toInsert )
    {
        if( contains(toInsert) )
            return false;
        
        nodes_.push_back( new Node(*toInsert) );
        return true;
    }

    // This will push_back a pointer Node to nodes_.
    // That pointer points to 'toTake', it does not own it unlike insert().
    bool take( Node *toTake )
    {
        if( contains(toTake) )
            return false;
        
        nodes_.push_back( toTake );
        return true;
    }

    // Does not delete the element. It only removes it from nodes_
    void remove( Node *toErase )
    {
        for( int i = 0;  i < nodes_.size();  ++i )
            if( *(nodes_[i]) == *toErase )
            {
                nodes_.erase( nodes_.begin() + i );
                break;
            }
    }
    
    // Returns whether the element was inserted/updated or not
    bool insertAndKeepMinimum( Node *n ) 
    {
        if( n == nullptr ) 
            throw std::invalid_argument("Cannot insert null");
        
        int oldElementPos = find( n );
        
        // If it is a new element we insert it and return true
        if( oldElementPos == -1 )
        {
            nodes_.push_back( new Node( *n ) );
            return true;
        }
        // If the node was already in the set and the new one
        // is better, we update the node (only cost and pointer to parent
        // is needed to update because they are the same nodes so everything else
        // stays the same)
        if( *n < *(nodes_[oldElementPos]) )
        {
            nodes_[oldElementPos]->updateCost( n->g() )
                                 ->updateParent( n->parent() );
            return true;
        }
        // If the old one remains equal we return false
        return false;
    }
    
};


class AStar
{
  private:
    NodeSet openSet_
          , closeSet_;
    const NodeSet& obstacles_; //<-- TODO: This has to be handle better.
    int h_;  // Position of the heuristic function array
    bool finished_;
    Node *startNode_
       , *endNode_;
    unsigned N_
           , M_;

  public:
    std::vector<sf::Vector2u> lastAdditionsToOpen;
    sf::Vector2u lastAdditionToClose;

  private:
    // TODO: return the path
    void buildShortestPath( Node *goal )
    {
        Node *current = goal;
        while( !( *current == *startNode_ ) )
        {
            std::cerr << '\n' << current->pos().x << ',' << current->pos().y;
            current = current->parent();
        }
        std::cerr << '\n' << startNode_->pos().x << ',' << startNode_->pos().y;
    }

  public:
    AStar(
        unsigned M, unsigned N,
        unsigned startX, unsigned startY, 
        unsigned endX, unsigned endY,
        const NodeSet& obstacles,
        unsigned h
    ):
      M_( M ), N_( N ),
      h_( (h < heuristicFunctions.size()) ? h : 0 ),
      finished_( false ),
      startNode_( nullptr ),
      endNode_( nullptr ),
      obstacles_( obstacles )
    {
        startNode_ = new Node (
            startX, startY,
            heuristicFunctions[h](startX, startY, endX, endY)
        );
        endNode_ = new Node( endX, endY, 0 );

        startNode_->updateCost( 0.0 );
        openSet_.insert( startNode_ );
    }

    ~AStar()
    {
        if( startNode_ ) delete startNode_;
        if( endNode_ ) delete endNode_;
    }
        
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
        Node *current = openSet_.getLowest();
        
        // Info for debugging
        if( debugInfo )
        {
            std::cerr << "Current pos: (" << current->pos().x << ',' << current->pos().y << ")\n";
            std::cerr << "Cost: " << current->g() << ", h: " << current->h() << ", f: " << current->f() << '\n';            
        }
        
        // Check if current node is the goal -> done with solution
        if( *current == *endNode_ )
        {
            // TODO: build shortest path
            buildShortestPath( current );
            finished_ = true;
            return true;
        }

        // Erase current node from open set and add it to the close set
        openSet_.remove( current );
        closeSet_.take( current );
        lastAdditionToClose = current->pos();

        // Check current node neighbours
        for( int i = 0; i < Node::NEIGHBOURS.size(); ++i )
        {
            const auto& pos = current->pos();
            int posX = pos.x + Node::NEIGHBOURS[i].x
              , posY = pos.y + Node::NEIGHBOURS[i].y;

            // Checking boundaries. If this is not a valid node we skip it
            if( posX < 0  ||  posX >= M_  ||  posY < 0  ||  posY >= N_ ) 
                continue;

            // Construct neighbour node
            auto heuristicVal = heuristicFunctions[h_]( posX, posY, endNode_->pos().x, endNode_->pos().y );
            auto neighbour = Node( posX, posY, heuristicVal, current );
            neighbour.updateCost( current->g() + 1 );

            // Checking obstacles. If this node is an obstacle we skip it
            if( obstacles_.contains(&neighbour) )
                continue;
                        
            // If the neighbour is already in the close set we do nothing
            // because it is already optimal
            if( closeSet_.contains(&neighbour) )
                continue;
  
            // Try to add neighbour to the open set. If it is already in the
            // open set we update the cost of it to the minimum one.
            // If the node was inserted this method will return true
            // If the old node remains because is better, this will return false
            if( openSet_.insertAndKeepMinimum( &neighbour ) )
                lastAdditionsToOpen.push_back( neighbour.pos() );
       }
    }
    
};


#endif
