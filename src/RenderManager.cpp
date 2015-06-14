#include "RenderManager.hpp"

#include "WindowManager.hpp"

// Static definitions
// Matrix3F RenderManager::s_TransformScreenSpace = Matrix3F();

RenderManager::RenderManager (const WindowManager& windowManager, Uint32 flags)
{
  trclog("Initializing render manager");

  m_WIDTH = windowManager.GetWidth();
  m_HEIGHT = windowManager.GetHeight();
    
  // Renderer
  m_pRenderer = SDL_CreateRenderer(windowManager.GetWindow(), -1, flags);

  // Main Texture
  m_pTexture = SDL_CreateTexture(m_pRenderer, 
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STATIC,
    m_WIDTH, m_HEIGHT
    );

  // Raw buffer
  m_pPixels = new Uint32[m_WIDTH * m_HEIGHT];

  // Transforms
  // Matrix3F::values_type values = {

  // };

  trclog("Render manager initialized.");
}

RenderManager::~RenderManager ()
{
  trclog("Destroying render manager...");
  
  delete[] m_pPixels;
  SDL_DestroyTexture(m_pTexture);
  SDL_DestroyRenderer(m_pRenderer);

  trclog("Render manager destroyed.");  
}

SDL_Renderer* RenderManager::GetRenderer () const
{
  return m_pRenderer;
}

SDL_Texture* RenderManager::GetTexture () const
{
  return m_pTexture;
}

void RenderManager::Render () const
{
	SDL_UpdateTexture(m_pTexture, 0, m_pPixels, m_WIDTH * sizeof(Uint32));

	SDL_RenderClear(m_pRenderer);
	SDL_RenderCopy(m_pRenderer, m_pTexture, 0, 0);
	SDL_RenderPresent(m_pRenderer);
}

void RenderManager::RenderLine (float x_s, float y_s, float x_e, float y_e, Uint32 color)
{
  // Handle potential negative slopes
  // (Of course, we assume that all coordinates in screen space are non-negative)
  if (x_s > x_e || y_s < y_e)
  {
    // std::swap(x_s, x_e);
    // std::swap(y_s, y_e);
  }
  // if (y_s > y_e)
  // {
  // }
  
  // Transform to screen space
  // Point3F P_s = s_TransformScreenSpace * line.start;  
  // Point3F P_e = s_TransformScreenSpace * line.end;
  //
  // For now, just multiply by width and height
  x_s *= m_WIDTH;
  y_s *= m_HEIGHT;
  x_e *= m_WIDTH;
  y_e *= m_HEIGHT;

  console("Start = (" << x_s << ", " << y_s << ")" << "\t" << "End = (" << x_e << ", " << y_e << ")");

  // Draw the start and end points first
  SetPixel( int( std::round(x_s) ), int( std::round(y_s) ), color );
  SetPixel( int( std::round(x_e) ), int( std::round(y_e) ), color );

  // If the start and end are the same, there's nothing else to be done
  if (x_s == x_e && y_s == y_e)
  {
    return;
  }

  // Approximate the line segment and render each discrete point on the line
  float dx = x_e - x_s;
  float dy = y_e - y_s;

  console("dx = " << dx << "\t" << "dy = " << dy);

  // Corner case 1: dy = 0, so dx/dy = n.d
  // Simply fill up every single pixel along the x-axis
  if (dy == 0)
  {
    int y = int(y_s);
    if (dx < 0)
    {
      for (int x = int(x_s); x > x_e; --x)
      {
        SetPixel(x, y, color);
      }
    }
    else
    {
      for (int x = int(x_s); x < x_e; ++x)
      {
        SetPixel(x, y, color);
      }
    }
  }
  // Corner case 2: dx = 0, so dy/dx = n.d
  // Simply fill up every single pixel along the y-axis
  else if (dx == 0)
  {
    int x = int(x_s);
    if (dy < 0)
    {
    	for (int y = int(y_s); y > y_e; --y)
	    {
	      SetPixel(x, y, color);
	    }
    }
    else
    {
	    for (int y = int(y_s); y < y_e; ++y)
	    {
	      SetPixel(x, y, color);
	    }
    }
  }
  // Corner case 3: dx = dy, so dy/dx = 1
  // Rarely occurs, but why not handle it?
  else if (dx == dy)
  {
  	for (int x = int( std::round(x_s) ), y = int( std::round(y_s) ); x < x_e && y < y_e; ++x, ++y)
    {
      SetPixel(x, y, color);
    }
  }
  else
  {
  	// dx > dy, so dy/dx < 1
  	// Approximate y component
  	if ( std::abs(dx) > std::abs(dy) )
  	{
  		float m = dy/std::abs(dx);
      console("m = " << m);
      float y = y_s;
      for (int x = int(x_s); x < x_e; ++x, y += m)
      {
        SetPixel(x, int( std::round(y) ), color);
      }
    }
    // dx < dy, so dy/dx > 1
    // Approximate x component
    else
    {
      float m = dx/std::abs(dy);
      console("m = " << m);
      float x = x_s;
  		for (int y = int(y_s); y < y_e; ++y, x += m)
  		{
  			SetPixel( int( std::round(x) ), y, color);
  		}
  	}
  }

}

void RenderManager::RenderPolygon (const Polygon3F& poly, Uint32 color)
{
  // Render each edge of the polygon
  for (int i = 0; i < poly.nVert; ++i)
  {
    // RenderLine(poly.edges[i], color);
  }
}
