#ifndef displayST7735_h
#define displayST7735_h
#include "../core/options.h"

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#if CLOCKFONT_MONO
  #include "fonts/DS_DIGI28pt7b_mono.h"                          // https://tchapi.github.io/Adafruit-GFX-Font-Customiser/
#else
  #include "fonts/DS_DIGI28pt7b.h"
#endif
#include "tools/l10n.h"

#define CHARWIDTH   6
#define CHARHEIGHT  8

typedef GFXcanvas16 Canvas;
#include "widgets/widgets.h"
#include "widgets/pages.h"


#if DTYPE==INITR_MINI160x80
  #if __has_include("conf/displayST7735_miniconf_custom.h")
    #include "conf/displayST7735_miniconf_custom.h"
  #else
    #include "conf/displayST7735_miniconf.h"
  #endif
#elif DTYPE==INITR_144GREENTAB
  #if __has_include("conf/displayST7735_144conf_custom.h")
    #include "conf/displayST7735_144conf_custom.h"
  #else
    #include "conf/displayST7735_144conf.h"
  #endif
#else
  #if __has_include("conf/displayST7735_blackconf_custom.h")
    #include "conf/displayST7735_blackconf_custom.h"
  #else
    #include "conf/displayST7735_blackconf.h"
  #endif
#endif


#define BOOT_PRG_COLOR    0xE68B
#define BOOT_TXT_COLOR    0xFFFF
#define PINK              0xF97F

class DspCore: public Adafruit_ST7735 {
#include "tools/commongfx.h"
};

extern DspCore dsp;

#endif
