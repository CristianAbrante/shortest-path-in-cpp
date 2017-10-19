#include "GridCamera.hpp"

GridCamera::GridCamera(const GraphicGrid& grid):
    view_({
        (float)grid.startPos().x,
        (float)grid.startPos().y,
        (float)grid.endPos().x,
        (float)grid.endPos().y
    }),
    defaultView_( view_ ),
    offset_({
        grid.numRows() / 100.0f,
        grid.numCols() / 100.0f
    })
{}

GridCamera* GridCamera::zoom( sf::Vector2f offsetFactor )
{
    offsetFactor.x *= offset_.x;
    offsetFactor.y *= offset_.y;

    offsetFactor += view_.getSize();

    
    if( offsetFactor.x > 1.0f  &&  offsetFactor.y > 1.0f )
    {
        view_.setSize( offsetFactor.x, offsetFactor.y );
    }

    return this;
}

GridCamera* GridCamera::move( const sf::Vector2f& offsetFactor )
{
    view_.move(
        offset_.x * offsetFactor.x,
        offset_.y * offsetFactor.y
    );

    return this;
}

GridCamera* GridCamera::resetCamera()
{
    view_ = defaultView_;
    return this;
}
