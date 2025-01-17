#pragma once

#include <memory>
#include <cstdint>
#include <climits>

#include <string>
#include <vector>

void init();
void update(uint32_t time_ms);
void draw();

namespace picosystem {

  typedef uint16_t color_t;

  struct buffer_t {
    uint32_t w, h;
    color_t *data;

    color_t *p(int32_t x, int32_t y) {
      return data + (x + y * w);
    }
  };

  using blend_func_t =
    void(*)(color_t* source, uint32_t source_step,
            color_t*   dest, uint32_t count);

  // drawing state
  extern color_t _pen;                    // pen
  extern int32_t _cx, _cy, _cw, _ch;      // clip rect
  extern int32_t _tx, _ty;                // text cursor position
  extern uint32_t _io, _lio;              // io state and last io state
  extern int32_t _camx, _camy;            // camera
  extern blend_func_t _bf;                // blend function
  extern buffer_t SCREEN;                 // framebuffer
  extern buffer_t &_dt;                   // drawing target
  extern buffer_t *_ss;                   // sprite sheet
  extern uint8_t *_font;
  extern float _fsin_lut[256];            // fast sin lut

  constexpr float _PI = 3.1415927f;

  // state
  void        pen(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
  void        pen(color_t p);
  void        clip(int32_t x, int32_t y, uint32_t w, uint32_t h);
  void        blend_mode(blend_func_t bf);
  void        target(buffer_t &dt);
  void        camera(int32_t camx, int32_t camy);
  void        sprite_sheet(buffer_t &ss);

  // primitives
  void        clear();
  void        pixel(int32_t x, int32_t y);
  void        hspan(int32_t x, int32_t y, int32_t c);
  void        vspan(int32_t x, int32_t y, int32_t c);
  void        rect(int32_t x, int32_t y, int32_t w, int32_t h);
  void        circle(int32_t x, int32_t y, int32_t r);
  void        line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
  void        blit(
                const buffer_t &source,
                int32_t x, int32_t y, int32_t w, int32_t h,
                int32_t dx, int32_t dy);
  void        sprite(uint32_t i, int32_t x, int32_t y, uint8_t flags = 0);
  void        text(const char &c, int32_t x, int32_t y);
  void        text(const char &c);
  void        text(const std::string &t, int32_t x, int32_t y);
  void        text(const std::string &t);

  // blend functions
  void        COPY(
                color_t* source, uint32_t source_step,
                color_t* dest, uint32_t count);
  void        BLEND(
                color_t* source, uint32_t source_step,
                color_t* dest, uint32_t count);
  void        MASK(
                color_t* source, uint32_t source_step,
                color_t* dest, uint32_t count);

  // utility
  std::string str(float v, uint8_t precision = 2);
  std::string str(uint32_t v);
  color_t     color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 15);
  color_t     hsv_color(float h, float s, float v, float a = 1.0f);
  uint32_t    time();
  uint32_t    time_us();
  void        sleep(uint32_t d);
  void        sleep_us(uint32_t d);
  bool        intersects(
                int32_t  x, int32_t  y, int32_t  w, int32_t  h,
                int32_t cx, int32_t cy, int32_t cw, int32_t ch);
  void        intersection(
                int32_t &x, int32_t &y, int32_t &w, int32_t &h,
                int32_t cx, int32_t cy, int32_t cw, int32_t ch);
  bool        contains(
                int32_t  x, int32_t  y,
                int32_t cx, int32_t cy, int32_t cw, int32_t ch);
  void        wrap(std::string &t, std::size_t chars);
  std::vector<std::string> split(const std::string& t, char d = '\n');
  float       fsin(float v);
  float       fcos(float v);

  // hardware
  bool        pressed(uint32_t b);
  bool        button(uint32_t b);
  uint32_t    battery();
  void        led(uint8_t r, uint8_t g, uint8_t b);
  void        backlight(uint8_t b);

  // internal methods - do not call directly, will change!
  void       _logo();
  void       _init_hardware();
  uint32_t   _gpio_get();
  float      _battery_voltage();
  void       _reset_to_dfu();
  void       _wait_vsync();
  void       _flip();
  bool       _is_flipping();
  void       _camera_offset(int32_t &x, int32_t &y);

  // input pins
  enum button {
    UP    = 23,
    DOWN  = 20,
    LEFT  = 22,
    RIGHT = 21,
    A     = 18,
    B     = 19,
    X     = 17,
    Y     = 16
  };

  // assets
  extern uint8_t _default_font[128][8];
  extern uint8_t _picosystem_logo[550];
  extern color_t _default_sprite_sheet[16384];

}