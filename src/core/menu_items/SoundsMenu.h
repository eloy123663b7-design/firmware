#ifndef __SOUNDS_MENU_H__
#define __SOUNDS_MENU_H__

#include <MenuItemInterface.h>

class SoundsMenu : public MenuItemInterface {
public:
    SoundsMenu() : MenuItemInterface("Sounds") {}

    void optionsMenu(void);
    void drawIcon(float scale);
    bool hasTheme() { return bruceConfig.theme.sounds; }
    const String& themePath() override { return bruceConfig.theme.paths.sounds; }

private:
    void playRtttlMenu(void);
};

#endif
