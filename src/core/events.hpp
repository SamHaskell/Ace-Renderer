#pragma once

#include "core/defines.hpp"

namespace Ace {

    /*
        NOTE: Made to match SDL as standard, will need to add some translation/mapping if using a different windowing solution
        https://wiki.libsdl.org/SDL2/SDLKeycodeLookup
    */ 

    enum KeyCode {
        KEY_UNKNOWN = 0,
        KEY_RETURN = '\r',
        KEY_ESCAPE = '\x1B',
        KEY_BACKSPACE = '\b',
        KEY_TAB = '\t',

        KEY_SPACE = ' ',
        KEY_EXCLAIM = '!',
        KEY_QUOTEDBL = '"',
        KEY_HASH = '#',
        KEY_PERCENT = '%',
        KEY_DOLLAR = '$',
        KEY_AMPERSAND = '&',
        KEY_QUOTE = '\'',
        KEY_LEFTPAREN = '(',
        KEY_RIGHTPAREN = ')',
        KEY_ASTERISK = '*',
        KEY_PLUS = '+',
        KEY_COMMA = ',',
        KEY_MINUS = '-',
        KEY_PERIOD = '.',
        KEY_SLASH = '/',

        KEY_0 = '0',
        KEY_1 = '1',
        KEY_2 = '2',
        KEY_3 = '3',
        KEY_4 = '4',
        KEY_5 = '5',
        KEY_6 = '6',
        KEY_7 = '7',
        KEY_8 = '8',
        KEY_9 = '9',
        
        KEY_COLON = ':',
        KEY_SEMICOLON = ';',
        KEY_LESS = '<',
        KEY_EQUALS = '=',
        KEY_GREATER = '>',
        KEY_QUESTION = '?',
        KEY_AT = '@',

        KEY_LEFTBRACKET = '[',
        KEY_BACKSLASH = '\\',
        KEY_RIGHTBRACKET = ']',
        KEY_CARET = '^',
        KEY_UNDERSCORE = '_',
        KEY_BACKQUOTE = '`',

        KEY_a = 'a',
        KEY_b = 'b',
        KEY_c = 'c',
        KEY_d = 'd',
        KEY_e = 'e',
        KEY_f = 'f',
        KEY_g = 'g',
        KEY_h = 'h',
        KEY_i = 'i',
        KEY_j = 'j',
        KEY_k = 'k',
        KEY_l = 'l',
        KEY_m = 'm',
        KEY_n = 'n',
        KEY_o = 'o',
        KEY_p = 'p',
        KEY_q = 'q',
        KEY_r = 'r',
        KEY_s = 's',
        KEY_t = 't',
        KEY_u = 'u',
        KEY_v = 'v',
        KEY_w = 'w',
        KEY_x = 'x',
        KEY_y = 'y',
        KEY_z = 'z',
    };

    enum class EventType {
        Key,
        MouseMotion,
        MouseButton
    };

    struct KeyEvent {
        KeyCode KeyCode;
        bool SystemRepeat;
        bool Pressed;
    };

    struct MouseMotionEvent {
        i32 X;
        i32 Y;
        i32 DeltaX;
        i32 DeltaY;
    };

    struct MouseButtonEvent {
        u32 ButtonCode;
        i32 X;
        i32 Y;
        bool Pressed;
    };

    struct Event {
        EventType Type;
        union {
            KeyEvent Key;
            MouseMotionEvent MouseMotion;
            MouseButtonEvent MouseButton;
        };
    };
};