#ifndef NODE_HPP
#define NODE_HPP

using Matrix2i = std::vector<sf::Vector2i>;

class Node
{
  private:
    unsigned x_, y_; // The position in grid define the state
    Node *parent_; // To know where this node comes from so we can know the shortest path at the end
    double costFromStart_; // G function
    double heuristicValue_; // Value of Heuristic function for this node

  public:
    static const Matrix2i NEIGHBOURS;

  public:
    Node( unsigned x, unsigned y, double heuristicValue = std::numeric_limits<double>::infinity(), Node *parent = nullptr):
      x_( x ),
      y_( y ),
      heuristicValue_( heuristicValue ),
      parent_( parent ),
      costFromStart_( std::numeric_limits<double>::infinity() )
    {}
      
    Node( sf::Vector2u pos, double heuristicValue = std::numeric_limits<double>::infinity(), Node *parent = nullptr ):
      Node( pos.x, pos.y, heuristicValue, parent ) // Leave the construction to the above constructor
    {}
    
    double f()const{ return costFromStart_ + heuristicValue_; }
    double g()const{ return costFromStart_; }
    double h()const{ return heuristicValue_; }
    
    const Node *const parent()const{ return parent_; }
    Node *updateParent( Node *parent ){ parent_ = parent; return this; }
    Node *updateCost( int newCost ){ if( newCost >= 0 )costFromStart_ = newCost; return this; }
    
    bool operator==( const Node& that )const{ return( x_ == that.x_  &&  y_ == that.y_ ); }
    bool operator<( const Node& that )const{ return( f() < that.f() ); }
    
    sf::Vector2u pos()const{ return {x_, y_}; }
};

// Positions of the neighbour relative to a cell
const Matrix2i Node::NEIGHBOURS = { {0, -1}, {-1, 0}, {+1, 0}, {0, +1} };


#endif