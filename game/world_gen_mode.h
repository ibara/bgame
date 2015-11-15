#pragma once

#include "../engine/engine.h"
#include <memory>
#include <thread>

using std::make_pair;
using engine::vterm::color_t;

class world_gen_mode : public engine::base_mode {
public:
     virtual void init() override;
     virtual void done() override;
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) override;
private:
     engine::gui ui;
     std::unique_ptr<std::thread> world_gen_thead;

     void render_init();
     void render_faultlines();
     void render_done();
     void render_tiles();
};
