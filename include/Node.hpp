#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

using Matrix2i = std::vector<sf::Vector2i>;

class Node
{
  private:
    unsigned x_, y_; // The position in grid define the state
    
  public:
    static const Matrix2i NEIGHBOURS;

  public:
    Node( unsigned x, unsigned y ):
      x_( x ),
      y_( y )
    {}
      
    Node( sf::Vector2u pos ):
      Node( pos.x, pos.y ) // Leave the construction to the above constructor
    {}
    
    bool operator==( const Node& that )const{ return( x_ == that.x_  &&  y_ == that.y_ ); }
    
    sf::Vector2u pos()const{ return {x_, y_}; }
};

// Positions of the neighbour relative to a cell
const Matrix2i Node::NEIGHBOURS = { {0, -1}, {-1, 0}, {+1, 0}, {0, +1} };


class Path
{
  private:
    std::vector<Node> path_;
    double g_, h_; // Cost and heuristic values
    
  public:
      Path(): path_(), g_(0), h_(0) {}
      Path( const Node& n ): path_(), g_(0), h_(0) { update(n, 0, 0); }
      
      Path* update( const Node& n, double costToAdd, double newHeuristicVal )
      {
          path_.push_back( n );
          g_ += costToAdd;
          h_ = newHeuristicVal;
          
          return this;
      }
      
      double f()const{ return g_ + h_; }
      double g()const{ return g_; }
      double h()const{ return h_; }
      
      bool empty()const{ return path_.empty(); }
      
      bool operator==( const Path& that )const{ return path_.back() == that.path_.back(); }
      bool operator<( const Path& that )const{ return f() < that.f(); }
      
      sf::Vector2u pos()const{ return path_.back().pos(); }

      std::vector<sf::Vector2u> getPath()const
      {
          std::vector<sf::Vector2u> result;

          for( const auto& node : path_ )
              result.push_back( node.pos() );
          
          return result;
      }
};

class PathSet
{
  private:
    std::vector<Path> paths_;

  private:
    // Returns the position of the element in nodes_. Or -1 if not found
    int find( const Path& toFind )const
    {
        int pos = -1;
        for( int i = 0;  i < paths_.size()  &&  pos == -1;  ++i )
            if( paths_[i] == toFind )
                pos = i;

        return pos;
    }
 
  public:
    PathSet(): paths_(){}

    bool empty()const
    { 
        return paths_.empty(); 
    }
    
    bool contains( const Path& toFind )const
    {
        return( find(toFind) != -1 );
    }
    
    // Returns the node with the minimum value
    Path getLowest()const
    {
        if( empty() )
            throw;

        Path min = paths_[0];
        for( int i = 1;  i < paths_.size();  ++i )
            if( paths_[i] < min )
                min = paths_[i];

        return min;
    }
    
    Path get( const Path& toGet )const
    {
        int index = find( toGet );

        if( index == -1 )
            return Path();
        else
            return paths_[index];
    }
    
    // Returns whether the element was inserted or not
    bool insert( const Path& toInsert )
    {
        if( contains(toInsert) )
            return false;
        
        paths_.push_back( toInsert );
        return true;
    }

    void remove( const Path& toErase )
    {
        for( int i = 0;  i < paths_.size();  ++i )
            if( paths_[i] == toErase )
            {
                paths_.erase( paths_.begin() + i );
                break;
            }
    }
    
    // Returns whether the element was inserted/updated or not
    bool insertAndKeepMinimum( const Path& p ) 
    {
        int oldElementPos = find( p );
        
        // If it is a new element we insert it and return true
        if( oldElementPos == -1 )
        {
            paths_.push_back( p );
            return true;
        }
        // If the path was already in the set and the new one
        // is better, we update the path 
        if( p < paths_[oldElementPos] )
        {
            paths_[oldElementPos] = p;
            return true;
        }
        // If the old one remains equal we return false
        return false;
    }
    
};

#endif