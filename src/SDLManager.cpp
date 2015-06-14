#include "SDLManager.hpp"

#include "WindowManager.hpp"
#include "RenderManager.hpp"
#include "NetworkManager.hpp"

SDLManager* SDLManager::s_pInstance = 0;

SDLManager* SDLManager::Instance (const std::string& title, Uint32 flags)
{
  if (s_pInstance)
  {
    return s_pInstance;
  }

  s_pInstance = new SDLManager(title, flags);

  return s_pInstance;
}

SDLManager::SDLManager (const std::string& title, Uint32 flags)
{
  trclog("Initializing SDL subsystems...");

  m_title = title;

  SDL_Init(flags);

  trclog("SDL subsystems initialized.");
}

SDLManager::~SDLManager ()
{
  trclog("Destroying all managers...");

  delete m_pWindowManager;
  delete m_pRenderManager;
  delete m_pNetworkManager;

  trclog("All managers destroyed.");

  trclog("Shutting down SDL subsystems...");

  SDL_Quit();

  trclog("SDL subsystems shut down.");
}


int SDLManager::InitializeManagers ()
{
  trclog("Initializing all managers...");

  // m_pWindowManager = new WindowManager(m_title, MAX_WIDTH, MAX_HEIGHT, SDL_WINDOW_RESIZABLE);
  // if ( !m_pWindowManager->GetWindow() )
  // {
  //   errlog("Could not create window:" << std::endl << SDL_GetError());
  //   return 1;
  // }
  // m_pRenderManager = new RenderManager(*m_pWindowManager, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  

  m_pNetworkManager = new NetworkManager();
  if (m_pNetworkManager->Failed())
  {
    errlog("Could not create network manager:" << std::endl << SDLNet_GetError());
    return 1;
  }

  trclog("All managers initialized. Locked and loaded.");
  return 0;
}
