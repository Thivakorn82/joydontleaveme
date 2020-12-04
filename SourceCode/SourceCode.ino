// == การคอมไพล์ ==
// ต้องการไลบรารี pt และ usbdrv (มีมาให้กับแพคเกจ cpeku-practicum เรียบร้อยแล้ว)
//
// == หมายเหตุ ==
// อุปกรณ์ USB ที่จำลองขึ้นกำหนดให้มีหมายเลข VID:PID เป็น 16c0:0482 ซึ่งได้รับการจดทะเบียน
// ให้ใช้งานเป็น Keyboard/Mouse/Joystick combo โดย VOTI
//////////////////////////////////////////////////////////////////////////////

#include  <pt.h>
#include  <usbdrv.h>
#include "keycodes.h"

//---------------------------------------------------//
#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_MOUSE 2
#define REPORT_ID_GAMEPAD 3
//--------------------------------------------------//
#define LIGHT_HIGH_THRES 700
#define LIGHT_LOW_THRES 300

int btn_shoot1 = PIN_PC2;
int btn_switch1 = PIN_PD0;
int btn_shoot2 = PIN_PB2;
int btn_switch2 = PIN_PD1;

int joystick_x1 = PIN_PC0;
int joystick_x2 = PIN_PB0;
int joystick_y1 = PIN_PC1;
int joystick_y2 = PIN_PB1;

// มาโครสำหรับจำลองการหน่วงเวลาใน protothread
#define PT_DELAY(pt,ms,tsVar) \
  tsVar = millis(); \
  PT_WAIT_UNTIL(pt, millis()-tsVar >= (ms));

//--------------------------------------------testjoystick-------------------------------------------------//
// Arduino pin numbers
//const int SW_pin = 2; // digital pin connected to switch output
//const int X_pin = PIN_PC0; // analog pin connected to X output
//const int Y_pin = PIN_PC1; // analog pin connected to Y output

//void setup() {
//  pinMode(SW_pin, INPUT);
//  digitalWrite(SW_pin, HIGH);
  //Serial.begin(115200);
//}

//void loop() {
//  Serial.print("Switch:  ");
//  Serial.print(digitalRead(SW_pin));
//  Serial.print("\n");
  //Serial.print("X-axis: ");
  //Serial.print(analogRead(X_pin));
  //Serial.print("\n");
  //Serial.print("Y-axis: ");
  //Serial.println(analogRead(Y_pin));
  //Serial.print("\n\n");
  //delay(500);
//}
//--------------------------------------------------------------------------------------------------------//


/////////////////////////////////////////////////////////////////////
// USB report descriptor, สร้างขึ้นจาก HID Descriptor Tool ซึ่ง
// ดาวน์โหลดได้จาก
//    http://www.usb.org/developers/hidpage/dt2_4.zip
//
// หรือใช้ HIDEdit ซึ่งให้บริการฟรีผ่านเว็บที่ http://hidedit.org/ << 2019 เข้าไม่ได้
//
// *** ไม่แนะนำให้สร้างเองด้วยมือ ***
/////////////////////////////////////////////////////////////////////
PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = 
{
  ////////////////////////////////////
  // โครงสร้าง HID report สำหรับคียบอร์ด
  ////////////////////////////////////
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x85, 0x01,                    //   REPORT_ID (1)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x95, 0x08,                    //   REPORT_COUNT (8)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
  0x95, 0x06,                    //   REPORT_COUNT (6)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
  0xc0,                          // END_COLLECTION

  //////////////////////////////////////
  // โครงสร้าง HID report สำหรับเมาส์ 3 ปุ่ม
  //////////////////////////////////////
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x0b, 0x02, 0x00, 0x01, 0x00,  // USAGE (Generic Desktop:Mouse)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x0b, 0x01, 0x00, 0x01, 0x00,  //   USAGE (Generic Desktop:Pointer)
  0xa1, 0x00,                    //   COLLECTION (Physical)
  0x85, 0x02,                    //     REPORT_ID (2)
  0x05, 0x09,                    //     USAGE_PAGE (Button)
  0x1b, 0x01, 0x00, 0x09, 0x00,  //     USAGE_MINIMUM (Button:Button 1)
  0x2b, 0x03, 0x00, 0x09, 0x00,  //     USAGE_MAXIMUM (Button:Button 3)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //     REPORT_SIZE (1)
  0x95, 0x03,                    //     REPORT_COUNT (3)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)
  0x75, 0x05,                    //     REPORT_SIZE (5)
  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
  0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
  0x0b, 0x30, 0x00, 0x01, 0x00,  //     USAGE (Generic Desktop:X)
  0x0b, 0x31, 0x00, 0x01, 0x00,  //     USAGE (Generic Desktop:Y)
  0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
  0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x95, 0x02,                    //     REPORT_COUNT (2)
  0x81, 0x06,                    //     INPUT (Data,Var,Rel)
  0xc0,                          //     END_COLLECTION
  0xc0,                          // END_COLLECTION

  ////////////////////////////////////////////////////////////
  // โครงสร้าง HID report สำหรับเกมแพดแบบหนึ่งปุ่มกดและหนึ่งก้านแอนะล็อก
  ////////////////////////////////////////////////////////////
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x04,                    // USAGE (Joystick)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
  0x09, 0x01,                    //   USAGE (Pointer)
  0xa1, 0x00,                    //   COLLECTION (Physical)
  0x85, 0x03,                    //     REPORT_ID (3)
  0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
  0x09, 0x32,                    //     USAGE (Z)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x03,              //     LOGICAL_MAXIMUM (1023)
  0x75, 0x0a,                    //     REPORT_SIZE (10)
  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)
  0x05, 0x09,                    //     USAGE_PAGE (Button)
  0x09, 0x01,                    //     USAGE (Button 1)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //     REPORT_SIZE (1)
  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)
  0x75, 0x01,                    //     REPORT_SIZE (1)
  0x95, 0x05,                    //     REPORT_COUNT (5)
  0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
  0xc0,                          //   END_COLLECTION
  0xc0                           // END_COLLECTION
};

struct ReportKeyboard
{
  /* +----\------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |Byte \ Bit |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
   * +------\----+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  0        |               Report ID = 1                   |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  1        |           Modifiers (shift,ctrl,etc)          |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  2        |                 Key Code                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  3        |                 Key Code                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  4        |                 Key Code                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  5        |                 Key Code                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  6        |                 Key Code                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  7        |                 Key Code                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
    */
  uint8_t  report_id;
  uint8_t  modifiers;
  uint8_t  key_code[6];
  
};

struct ReportMouse
{
  /* +----\------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |Byte \ Bit |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
   * +------\----+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  0        |               Report ID = 2                   |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  1        |              Buttons' statuses                |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  2        |                  Delta X                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  3        |                  Delta Y                      |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   */
  uint8_t  report_id;
  uint8_t  buttons;
  int8_t   dx;
  int8_t   dy;
};

struct ReportGamepad
{
  /*
   * +----\------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |Byte \ Bit |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
   * +------\----+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  0        |               Report ID = 3                   |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  1        |                 Light (L)                     |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   * |  2        |            UNUSED           | Btn | Light (H) |
   * +-----------+-----+-----+-----+-----+-----+-----+-----+-----+
   */
  uint8_t  report_id;
  uint16_t light:10;  // ค่าแสง 10 บิต (0-1023)
  uint8_t  button:1;  // ปุ่ม 1 บิต
  uint8_t  unused:5;  // ไม่ใช้ แต่ต้องเติมให้เต็มไบต์
};

ReportKeyboard reportKeyboard;
ReportMouse reportMouse;
ReportGamepad reportGamepad;


struct pt main_pt;
struct pt shoot1_pt;
struct pt shoot2_pt;
struct pt switch1_pt;
struct pt switch2_pt;
struct pt joystickx1_pt;
struct pt joystickx2_pt;
struct pt joysticky1_pt;
struct pt joysticky2_pt;

////////////////////////////////////////////////////////////////
// Automatically called by usbpoll() when host makes a request
////////////////////////////////////////////////////////////////
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
  return 0;
}

void sendKey(uint8_t index, uint8_t keycode, uint8_t modifiers)
{
  reportKeyboard.key_code[index] = keycode;
  reportKeyboard.modifiers = modifiers;
  usbSetInterrupt((uchar*)&reportKeyboard, sizeof(reportKeyboard));
}
//////////////////////////////////////////////////////////////
void sendMouse(int8_t dx, int8_t dy, uint8_t buttons)
{
  reportMouse.dx = dx;
  reportMouse.dy = dy;
  reportMouse.buttons = buttons;
  usbSetInterrupt((uchar*)&reportMouse, sizeof(reportMouse));
}
///////////////////////////////////////////////////

//ส่วนของ switch

PT_THREAD(do_shoot1(struct pt *pt)){
  static uint32_t ts = 0;
  PT_BEGIN(pt);

  for(;;){
    PT_WAIT_UNTIL(pt,digitalRead(btn_shoot1)==0);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(0,KEY_SPACE,0);

    PT_WAIT_UNTIL(pt,digitalRead(btn_shoot1)==1);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(0,KEY_NONE,0);
  }
  PT_END(pt);
}


PT_THREAD(do_switch1(struct pt *pt)){
  static uint32_t ts = 0;
  PT_BEGIN(pt);

  for(;;){
    PT_WAIT_UNTIL(pt,digitalRead(btn_switch1)==0);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(0,KEY_E,0);

    PT_WAIT_UNTIL(pt,digitalRead(btn_switch1)==1);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(0,KEY_NONE,0);
  }
  PT_END(pt);
}

PT_THREAD(do_shoot2(struct pt *pt)){
  static uint32_t ts = 0;
  PT_BEGIN(pt);

  for(;;){
    PT_WAIT_UNTIL(pt,digitalRead(btn_shoot2)==0);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(1,KEY_SLASH,0);

    PT_WAIT_UNTIL(pt,digitalRead(btn_shoot2)==1);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(1,KEY_NONE,0);
  }
  PT_END(pt);
}

PT_THREAD(do_switch2(struct pt *pt)){
  static uint32_t ts = 0;
  PT_BEGIN(pt);

  for(;;){
    PT_WAIT_UNTIL(pt,digitalRead(btn_switch2)==0);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(1,KEY_PERIOD,0);

    PT_WAIT_UNTIL(pt,digitalRead(btn_switch2)==1);
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(1,KEY_NONE,0);
  }
  PT_END(pt);
}
//--------------------------------------------------------------------------------------------------//
//ส่วนของจอยสติก
PT_THREAD(do_joystickx1(struct pt *pt))//x1
{
  static uint32_t ts = 0;
  static uint32_t checker = 0;
  PT_BEGIN(pt);

  for (;;) {
    PT_WAIT_UNTIL(pt,(analogRead(joystick_x1)>620) or (analogRead(joystick_x1)<320));
    if (analogRead(joystick_x1)>620) {
      checker = 1;  
    }
    else if (analogRead(joystick_x1)<320) {
      checker = 2;  
    }
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    if (checker == 1) {
      sendKey(5,KEY_D,0);
    }
    else if (checker == 2) {
      sendKey(5,KEY_A,0);
    }     

    PT_WAIT_UNTIL(pt,((analogRead(joystick_x1)<=620) and (checker == 1)) or ((analogRead(joystick_x1)>=320) and (checker == 2)));
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(5,KEY_NONE,0); 
    checker = 0;                                                                     
  }
  PT_END(pt);
}

PT_THREAD(do_joystickx2(struct pt *pt))//x2
{
  static uint32_t ts = 0;
  static uint32_t checker = 0;
  PT_BEGIN(pt);

  for (;;) {
    PT_WAIT_UNTIL(pt,(analogRead(joystick_x2)>620) or (analogRead(joystick_x2)<320));
    if (analogRead(joystick_x2)>620) {
      checker = 1;  
    }
    else if (analogRead(joystick_x2)<320) {
      checker = 2;  
    }
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    if (checker == 1) {
      sendKey(5,KEY_D,0);
    }
    else if (checker == 2) {
      sendKey(5,KEY_A,0);
    }     

    PT_WAIT_UNTIL(pt,((analogRead(joystick_x1)<=620) and (checker == 1)) or ((analogRead(joystick_x1)>=320) and (checker == 2)));
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(5,KEY_NONE,0); 
    checker = 0;                                                                     
  }
  PT_END(pt);
}

PT_THREAD(do_joysticky1(struct pt *pt))//y1
{
  static uint32_t ts = 0;
  static uint32_t checker = 0;
  PT_BEGIN(pt);

  for (;;) {
    PT_WAIT_UNTIL(pt,(analogRead(joystick_y1)>620) or (analogRead(joystick_y1)<320));
    if (analogRead(joystick_y1)>620) {
      checker = 1;  
    }
    else if (analogRead(joystick_y1)<320) {
      checker = 2;  
    }
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    if (checker == 1) {
      sendKey(5,KEY_W,0);
    }
    else if (checker == 2) {
      sendKey(5,KEY_S,0);
    }     

    PT_WAIT_UNTIL(pt,((analogRead(joystick_x1)<=620) and (checker == 1)) or ((analogRead(joystick_x1)>=320) and (checker == 2)));
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(5,KEY_NONE,0); 
    checker = 0;                                                                     
  }
  PT_END(pt);
}

PT_THREAD(do_joysticky2(struct pt *pt))//y1
{
  static uint32_t ts = 0;
  static uint32_t checker = 0;
  PT_BEGIN(pt);

  for (;;) {
    PT_WAIT_UNTIL(pt,(analogRead(joystick_y2)>620) or (analogRead(joystick_y2)<320));
    if (analogRead(joystick_y2)>620) {
      checker = 1;  
    }
    else if (analogRead(joystick_y2)<320) {
      checker = 2;  
    }
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    if (checker == 1) {
      sendKey(5,KEY_W,0);
    }
    else if (checker == 2) {
      sendKey(5,KEY_S,0);
    }     

    PT_WAIT_UNTIL(pt,((analogRead(joystick_x1)<=620) and (checker == 1)) or ((analogRead(joystick_x1)>=320) and (checker == 2)));
    PT_WAIT_UNTIL(pt,usbInterruptIsReady());
    sendKey(5,KEY_NONE,0); 
    checker = 0;                                                                     
  }
  PT_END(pt);
}



PT_THREAD(do_main(struct pt *pt)){
  PT_BEGIN(pt);
  do_shoot1(&shoot1_pt);
  do_switch1(&switch1_pt);
  do_shoot2(&shoot2_pt);
  do_switch2(&switch2_pt);
  do_joystickx1(&joystickx1_pt);
  do_joystickx2(&joystickx2_pt);
  do_joysticky1(&joysticky1_pt);
  do_joysticky2(&joysticky2_pt);
  
  PT_END(pt);
}




void setup(){
  usbInit();
  usbDeviceDisconnect();
  delay(300);
  usbDeviceConnect();
  Serial.begin(9600);

  //pinMode
  pinMode(btn_shoot1,INPUT_PULLUP);
  pinMode(btn_switch1,INPUT_PULLUP);
  pinMode(btn_shoot2,INPUT_PULLUP);
  pinMode(btn_switch2,INPUT_PULLUP);

  reportKeyboard.report_id = REPORT_ID_KEYBOARD;
  reportKeyboard.modifiers = 0;
  //reportKeyboard.key_code[0] = KEY_NONE;
  //reportKeyboard.key_code[1] = KEY_NONE;
  //reportKeyboard.key_code[2] = KEY_NONE;
  //reportKeyboard.key_code[3] = KEY_NONE;
  //reportKeyboard.key_code[4] = KEY_NONE;
  //reportKeyboard.key_code[5] = KEY_NONE;

  reportMouse.report_id = REPORT_ID_MOUSE;
  reportMouse.dx = 0;
  reportMouse.dy = 0;
  reportMouse.buttons = 0;

  reportGamepad.report_id = REPORT_ID_GAMEPAD;
  reportGamepad.light = 0;
  reportGamepad.button = 0;

  PT_INIT(&main_pt);
  PT_INIT(&shoot1_pt);
  PT_INIT(&switch1_pt);
  PT_INIT(&shoot2_pt);
  PT_INIT(&switch2_pt);  
  PT_INIT(&joystickx1_pt);
  PT_INIT(&joystickx2_pt);
  PT_INIT(&joysticky1_pt);
  PT_INIT(&joysticky2_pt);

}


void loop(){
  usbPoll();

  do_main(&main_pt);
}
