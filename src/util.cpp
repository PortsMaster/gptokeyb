/* Copyright (c) 2021-2023
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
#
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
#
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the
* Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301 USA
#
* Authored by: Kris Henriksen <krishenriksen.work@gmail.com>
#
* AnberPorts-Keyboard-Mouse
* 
* Part of the code is from from https://github.com/krishenriksen/AnberPorts/blob/master/AnberPorts-Keyboard-Mouse/main.c (mostly the fake keyboard)
* Fake Xbox code from: https://github.com/Emanem/js2xbox
* 
* Modified (badly) by: Shanti Gilbert for EmuELEC
* Modified further by: Nikolai Wuttke for EmuELEC (Added support for SDL and the SDLGameControllerdb.txt)
* Modified further by: Jacob Smith
* 
* Any help improving this code would be greatly appreciated! 
* 
* DONE: Xbox360 mode: Fix triggers so that they report from 0 to 255 like real Xbox triggers
*       Xbox360 mode: Figure out why the axis are not correctly labeled?  SDL_CONTROLLER_AXIS_RIGHTX / SDL_CONTROLLER_AXIS_RIGHTY / SDL_CONTROLLER_AXIS_TRIGGERLEFT / SDL_CONTROLLER_AXIS_TRIGGERRIGHT
*       Keyboard mode: Add a config file option to load mappings from.
*       add L2/R2 triggers
* 
*/

#include "gptokeyb.h"


int emit_init()
{
#ifdef ENABLE_X11
    if (output_mode == OM_X11)
        return emit_init_x11();
    else
#endif
        return emit_init_uinput();
}


void emit_quit()
{
#ifdef ENABLE_X11
    if (output_mode == OM_X11)
        emit_quit_x11();
    else
#endif
        emit_quit_uinput();
}


void emitKey(int keysym, bool is_pressed, int modifier)
{
#ifdef ENABLE_X11
    if (output_mode == OM_X11)
        emitKey_x11(keysym, is_pressed, modifier);
    else
#endif
        emitKey_uinput(keysym, is_pressed, modifier);
}


void emitTextInputKey(int keysym, bool uppercase)
{
#ifdef ENABLE_X11
    if (output_mode == OM_X11)
        emitTextInputKey_x11(keysym, uppercase);
    else
#endif
        emitTextInputKey_uinput(keysym, uppercase);
}


void emitAxisMotion(int x, int y)
{
#ifdef ENABLE_X11
    if (output_mode == OM_X11)
        emitAxisMotion_x11(x, y);
    else
#endif
        emitAxisMotion_uinput(x, y);
}


void emitMouseMotion(int x, int y)
{
#ifdef ENABLE_X11
    if (output_mode == OM_X11)
        emitMouseMotion_x11(x, y);
    else
#endif
        emitMouseMotion_uinput(x, y);
}


void handleAnalogTrigger(bool is_triggered, bool& was_triggered, int key, int modifier)
{
    if (is_triggered && !was_triggered) {
        emitKey(key, true, modifier);
    } else if (!is_triggered && was_triggered) {
        emitKey(key, false, modifier);
    }

    was_triggered = is_triggered;
}


// convert ASCII chars to key codes
short char_to_keycode(const char* str)
{
    // arrow keys
    if (strcmp(str, "up") == 0)
        return KEY_UP;
    else if (strcmp(str, "down") == 0)
        return KEY_DOWN;
    else if (strcmp(str, "left") == 0)
        return KEY_LEFT;
    else if (strcmp(str, "right") == 0)
        return KEY_RIGHT;

    // special keys
    else if (strcmp(str, "mouse_left") == 0)
        return BTN_LEFT;
    else if (strcmp(str, "mouse_right") == 0)
        return BTN_RIGHT;
    else if (strcmp(str, "space") == 0)
        return KEY_SPACE;
    else if (strcmp(str, "esc") == 0)
        return KEY_ESC;
    else if (strcmp(str, "end") == 0)
        return KEY_END;
    else if (strcmp(str, "home") == 0)
        return KEY_HOME;
    else if (strcmp(str, "shift") == 0)
        return KEY_LEFTSHIFT;
    else if (strcmp(str, "leftshift") == 0)
        return KEY_LEFTSHIFT;
    else if (strcmp(str, "rightshift") == 0)
        return KEY_RIGHTSHIFT;
    else if (strcmp(str, "ctrl") == 0)
        return KEY_LEFTCTRL;
    else if (strcmp(str, "leftctrl") == 0)
        return KEY_LEFTCTRL;
    else if (strcmp(str, "rightctrl") == 0)
        return KEY_RIGHTCTRL;
    else if (strcmp(str, "alt") == 0)
        return KEY_LEFTALT;
    else if (strcmp(str, "leftalt") == 0)
        return KEY_LEFTALT;
    else if (strcmp(str, "rightalt") == 0)
        return KEY_RIGHTALT;
    else if (strcmp(str, "backspace") == 0)
        return KEY_BACKSPACE;
    else if (strcmp(str, "enter") == 0)
        return KEY_ENTER;
    else if (strcmp(str, "pageup") == 0)
        return KEY_PAGEUP;
    else if (strcmp(str, "pagedown") == 0)
        return KEY_PAGEDOWN;
    else if (strcmp(str, "insert") == 0)
        return KEY_INSERT;
    else if (strcmp(str, "delete") == 0)
        return KEY_DELETE;
    else if (strcmp(str, "capslock") == 0)
        return KEY_CAPSLOCK;
    else if (strcmp(str, "tab") == 0)
        return KEY_TAB;
    else if (strcmp(str, "pause") == 0)
        return KEY_PAUSE;
    else if (strcmp(str, "menu") == 0)
        return KEY_MENU;

    // normal keyboard
    else if (strcmp(str, "a") == 0)
        return KEY_A;
    else if (strcmp(str, "b") == 0)
        return KEY_B;
    else if (strcmp(str, "c") == 0)
        return KEY_C;
    else if (strcmp(str, "d") == 0)
        return KEY_D;
    else if (strcmp(str, "e") == 0)
        return KEY_E;
    else if (strcmp(str, "f") == 0)
        return KEY_F;
    else if (strcmp(str, "g") == 0)
        return KEY_G;
    else if (strcmp(str, "h") == 0)
        return KEY_H;
    else if (strcmp(str, "i") == 0)
        return KEY_I;
    else if (strcmp(str, "j") == 0)
        return KEY_J;
    else if (strcmp(str, "k") == 0)
        return KEY_K;
    else if (strcmp(str, "l") == 0)
        return KEY_L;
    else if (strcmp(str, "m") == 0)
        return KEY_M;
    else if (strcmp(str, "n") == 0)
        return KEY_N;
    else if (strcmp(str, "o") == 0)
        return KEY_O;
    else if (strcmp(str, "p") == 0)
        return KEY_P;
    else if (strcmp(str, "q") == 0)
        return KEY_Q;
    else if (strcmp(str, "r") == 0)
        return KEY_R;
    else if (strcmp(str, "s") == 0)
        return KEY_S;
    else if (strcmp(str, "t") == 0)
        return KEY_T;
    else if (strcmp(str, "u") == 0)
        return KEY_U;
    else if (strcmp(str, "v") == 0)
        return KEY_V;
    else if (strcmp(str, "w") == 0)
        return KEY_W;
    else if (strcmp(str, "x") == 0)
        return KEY_X;
    else if (strcmp(str, "y") == 0)
        return KEY_Y;
    else if (strcmp(str, "z") == 0)
        return KEY_Z;

    else if (strcmp(str, "1") == 0)
        return KEY_1;
    else if (strcmp(str, "2") == 0)
        return KEY_2;
    else if (strcmp(str, "3") == 0)
        return KEY_3;
    else if (strcmp(str, "4") == 0)
        return KEY_4;
    else if (strcmp(str, "5") == 0)
        return KEY_5;
    else if (strcmp(str, "6") == 0)
        return KEY_6;
    else if (strcmp(str, "7") == 0)
        return KEY_7;
    else if (strcmp(str, "8") == 0)
        return KEY_8;
    else if (strcmp(str, "9") == 0)
        return KEY_9;
    else if (strcmp(str, "0") == 0)
        return KEY_0;

    else if (strcmp(str, "numpad1") == 0)
        return KEY_KP1;
    else if (strcmp(str, "numpad2") == 0)
        return KEY_KP2;
    else if (strcmp(str, "numpad3") == 0)
        return KEY_KP3;
    else if (strcmp(str, "numpad4") == 0)
        return KEY_KP4;
    else if (strcmp(str, "numpad5") == 0)
        return KEY_KP5;
    else if (strcmp(str, "numpad6") == 0)
        return KEY_KP6;
    else if (strcmp(str, "numpad7") == 0)
        return KEY_KP7;
    else if (strcmp(str, "numpad8") == 0)
        return KEY_KP8;
    else if (strcmp(str, "numpad9") == 0)
        return KEY_KP9;
    else if (strcmp(str, "numpad0") == 0)
        return KEY_KP0;
    else if (strcmp(str, "numpad*") == 0)
        return KEY_KPASTERISK;
    else if (strcmp(str, "numpad/") == 0)
        return KEY_KPSLASH;
    else if (strcmp(str, "numpad+") == 0)
        return KEY_KPPLUS;
    else if (strcmp(str, "numpad-") == 0)
        return KEY_KPMINUS;
    else if (strcmp(str, "numpad,") == 0)
        return KEY_KPCOMMA;
    else if (strcmp(str, "numpad.") == 0)
        return KEY_KPDOT;
    else if (strcmp(str, "numpadenter") == 0)
        return KEY_KPENTER;

    else if (strcmp(str, "f1") == 0)
        return KEY_F1;
    else if (strcmp(str, "f2") == 0)
        return KEY_F2;
    else if (strcmp(str, "f3") == 0)
        return KEY_F3;
    else if (strcmp(str, "f4") == 0)
        return KEY_F4;
    else if (strcmp(str, "f5") == 0)
        return KEY_F5;
    else if (strcmp(str, "f6") == 0)
        return KEY_F6;
    else if (strcmp(str, "f7") == 0)
        return KEY_F7;
    else if (strcmp(str, "f8") == 0)
        return KEY_F8;
    else if (strcmp(str, "f9") == 0)
        return KEY_F9;
    else if (strcmp(str, "f10") == 0)
        return KEY_F10;
    else if (strcmp(str, "f11") == 0)
        return KEY_F11;
    else if (strcmp(str, "f12") == 0)
        return KEY_F12;

    else if (strcmp(str, "@") == 0)
        return KEY_2; // with SHIFT
    else if (strcmp(str, "#") == 0)
        return KEY_3; // with SHIFT
    //else if (strcmp(str, "€") == 0) return KEY_5; // with ALTGR; not ASCII
    else if (strcmp(str, "%") == 0)
        return KEY_5; // with SHIFT
    else if (strcmp(str, "&") == 0)
        return KEY_7; // with SHIFT
    else if (strcmp(str, "*") == 0)
        return KEY_8; // with SHIFT; alternative is KEY_KPASTERISK
    else if (strcmp(str, "-") == 0)
        return KEY_MINUS; // alternative is KEY_KPMINUS
    else if (strcmp(str, "+") == 0)
        return KEY_EQUAL; // with SHIFT; alternative is KEY_KPPLUS
    else if (strcmp(str, "(") == 0)
        return KEY_9; // with SHIFT
    else if (strcmp(str, ")") == 0)
        return KEY_0; // with SHIFT

    else if (strcmp(str, "!") == 0)
        return KEY_1; // with SHIFT
    else if (strcmp(str, "\"") == 0)
        return KEY_APOSTROPHE; // with SHIFT, dead key
    else if (strcmp(str, "\'") == 0)
        return KEY_APOSTROPHE; // dead key
    else if (strcmp(str, ":") == 0)
        return KEY_SEMICOLON; // with SHIFT
    else if (strcmp(str, ";") == 0)
        return KEY_SEMICOLON;
    else if (strcmp(str, "/") == 0)
        return KEY_SLASH;
    else if (strcmp(str, "?") == 0)
        return KEY_SLASH; // with SHIFT
    else if (strcmp(str, ".") == 0)
        return KEY_DOT;
    else if (strcmp(str, ",") == 0)
        return KEY_COMMA;

    // special chars
    else if (strcmp(str, "~") == 0)
        return KEY_GRAVE; // with SHIFT, dead key
    else if (strcmp(str, "`") == 0)
        return KEY_GRAVE; // dead key
    else if (strcmp(str, "|") == 0)
        return KEY_BACKSLASH; // with SHIFT
    else if (strcmp(str, "{") == 0)
        return KEY_LEFTBRACE; // with SHIFT
    else if (strcmp(str, "}") == 0)
        return KEY_RIGHTBRACE; // with SHIFT
    else if (strcmp(str, "$") == 0)
        return KEY_4; // with SHIFT
    else if (strcmp(str, "^") == 0)
        return KEY_6; // with SHIFT, dead key
    else if (strcmp(str, "_") == 0)
        return KEY_MINUS; // with SHIFT
    else if (strcmp(str, "=") == 0)
        return KEY_EQUAL;
    else if (strcmp(str, "[") == 0)
        return KEY_LEFTBRACE;
    else if (strcmp(str, "]") == 0)
        return KEY_RIGHTBRACE;
    else if (strcmp(str, "\\") == 0)
        return KEY_BACKSLASH;
    else if (strcmp(str, "<") == 0)
        return KEY_COMMA; // with SHIFT
    else if (strcmp(str, ">") == 0)
        return KEY_DOT; // with SHIFT

    // Default
    return 0;
}


void doKillMode()
{
    if (pckill_mode) {
        emitKey(KEY_F4, true, KEY_LEFTALT);
        SDL_Delay(15);
        emitKey(KEY_F4, false, KEY_LEFTALT);
    }

    SDL_RemoveTimer( state.key_repeat_timer_id );
    if (state.start_jsdevice == state.hotkey_jsdevice) {
        if (! sudo_kill) {
            printf("[GPTK]: Killing: %s\n", AppToKill);
            system(("pkill -f '" + std::string(AppToKill) + "' ").c_str());
            // system("show_splash.sh exit");

            sleep(3);
            if (system((" pgrep -f '" + std::string(AppToKill) + "' ").c_str()) == 0) {
                printf("[GPTK]: Forcefully Killing: %s\n", AppToKill);
                system((" pkill -9 -f '" + std::string(AppToKill) + "' ").c_str());
            }

            exit(0); 
        } else {
            printf("[GPTK]: Killing: %s\n", AppToKill);
            system((" sudo pkill -f '" + std::string(AppToKill) + "' ").c_str());

            sleep(3);

            if (system((" pgrep -f '" + std::string(AppToKill) + "' ").c_str()) == 0) {
                printf("[GPTK]: Forcefully Killing: %s\n", AppToKill);
                system((" sudo pkill -9 -f '" + std::string(AppToKill) + "' ").c_str());
            }

            exit(0); 
        }
    } // sudo kill
}
