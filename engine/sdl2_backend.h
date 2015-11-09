#pragma once

#include "virtual_terminal.h"
#include "sdl2_resources.h"
#include <SDL2/SDL.h>
#include <string>

using std::pair;

namespace engine {

class sdl2_backend {
public:
  sdl2_backend();
  ~sdl2_backend();
  void init(const string& window_title, const int width, const int height);
  void stop();
  pair<int,int> get_console_size();
  void draw_vterm(vector<vterm::screen_character>* screen);
  void poll_inputs();
  
  void clear_screen();
  void present();
  pair<int,int> get_screen_size();
  void render_bitmap(const std::string &tag, const SDL_Rect &source, const SDL_Rect &dest);
  void render_bitmap_simple(const std::string &tag, const int &x, const int &y);
  void render_bitmap_centered(const std::string &tag, const int &x, const int &y);
  
  int load_image_resource(const std::string &filename, const std::string &tag);
  int load_font_resource(const std::string &filename, const std::string &tag, const int &size);
  int render_text_to_image(const string& font_tag, const string text, const string& new_tag, SDL_Color color);
private:
  bool initialized;
  SDL_Window * window;
  SDL_Renderer * renderer;
  
  sdl_resource_manager resources;  
};
  
}