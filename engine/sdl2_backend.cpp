#include "sdl2_backend.h"
#include <SDL2/SDL_image.h>
#include "command_manager.h"

using std::make_pair;

namespace engine {

// TODO: Configuration driven height
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

sdl2_backend::sdl2_backend()
{
     initialized = false;
}

sdl2_backend::~sdl2_backend()
{
     stop();
}

void sdl2_backend::init()
{
     const int error_code = SDL_Init ( SDL_INIT_VIDEO );
     if ( error_code < 0 ) throw 101; // TODO: Real exception

     // TODO: Configuration driven window title
     window = SDL_CreateWindow ( "Black Future",
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                               );
     if ( window == NULL ) throw 102;

     renderer = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
     if ( renderer == NULL ) throw 103;

     int image_flags = IMG_INIT_PNG;
     if ( ! ( IMG_Init ( image_flags ) & image_flags ) ) throw 104;

     SDL_Surface * font_surface = IMG_Load ( "../assets/terminal8x8_palette2.png" );
     if ( font_surface == NULL ) throw 104;
     font_image = SDL_CreateTextureFromSurface ( renderer, font_surface );
     SDL_FreeSurface ( font_surface );


     initialized = true;
}

void sdl2_backend::stop()
{
     if ( initialized ) {
          SDL_DestroyTexture ( font_image );
          SDL_DestroyRenderer ( renderer );
          SDL_DestroyWindow ( window );
          SDL_Quit();
     }
}

pair< int, int > sdl2_backend::get_console_size()
{
     return make_pair ( SCREEN_WIDTH/8,SCREEN_HEIGHT/8 );
}

void sdl2_backend::draw ( vector< vterm::screen_character >* screen )
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  const int ascii_height = SCREEN_HEIGHT/8;
  const int ascii_width = SCREEN_WIDTH/8;
  const SDL_Rect background_source{88, 104, 8, 8};
  
  for (int y=0; y<ascii_height; ++y) {
    for (int x=0; x<ascii_width; ++x) {
      const int screen_x = x * 8;
      const int screen_y = y * 8;
      const unsigned char target_char = screen->operator[]((y*ascii_width)+x).character;
      const tuple<unsigned char, unsigned char, unsigned char> foreground = screen->operator[]((y*ascii_width)+x).foreground_color;
      const tuple<unsigned char, unsigned char, unsigned char> background = screen->operator[]((y*ascii_width)+x).background_color;
      const int texture_x = (target_char % 16) * 8;
      const int texture_y = (target_char / 16) * 8;

      // Where it goes
      SDL_Rect dst_rect{screen_x, screen_y, 8, 8};
      
      // Blit the background
      SDL_SetTextureColorMod(font_image, std::get<0>(background), std::get<1>(background), std::get<2>(background));
      SDL_RenderCopy(renderer, font_image, &background_source, &dst_rect);
      
      // Blit the foreground
      SDL_SetTextureColorMod(font_image, std::get<0>(foreground), std::get<1>(foreground), std::get<2>(foreground));
      SDL_Rect src_rect{texture_x, texture_y, 8, 8};
      SDL_RenderCopy(renderer, font_image, &src_rect, &dst_rect);
    }
  }
  
  SDL_RenderPresent(renderer);
}

command::keys translate_keycode(const SDL_Event &e) {
  switch (e.key.keysym.sym) {
    case SDLK_UP : return command::UP;
    case SDLK_DOWN : return command::DOWN;
    case SDLK_LEFT : return command::LEFT;
    case SDLK_RIGHT : return command::RIGHT;
    case SDLK_RETURN : return command::ENTER;
    case SDLK_q : return command::Q;
  }
  return command::NONE;
}

void sdl2_backend::poll_inputs()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_KEYDOWN) {
	command::on_command({ translate_keycode(e), 0, 0, command::KEYDOWN });
      }
    }
}


}