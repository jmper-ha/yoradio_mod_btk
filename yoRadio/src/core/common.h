#ifndef COMMON_H
#define COMMON_H

enum displayMode_e {PLAYER, 
                    VOL, 
                    STATIONS, 
                    NUMBERS, 
                    LOST, 
                    UPDATING, 
                    INFO, 
                    SETTINGS, 
                    TIMEZONE, 
                    WIFI, 
                    CLEAR, 
                    SLEEPING, 
                    SDCHANGE, 
                    SCREENSAVER, 
                    SCREENBLANK,
                    QR };
                    
enum pages_e      : uint8_t  { PG_PLAYER=0, PG_DIALOG=1, PG_PLAYLIST=2, PG_SCREENSAVER=3 };

enum displayRequestType_e { BOOTSTRING, 
                            NEWMODE, 
                            CLOCK, 
                            NEWTITLE, 
                            NEWSTATION, 
                            NEXTSTATION, 
                            DRAWPLAYLIST, 
                            DRAWVOL, 
                            DBITRATE, 
                            AUDIOINFO, 
                            SHOWVUMETER, 
                            DSPRSSI, 
                            SHOWWEATHER, 
                            NEWWEATHER, 
                            PSTOP, 
                            PSTART, 
                            DSP_START, 
                            WAITFORSD, 
                            SDFILEINDEX, 
                            NEWIP, 
                            NOPE,
                            SHOWFAVWARN,
                            SHOWBT,
                            BATTERY,
                            CHARGE };
struct requestParams_t
{
  displayRequestType_e type;
  int payload;
};

enum controlEvt_e { EVT_NONE=255, 
                    EVT_BTNLEFT=0, 
                    EVT_BTNCENTER, 
                    EVT_BTNRIGHT, 
                    EVT_ENCBTNB, 
                    EVT_BTNUP, 
                    EVT_BTNDOWN, 
                    EVT_ENC2BTNB, 
                    EVT_BTNMODE,
                    EVT_QR,
                    EVT_F1,
                    EVT_F2,
                    EVT_F3,
                    EVT_F4,
                    EVT_F5,
                    EVT_F6,
                    };



#endif
