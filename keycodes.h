#ifndef _KEYCODES_H_
#define _KEYCODES_H_

//////////////////////////////////////////////////////
// นิยามคีย์โค้ดตามมาตรฐาน USB Human Interface Device
// 
// ข้อมูลจากเอกสาร Universal Serial Bus HID Usage Tables
// หัวข้อ 10 Keyboard/Keypad Page (0x07)
//////////////////////////////////////////////////////
#define KEY_NONE          0x00
#define KEY_A             0x04
#define KEY_B             0x05
#define KEY_C             0x06
#define KEY_D             0x07
#define KEY_E             0x08
#define KEY_F             0x09
#define KEY_G             0x0A
#define KEY_H             0x0B
#define KEY_I             0x0C
#define KEY_J             0x0D
#define KEY_K             0x0E
#define KEY_L             0x0F
#define KEY_M             0x10
#define KEY_N             0x11
#define KEY_O             0x12
#define KEY_P             0x13
#define KEY_Q             0x14
#define KEY_R             0x15
#define KEY_S             0x16
#define KEY_T             0x17
#define KEY_U             0x18
#define KEY_V             0x19
#define KEY_W             0x1A
#define KEY_X             0x1B
#define KEY_Y             0x1C
#define KEY_Z             0x1D
#define KEY_1             0x1E
#define KEY_2             0x1F
#define KEY_3             0x20
#define KEY_4             0x21
#define KEY_5             0x22
#define KEY_6             0x23
#define KEY_7             0x24
#define KEY_8             0x25
#define KEY_9             0x26
#define KEY_0             0x27
#define KEY_RETURN        0x28
#define KEY_ESC           0x29
#define KEY_DELETE        0x2A
#define KEY_TAB           0x2B
#define KEY_SPACE         0x2C
#define KEY_MINUS         0x2D
#define KEY_EQUAL         0x2E
#define KEY_LBRACKET      0x2F
#define KEY_RBRACKET      0x30
#define KEY_BACKSLASH     0x31
#define KEY_SEMICOLON     0x33
#define KEY_GRAVE         0x35
#define KEY_COMMA         0x36
#define KEY_PERIOD        0x37
#define KEY_SLASH         0x38
#define KEY_CAPS_LOCK     0x39
#define KEY_F1            0x3A
#define KEY_F2            0x3B
#define KEY_F3            0x3C
#define KEY_F4            0x3D
#define KEY_F5            0x3E
#define KEY_F6            0x3F
#define KEY_F7            0x40
#define KEY_F8            0x41
#define KEY_F9            0x42
#define KEY_F10           0x43
#define KEY_F11           0x44
#define KEY_F12           0x45
#define KEY_PRINT_SCREEN  0x46
#define KEY_SCROLL_LOCK   0x47
#define KEY_PAUSE         0x48
#define KEY_INSERT        0x49
#define KEY_HOME          0x4A
#define KEY_PAGEUP        0x4B
#define KEY_END           0x4D
#define KEY_PAGEDN        0x4E
#define KEY_RIGHT_ARROW   0x4F
#define KEY_LEFT_ARROW    0x50
#define KEY_DOWN_ARROW    0x51
#define KEY_UP_ARROW      0x52
#define KEY_NUM_LOCK      0x54
#define KEY_PAD_MULTIPLY  0x55
#define KEY_PAD_MINUS     0x56
#define KEY_PAD_PLUS      0x57
#define KEY_PAD_ENTER     0x58
#define KEY_PAD_1         0x59
#define KEY_PAD_2         0x5A
#define KEY_PAD_3         0x5B
#define KEY_PAD_4         0x5C
#define KEY_PAD_5         0x5D
#define KEY_PAD_6         0x5E
#define KEY_PAD_7         0x5F
#define KEY_PAD_8         0x60
#define KEY_PAD_9         0x61
#define KEY_PAD_0         0x62
#define KEY_PAD_PERIOD    0x63

// Modifiers
#define KEY_MOD_LEFT_CTRL   0x01
#define KEY_MOD_LEFT_SHIFT  0x02
#define KEY_MOD_LEFT_ALT    0x04
#define KEY_MOD_LEFT_GUI    0x08
#define KEY_MOD_RIGHT_CTRL  0x10
#define KEY_MOD_RIGHT_SHIFT 0x20
#define KEY_MOD_RIGHT_ALT   0x40
#define KEY_MOD_RIGHT_GUI   0x80

#endif
