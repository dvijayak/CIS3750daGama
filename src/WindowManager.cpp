#include "WindowManager.hpp"

WindowManager::WindowManager (std::string title, int width, int height, Uint32 flags)
{
  trclog("Initializing window manager...");
    
  m_pWindow = SDL_CreateWindow(title.c_str(), 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    flags
    );

  trclog("Window manager initialized.");
}

WindowManager::~WindowManager ()
{
  trclog("Destroying window manager...");
  
  SDL_DestroyWindow(m_pWindow);

  trclog("Window manager destroyed.");  
}

SDL_Window* WindowManager::GetWindow () const
{
  return m_pWindow;
}

std::string WindowManager::GetTitle () const
{
  return std::string(SDL_GetWindowTitle(m_pWindow));
}

int WindowManager::GetWidth () const 
{
  int w = 0;
  SDL_GetWindowSize(m_pWindow, &w, 0);
  return w;
}

int WindowManager::GetHeight () const 
{
  int h = 0;
  SDL_GetWindowSize(m_pWindow, 0, &h);
  return h;
}
