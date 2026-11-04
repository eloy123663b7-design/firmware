#include "SoundsMenu.h"

#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include "core/utils.h"
#include "modules/others/audio.h"
#include "modules/others/audio_player.h"
#if defined(MIC_SPM1423) || defined(MIC_INMP441)
#include "modules/others/mic.h"
#endif

/*********************************************************************
**  Function: optionsMenu
**  Sounds submenu: music player, RTTTL playback, test tone, TTS and mic
**********************************************************************/
void SoundsMenu::optionsMenu() {
    options = {
#if defined(HAS_NS4168_SPKR)
        {"Music Player",    [this]() {
             FS *fs;
             if (!getFsStorage(fs)) return;
             String filepath = loopSD(*fs, true, "wav,mp3,flac,aac,opus,mod,rtttl");
             if (filepath != "") musicPlayerUI(fs, filepath);
         }                                                            },
        {"Play RTTTL",      [this]() { playRtttlMenu(); }             },
        {"Test Tone",       [this]() {
             bool soundEnabled = bruceConfig.soundEnabled;
             bruceConfig.soundEnabled = true;
             _tone(1000, 500);
             bruceConfig.soundEnabled = soundEnabled;
         }                                                            },
        {"Text to Speech",  [this]() {
             String text = keyboard("", 60, "Text to speak");
             if (text != "") tts(text);
         }                                                            },
#endif
#if defined(MIC_SPM1423) || defined(MIC_INMP441)
        {"Mic Spectrum",    mic_test                                  },
        {"Mic Record",      mic_record_app                            },
#endif
    };

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "Sounds");
}

/*********************************************************************
**  Function: playRtttlMenu
**  Prompts for an RTTTL string and plays it back
**********************************************************************/
void SoundsMenu::playRtttlMenu() {
    String song = keyboard(
        "mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6", 200, "RTTTL string"
    );
    if (song == "") return;

    bool soundEnabled = bruceConfig.soundEnabled;
    bruceConfig.soundEnabled = true;
    playAudioRTTTLString(song);
    bruceConfig.soundEnabled = soundEnabled;
}

/*********************************************************************
**  Function: drawIcon
**  Draws an eighth-note (music note) icon
**********************************************************************/
void SoundsMenu::drawIcon(float scale) {
    clearIconArea();

    int headRX = scale * 9;
    int headRY = scale * 6;
    int lineWidth = scale * 4;
    if (lineWidth < 1) lineWidth = 1;

    int headX = iconCenterX - scale * 4;
    int headY = iconCenterY + scale * 14;
    int stemX = headX + headRX - lineWidth / 2;
    int stemTopY = iconCenterY - scale * 20;

    // Note head
    tft.fillEllipse(headX, headY, headRX, headRY, bruceConfig.priColor);

    // Stem
    tft.drawWideLine(
        stemX, stemTopY, stemX, headY, lineWidth, bruceConfig.priColor, bruceConfig.priColor
    );

    // Flag
    tft.fillTriangle(
        stemX,
        stemTopY,
        stemX + scale * 14,
        stemTopY + scale * 10,
        stemX,
        stemTopY + scale * 18,
        bruceConfig.priColor
    );
}
