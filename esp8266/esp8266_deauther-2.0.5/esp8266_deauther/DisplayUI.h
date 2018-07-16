#ifndef DisplayUI_h
#define DisplayUI_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <FS.h>
extern "C" {
  #include "user_interface.h"
}
#include "language.h"
#include "A_config.h"
#include "Settings.h"
#include "Names.h"
#include "SSIDs.h"
#include "Scan.h"
#include "Attack.h"



extern Settings settings;
extern Names names;
extern SSIDs ssids;
extern Accesspoints accesspoints;
extern Stations stations;
extern Scan scan;
extern Attack attack;
extern uint32_t currentTime;

extern String buildString(String left, String right, int maxLen);
extern String replaceUtf8(String str, String r);

// different display modes
#define SCREEN_MODE_OFF 0
#define SCREEN_MODE_BUTTON_TEST 1
#define SCREEN_MODE_MENU 2
#define SCREEN_MODE_LOADSCAN 3
#define SCREEN_MODE_PACKETMONITOR 4
#define SCREEN_MODE_INTRO 5

// ===== adjustable ===== //
#define BUTTON_DELAY 280  // in ms
#define DRAW_INTERVAL 100 // 100ms = 10 FPS
#define CHARS_PER_LINE 17
#define SCROLL_SPEED 5
#define SCREEN_INTRO_TIME 2500
// ====================== //

struct Menu;
struct MenuNode;
struct Button;

struct Button {
  bool enabled; // use button
  uint8_t gpio; // pin that is used
  bool pushed; // currently pushed
  bool hold; // if button was hold (only used for buttonA at the moment)
  uint32_t time; // last time it was pushed 
  std::function<bool()> read; // function to return if button is pushed
  std::function<void()> setup; // function to enable/setup the button, if needed
  std::function<void()> push; // function that is executed when button is pushed
  std::function<void()> release; // function that is executed when button is released
};

struct Menu {
  SimpleList<MenuNode>* list;
  Menu* parentMenu;
  uint8_t selected;
  std::function<void()> build; // function that is executed when button is clicked
};

struct MenuNode {
  std::function<String()> getStr; // function used to create the displayed string
  std::function<void()> click; // function that is executed when node is clicked
  std::function<void()> hold; // function that is executed when node is pressed for > 800ms
};

class DisplayUI {
  public:
    DisplayUI();
    void setup();
    void update();
    void on();
    void off();

    uint8_t mode = SCREEN_MODE_MENU;
  private:
    DEAUTHER_DISPLAY // see config.h

    void setupDisplay();
    void setupButtons();
    
    int16_t selectedID = 0; // i.e. access point ID to draw the apMenu
    uint16_t scrollCounter = 0; // for horizontal scrolling
    uint32_t drawTime = 0; // last time a frame was drawn
    uint32_t startTime = 0; // when the screen was enabled
    bool enabled = false; // display enabled
    
    Button buttonUp;
    Button buttonDown;
    Button buttonLeft;
    Button buttonRight;
    Button buttonA;
    Button buttonB;
    
    // selected attack modes
    bool beaconSelected = false;
    bool deauthSelected = false;
    bool probeSelected = false;

    // functions for buttons
    bool updateButton(Button* button); // read and update

    // draw functions
    void draw();
    void drawString(int x, int y, String str);
    void drawButtonTest();
    void drawMenu();
    void drawLoadingScan();
    void drawPacketMonitor();
    void drawIntro();
    void clearMenu(Menu* menu);
    
    // menu functions
    void changeMenu(Menu* menu);
    void goBack();
    void createMenu(Menu* menu, Menu* parent, std::function<void()> build);

    void addMenuNode(Menu* menu, std::function<String()> getStr, std::function<void()> click, std::function<void()> hold);
    void addMenuNode(Menu* menu, std::function<String()> getStr, std::function<void()> click);
    void addMenuNode(Menu* menu, std::function<String()> getStr, Menu* next);
    void addMenuNode(Menu* menu, const char* ptr, std::function<void()> click);
    void addMenuNode(Menu* menu, const char* ptr, Menu* next);
    
    // menus
    Menu* currentMenu;
    
    Menu mainMenu;
    
    Menu scanMenu;
    Menu showMenu;
    Menu attackMenu;
    
    Menu apListMenu;
    Menu stationListMenu;
    Menu nameListMenu;
    Menu ssidListMenu;
    
    Menu apMenu;
    Menu stationMenu;
    Menu nameMenu;
    Menu ssidMenu;
};

// ===== FONT ===== //
// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t DejaVu_Sans_Mono_12[] PROGMEM = {
  0x07, // Width: 7
  0x0F, // Height: 15
  0x20, // First Char: 32
  0xE0, // Numbers of Chars: 224

  // Jump Table:
  0xFF, 0xFF, 0x00, 0x07,  // 32:65535
  0x00, 0x00, 0x08, 0x07,  // 33:0
  0x00, 0x08, 0x09, 0x07,  // 34:8
  0x00, 0x11, 0x0D, 0x07,  // 35:17
  0x00, 0x1E, 0x0C, 0x07,  // 36:30
  0x00, 0x2A, 0x0E, 0x07,  // 37:42
  0x00, 0x38, 0x0E, 0x07,  // 38:56
  0x00, 0x46, 0x07, 0x07,  // 39:70
  0x00, 0x4D, 0x0C, 0x07,  // 40:77
  0x00, 0x59, 0x0A, 0x07,  // 41:89
  0x00, 0x63, 0x0B, 0x07,  // 42:99
  0x00, 0x6E, 0x0E, 0x07,  // 43:110
  0x00, 0x7C, 0x08, 0x07,  // 44:124
  0x00, 0x84, 0x0A, 0x07,  // 45:132
  0x00, 0x8E, 0x08, 0x07,  // 46:142
  0x00, 0x96, 0x0D, 0x07,  // 47:150
  0x00, 0xA3, 0x0E, 0x07,  // 48:163
  0x00, 0xB1, 0x0C, 0x07,  // 49:177
  0x00, 0xBD, 0x0E, 0x07,  // 50:189
  0x00, 0xCB, 0x0E, 0x07,  // 51:203
  0x00, 0xD9, 0x0E, 0x07,  // 52:217
  0x00, 0xE7, 0x0E, 0x07,  // 53:231
  0x00, 0xF5, 0x0E, 0x07,  // 54:245
  0x01, 0x03, 0x0D, 0x07,  // 55:259
  0x01, 0x10, 0x0E, 0x07,  // 56:272
  0x01, 0x1E, 0x0E, 0x07,  // 57:286
  0x01, 0x2C, 0x08, 0x07,  // 58:300
  0x01, 0x34, 0x08, 0x07,  // 59:308
  0x01, 0x3C, 0x0E, 0x07,  // 60:316
  0x01, 0x4A, 0x0E, 0x07,  // 61:330
  0x01, 0x58, 0x0E, 0x07,  // 62:344
  0x01, 0x66, 0x0D, 0x07,  // 63:358
  0x01, 0x73, 0x0E, 0x07,  // 64:371
  0x01, 0x81, 0x0E, 0x07,  // 65:385
  0x01, 0x8F, 0x0E, 0x07,  // 66:399
  0x01, 0x9D, 0x0E, 0x07,  // 67:413
  0x01, 0xAB, 0x0E, 0x07,  // 68:427
  0x01, 0xB9, 0x0E, 0x07,  // 69:441
  0x01, 0xC7, 0x0D, 0x07,  // 70:455
  0x01, 0xD4, 0x0E, 0x07,  // 71:468
  0x01, 0xE2, 0x0E, 0x07,  // 72:482
  0x01, 0xF0, 0x0C, 0x07,  // 73:496
  0x01, 0xFC, 0x0C, 0x07,  // 74:508
  0x02, 0x08, 0x0E, 0x07,  // 75:520
  0x02, 0x16, 0x0E, 0x07,  // 76:534
  0x02, 0x24, 0x0E, 0x07,  // 77:548
  0x02, 0x32, 0x0E, 0x07,  // 78:562
  0x02, 0x40, 0x0E, 0x07,  // 79:576
  0x02, 0x4E, 0x0D, 0x07,  // 80:590
  0x02, 0x5B, 0x0E, 0x07,  // 81:603
  0x02, 0x69, 0x0E, 0x07,  // 82:617
  0x02, 0x77, 0x0E, 0x07,  // 83:631
  0x02, 0x85, 0x0D, 0x07,  // 84:645
  0x02, 0x92, 0x0E, 0x07,  // 85:658
  0x02, 0xA0, 0x0D, 0x07,  // 86:672
  0x02, 0xAD, 0x0E, 0x07,  // 87:685
  0x02, 0xBB, 0x0E, 0x07,  // 88:699
  0x02, 0xC9, 0x0D, 0x07,  // 89:713
  0x02, 0xD6, 0x0E, 0x07,  // 90:726
  0x02, 0xE4, 0x0A, 0x07,  // 91:740
  0x02, 0xEE, 0x0E, 0x07,  // 92:750
  0x02, 0xFC, 0x08, 0x07,  // 93:764
  0x03, 0x04, 0x0B, 0x07,  // 94:772
  0x03, 0x0F, 0x0E, 0x07,  // 95:783
  0x03, 0x1D, 0x09, 0x07,  // 96:797
  0x03, 0x26, 0x0C, 0x07,  // 97:806
  0x03, 0x32, 0x0C, 0x07,  // 98:818
  0x03, 0x3E, 0x0C, 0x07,  // 99:830
  0x03, 0x4A, 0x0C, 0x07,  // 100:842
  0x03, 0x56, 0x0C, 0x07,  // 101:854
  0x03, 0x62, 0x0B, 0x07,  // 102:866
  0x03, 0x6D, 0x0C, 0x07,  // 103:877
  0x03, 0x79, 0x0C, 0x07,  // 104:889
  0x03, 0x85, 0x0C, 0x07,  // 105:901
  0x03, 0x91, 0x0A, 0x07,  // 106:913
  0x03, 0x9B, 0x0C, 0x07,  // 107:923
  0x03, 0xA7, 0x0C, 0x07,  // 108:935
  0x03, 0xB3, 0x0C, 0x07,  // 109:947
  0x03, 0xBF, 0x0C, 0x07,  // 110:959
  0x03, 0xCB, 0x0C, 0x07,  // 111:971
  0x03, 0xD7, 0x0C, 0x07,  // 112:983
  0x03, 0xE3, 0x0C, 0x07,  // 113:995
  0x03, 0xEF, 0x0D, 0x07,  // 114:1007
  0x03, 0xFC, 0x0C, 0x07,  // 115:1020
  0x04, 0x08, 0x0C, 0x07,  // 116:1032
  0x04, 0x14, 0x0C, 0x07,  // 117:1044
  0x04, 0x20, 0x0B, 0x07,  // 118:1056
  0x04, 0x2B, 0x0D, 0x07,  // 119:1067
  0x04, 0x38, 0x0C, 0x07,  // 120:1080
  0x04, 0x44, 0x0B, 0x07,  // 121:1092
  0x04, 0x4F, 0x0C, 0x07,  // 122:1103
  0x04, 0x5B, 0x0C, 0x07,  // 123:1115
  0x04, 0x67, 0x08, 0x07,  // 124:1127
  0x04, 0x6F, 0x0B, 0x07,  // 125:1135
  0x04, 0x7A, 0x0E, 0x07,  // 126:1146
  0x04, 0x88, 0x0E, 0x07,  // 127:1160
  0x04, 0x96, 0x0E, 0x07,  // 128:1174
  0x04, 0xA4, 0x0E, 0x07,  // 129:1188
  0x04, 0xB2, 0x0E, 0x07,  // 130:1202
  0x04, 0xC0, 0x0E, 0x07,  // 131:1216
  0x04, 0xCE, 0x0E, 0x07,  // 132:1230
  0x04, 0xDC, 0x0E, 0x07,  // 133:1244
  0x04, 0xEA, 0x0E, 0x07,  // 134:1258
  0x04, 0xF8, 0x0E, 0x07,  // 135:1272
  0x05, 0x06, 0x0E, 0x07,  // 136:1286
  0x05, 0x14, 0x0E, 0x07,  // 137:1300
  0x05, 0x22, 0x0E, 0x07,  // 138:1314
  0x05, 0x30, 0x0E, 0x07,  // 139:1328
  0x05, 0x3E, 0x0E, 0x07,  // 140:1342
  0x05, 0x4C, 0x0E, 0x07,  // 141:1356
  0x05, 0x5A, 0x0E, 0x07,  // 142:1370
  0x05, 0x68, 0x0E, 0x07,  // 143:1384
  0x05, 0x76, 0x0E, 0x07,  // 144:1398
  0x05, 0x84, 0x0E, 0x07,  // 145:1412
  0x05, 0x92, 0x0E, 0x07,  // 146:1426
  0x05, 0xA0, 0x0E, 0x07,  // 147:1440
  0x05, 0xAE, 0x0E, 0x07,  // 148:1454
  0x05, 0xBC, 0x0E, 0x07,  // 149:1468
  0x05, 0xCA, 0x0E, 0x07,  // 150:1482
  0x05, 0xD8, 0x0E, 0x07,  // 151:1496
  0x05, 0xE6, 0x0E, 0x07,  // 152:1510
  0x05, 0xF4, 0x0E, 0x07,  // 153:1524
  0x06, 0x02, 0x0E, 0x07,  // 154:1538
  0x06, 0x10, 0x0E, 0x07,  // 155:1552
  0x06, 0x1E, 0x0E, 0x07,  // 156:1566
  0x06, 0x2C, 0x0E, 0x07,  // 157:1580
  0x06, 0x3A, 0x0E, 0x07,  // 158:1594
  0x06, 0x48, 0x0E, 0x07,  // 159:1608
  0xFF, 0xFF, 0x00, 0x07,  // 160:65535
  0x06, 0x56, 0x08, 0x07,  // 161:1622
  0x06, 0x5E, 0x0C, 0x07,  // 162:1630
  0x06, 0x6A, 0x0C, 0x07,  // 163:1642
  0x06, 0x76, 0x0E, 0x07,  // 164:1654
  0x06, 0x84, 0x0D, 0x07,  // 165:1668
  0x06, 0x91, 0x08, 0x07,  // 166:1681
  0x06, 0x99, 0x0C, 0x07,  // 167:1689
  0x06, 0xA5, 0x09, 0x07,  // 168:1701
  0x06, 0xAE, 0x0E, 0x07,  // 169:1710
  0x06, 0xBC, 0x0A, 0x07,  // 170:1724
  0x06, 0xC6, 0x0E, 0x07,  // 171:1734
  0x06, 0xD4, 0x0E, 0x07,  // 172:1748
  0x06, 0xE2, 0x0A, 0x07,  // 173:1762
  0x06, 0xEC, 0x0E, 0x07,  // 174:1772
  0x06, 0xFA, 0x0B, 0x07,  // 175:1786
  0x07, 0x05, 0x0B, 0x07,  // 176:1797
  0x07, 0x10, 0x0E, 0x07,  // 177:1808
  0x07, 0x1E, 0x09, 0x07,  // 178:1822
  0x07, 0x27, 0x0B, 0x07,  // 179:1831
  0x07, 0x32, 0x09, 0x07,  // 180:1842
  0x07, 0x3B, 0x0E, 0x07,  // 181:1851
  0x07, 0x49, 0x0E, 0x07,  // 182:1865
  0x07, 0x57, 0x08, 0x07,  // 183:1879
  0x07, 0x5F, 0x0A, 0x07,  // 184:1887
  0x07, 0x69, 0x09, 0x07,  // 185:1897
  0x07, 0x72, 0x0A, 0x07,  // 186:1906
  0x07, 0x7C, 0x0E, 0x07,  // 187:1916
  0x07, 0x8A, 0x0E, 0x07,  // 188:1930
  0x07, 0x98, 0x0C, 0x07,  // 189:1944
  0x07, 0xA4, 0x0E, 0x07,  // 190:1956
  0x07, 0xB2, 0x0A, 0x07,  // 191:1970
  0x07, 0xBC, 0x0E, 0x07,  // 192:1980
  0x07, 0xCA, 0x0E, 0x07,  // 193:1994
  0x07, 0xD8, 0x0E, 0x07,  // 194:2008
  0x07, 0xE6, 0x0E, 0x07,  // 195:2022
  0x07, 0xF4, 0x0E, 0x07,  // 196:2036
  0x08, 0x02, 0x0E, 0x07,  // 197:2050
  0x08, 0x10, 0x0E, 0x07,  // 198:2064
  0x08, 0x1E, 0x0E, 0x07,  // 199:2078
  0x08, 0x2C, 0x0E, 0x07,  // 200:2092
  0x08, 0x3A, 0x0E, 0x07,  // 201:2106
  0x08, 0x48, 0x0E, 0x07,  // 202:2120
  0x08, 0x56, 0x0E, 0x07,  // 203:2134
  0x08, 0x64, 0x0C, 0x07,  // 204:2148
  0x08, 0x70, 0x0C, 0x07,  // 205:2160
  0x08, 0x7C, 0x0C, 0x07,  // 206:2172
  0x08, 0x88, 0x0C, 0x07,  // 207:2184
  0x08, 0x94, 0x0E, 0x07,  // 208:2196
  0x08, 0xA2, 0x0E, 0x07,  // 209:2210
  0x08, 0xB0, 0x0E, 0x07,  // 210:2224
  0x08, 0xBE, 0x0E, 0x07,  // 211:2238
  0x08, 0xCC, 0x0E, 0x07,  // 212:2252
  0x08, 0xDA, 0x0E, 0x07,  // 213:2266
  0x08, 0xE8, 0x0E, 0x07,  // 214:2280
  0x08, 0xF6, 0x0C, 0x07,  // 215:2294
  0x09, 0x02, 0x0E, 0x07,  // 216:2306
  0x09, 0x10, 0x0E, 0x07,  // 217:2320
  0x09, 0x1E, 0x0E, 0x07,  // 218:2334
  0x09, 0x2C, 0x0E, 0x07,  // 219:2348
  0x09, 0x3A, 0x0E, 0x07,  // 220:2362
  0x09, 0x48, 0x0D, 0x07,  // 221:2376
  0x09, 0x55, 0x0D, 0x07,  // 222:2389
  0x09, 0x62, 0x0C, 0x07,  // 223:2402
  0x09, 0x6E, 0x0C, 0x07,  // 224:2414
  0x09, 0x7A, 0x0C, 0x07,  // 225:2426
  0x09, 0x86, 0x0C, 0x07,  // 226:2438
  0x09, 0x92, 0x0C, 0x07,  // 227:2450
  0x09, 0x9E, 0x0C, 0x07,  // 228:2462
  0x09, 0xAA, 0x0C, 0x07,  // 229:2474
  0x09, 0xB6, 0x0C, 0x07,  // 230:2486
  0x09, 0xC2, 0x0C, 0x07,  // 231:2498
  0x09, 0xCE, 0x0C, 0x07,  // 232:2510
  0x09, 0xDA, 0x0C, 0x07,  // 233:2522
  0x09, 0xE6, 0x0C, 0x07,  // 234:2534
  0x09, 0xF2, 0x0C, 0x07,  // 235:2546
  0x09, 0xFE, 0x0C, 0x07,  // 236:2558
  0x0A, 0x0A, 0x0C, 0x07,  // 237:2570
  0x0A, 0x16, 0x0C, 0x07,  // 238:2582
  0x0A, 0x22, 0x0C, 0x07,  // 239:2594
  0x0A, 0x2E, 0x0C, 0x07,  // 240:2606
  0x0A, 0x3A, 0x0C, 0x07,  // 241:2618
  0x0A, 0x46, 0x0C, 0x07,  // 242:2630
  0x0A, 0x52, 0x0C, 0x07,  // 243:2642
  0x0A, 0x5E, 0x0C, 0x07,  // 244:2654
  0x0A, 0x6A, 0x0C, 0x07,  // 245:2666
  0x0A, 0x76, 0x0C, 0x07,  // 246:2678
  0x0A, 0x82, 0x0C, 0x07,  // 247:2690
  0x0A, 0x8E, 0x0C, 0x07,  // 248:2702
  0x0A, 0x9A, 0x0C, 0x07,  // 249:2714
  0x0A, 0xA6, 0x0C, 0x07,  // 250:2726
  0x0A, 0xB2, 0x0C, 0x07,  // 251:2738
  0x0A, 0xBE, 0x0C, 0x07,  // 252:2750
  0x0A, 0xCA, 0x0B, 0x07,  // 253:2762
  0x0A, 0xD5, 0x0C, 0x07,  // 254:2773
  0x0A, 0xE1, 0x0B, 0x07,  // 255:2785

  // Font Data:
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0D, // 33
  0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x38, // 34
  0x00, 0x02, 0x40, 0x0E, 0xE0, 0x03, 0x50, 0x0A, 0xC0, 0x07, 0x70, 0x02, 0x40, // 35
  0x00, 0x00, 0xE0, 0x04, 0x90, 0x08, 0xF8, 0x3F, 0x10, 0x09, 0x20, 0x07, // 36
  0x30, 0x00, 0x48, 0x01, 0x48, 0x01, 0xB0, 0x06, 0x80, 0x09, 0x40, 0x09, 0x00, 0x06, // 37
  0x00, 0x00, 0x00, 0x07, 0xF0, 0x0C, 0xC8, 0x08, 0x08, 0x0B, 0x08, 0x06, 0x00, 0x0B, // 38
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, // 39
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 0x1C, 0x1C, 0x04, 0x10, // 40
  0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 0x1C, 0x1C, 0xE0, 0x03, // 41
  0x00, 0x00, 0x90, 0x00, 0x60, 0x00, 0xF8, 0x01, 0x60, 0x00, 0x90, // 42
  0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0xE0, 0x0F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, // 43
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0C, // 44
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, // 45
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, // 46
  0x00, 0x00, 0x00, 0x10, 0x00, 0x0C, 0x00, 0x03, 0xC0, 0x00, 0x30, 0x00, 0x08, // 47
  0x00, 0x00, 0xE0, 0x03, 0x18, 0x0C, 0x08, 0x08, 0x88, 0x08, 0x18, 0x0C, 0xE0, 0x03, // 48
  0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x0F, 0x00, 0x08, 0x00, 0x08, // 49
  0x00, 0x00, 0x10, 0x08, 0x08, 0x0C, 0x08, 0x0A, 0x08, 0x09, 0x88, 0x08, 0x70, 0x08, // 50
  0x00, 0x00, 0x10, 0x04, 0x08, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x70, 0x07, // 51
  0x00, 0x00, 0x00, 0x03, 0xC0, 0x02, 0x60, 0x02, 0x18, 0x02, 0xF8, 0x0F, 0x00, 0x02, // 52
  0x00, 0x00, 0x78, 0x04, 0x48, 0x08, 0x48, 0x08, 0x48, 0x08, 0xC8, 0x0C, 0x80, 0x07, // 53
  0x00, 0x00, 0xE0, 0x03, 0x90, 0x0C, 0x48, 0x08, 0x48, 0x08, 0xC8, 0x0C, 0x90, 0x07, // 54
  0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x06, 0x88, 0x01, 0x78, 0x00, 0x18, // 55
  0x00, 0x00, 0x70, 0x07, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x70, 0x07, // 56
  0x00, 0x00, 0xF0, 0x04, 0x18, 0x09, 0x08, 0x09, 0x08, 0x09, 0x98, 0x04, 0xE0, 0x03, // 57
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, // 58
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xC0, 0x0C, // 59
  0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x20, 0x04, // 60
  0x00, 0x00, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, // 61
  0x00, 0x00, 0x20, 0x04, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x80, 0x01, 0x80, 0x01, // 62
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x88, 0x0D, 0xC8, 0x00, 0x48, 0x00, 0x30, // 63
  0x00, 0x00, 0xC0, 0x0F, 0x20, 0x18, 0x10, 0x23, 0x90, 0x24, 0xB0, 0x24, 0xE0, 0x07, // 64
  0x00, 0x00, 0x00, 0x0C, 0xC0, 0x03, 0x38, 0x02, 0x38, 0x02, 0xC0, 0x03, 0x00, 0x0C, // 65
  0x00, 0x00, 0xF8, 0x0F, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x70, 0x07, // 66
  0x00, 0x00, 0xE0, 0x03, 0x10, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, // 67
  0x00, 0x00, 0xF8, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0xE0, 0x03, // 68
  0x00, 0x00, 0xF8, 0x0F, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, // 69
  0x00, 0x00, 0xF8, 0x0F, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, // 70
  0x00, 0x00, 0xE0, 0x03, 0x10, 0x04, 0x08, 0x08, 0x08, 0x08, 0x88, 0x08, 0x90, 0x07, // 71
  0x00, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xF8, 0x0F, // 72
  0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x0F, 0x08, 0x08, 0x08, 0x08, // 73
  0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x07, // 74
  0x00, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0xC0, 0x00, 0x20, 0x03, 0x10, 0x06, 0x08, 0x08, // 75
  0x00, 0x00, 0xF8, 0x0F, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, // 76
  0x00, 0x00, 0xF8, 0x0F, 0x30, 0x00, 0xC0, 0x01, 0xC0, 0x01, 0x30, 0x00, 0xF8, 0x0F, // 77
  0x00, 0x00, 0xF8, 0x0F, 0x18, 0x00, 0xE0, 0x00, 0x80, 0x03, 0x00, 0x0C, 0xF8, 0x0F, // 78
  0x00, 0x00, 0xE0, 0x03, 0x18, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x18, 0x0C, 0xE0, 0x03, // 79
  0x00, 0x00, 0xF8, 0x0F, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x70, // 80
  0x00, 0x00, 0xE0, 0x03, 0x18, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x18, 0x3C, 0xE0, 0x07, // 81
  0x00, 0x00, 0xF8, 0x0F, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x01, 0x70, 0x06, // 82
  0x00, 0x00, 0x70, 0x04, 0xC8, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x10, 0x07, // 83
  0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0xF8, 0x0F, 0x08, 0x00, 0x08, 0x00, 0x08, // 84
  0x00, 0x00, 0xF8, 0x07, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0xF8, 0x07, // 85
  0x00, 0x00, 0x18, 0x00, 0xE0, 0x01, 0x00, 0x0E, 0x00, 0x0E, 0xE0, 0x01, 0x18, // 86
  0xF8, 0x01, 0x00, 0x0E, 0xC0, 0x03, 0x30, 0x00, 0xC0, 0x03, 0x00, 0x0E, 0xF8, 0x01, // 87
  0x00, 0x00, 0x08, 0x08, 0x30, 0x06, 0xC0, 0x01, 0xC0, 0x01, 0x30, 0x06, 0x08, 0x08, // 88
  0x08, 0x00, 0x10, 0x00, 0x60, 0x00, 0x80, 0x0F, 0x60, 0x00, 0x10, 0x00, 0x08, // 89
  0x00, 0x00, 0x08, 0x0C, 0x08, 0x0E, 0x88, 0x09, 0xC8, 0x08, 0x38, 0x08, 0x18, 0x08, // 90
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F, 0x04, 0x10, // 91
  0x00, 0x00, 0x08, 0x00, 0x30, 0x00, 0xC0, 0x00, 0x00, 0x03, 0x00, 0x0C, 0x00, 0x10, // 92
  0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 0xFC, 0x1F, // 93
  0x20, 0x00, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0x10, 0x00, 0x20, // 94
  0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, // 95
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, // 96
  0x00, 0x00, 0x40, 0x06, 0x20, 0x09, 0x20, 0x09, 0x20, 0x09, 0xC0, 0x0F, // 97
  0x00, 0x00, 0xFC, 0x0F, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0xC0, 0x07, // 98
  0x00, 0x00, 0xC0, 0x07, 0x60, 0x0C, 0x20, 0x08, 0x20, 0x08, 0x40, 0x08, // 99
  0x00, 0x00, 0xC0, 0x07, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0xFC, 0x0F, // 100
  0x00, 0x00, 0xC0, 0x07, 0x60, 0x09, 0x20, 0x09, 0x20, 0x09, 0xC0, 0x05, // 101
  0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0xF8, 0x0F, 0x24, 0x00, 0x24, // 102
  0x00, 0x00, 0xC0, 0x07, 0x20, 0x28, 0x20, 0x48, 0x20, 0x48, 0xE0, 0x3F, // 103
  0x00, 0x00, 0xFC, 0x0F, 0x40, 0x00, 0x20, 0x00, 0x20, 0x00, 0xC0, 0x0F, // 104
  0x00, 0x00, 0x20, 0x08, 0x20, 0x08, 0xE4, 0x0F, 0x00, 0x08, 0x00, 0x08, // 105
  0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x20, 0x40, 0xE4, 0x3F, // 106
  0x00, 0x00, 0xFC, 0x0F, 0x00, 0x01, 0x80, 0x02, 0x40, 0x04, 0x20, 0x08, // 107
  0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFC, 0x07, 0x00, 0x08, 0x00, 0x08, // 108
  0x00, 0x00, 0xE0, 0x0F, 0x20, 0x00, 0xE0, 0x0F, 0x20, 0x00, 0xE0, 0x0F, // 109
  0x00, 0x00, 0xE0, 0x0F, 0x40, 0x00, 0x20, 0x00, 0x20, 0x00, 0xC0, 0x0F, // 110
  0x00, 0x00, 0xC0, 0x07, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0xC0, 0x07, // 111
  0x00, 0x00, 0xE0, 0x7F, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0xC0, 0x07, // 112
  0x00, 0x00, 0xC0, 0x07, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0xE0, 0x7F, // 113
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0x60, 0x00, 0x20, 0x00, 0x20, 0x00, 0x40, // 114
  0x00, 0x00, 0xC0, 0x04, 0x20, 0x09, 0x20, 0x09, 0x20, 0x09, 0x40, 0x06, // 115
  0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0xF8, 0x0F, 0x20, 0x08, 0x20, 0x08, // 116
  0x00, 0x00, 0xE0, 0x07, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0xE0, 0x0F, // 117
  0x00, 0x00, 0x60, 0x00, 0x80, 0x03, 0x00, 0x0C, 0x80, 0x03, 0x60, // 118
  0x60, 0x00, 0x80, 0x03, 0x00, 0x0E, 0x80, 0x01, 0x00, 0x0E, 0x80, 0x03, 0x60, // 119
  0x00, 0x00, 0x20, 0x08, 0xC0, 0x06, 0x00, 0x01, 0xC0, 0x06, 0x20, 0x08, // 120
  0x00, 0x00, 0x60, 0x40, 0x80, 0x67, 0x00, 0x1C, 0x80, 0x03, 0x60, // 121
  0x00, 0x00, 0x20, 0x0C, 0x20, 0x0A, 0x20, 0x09, 0xA0, 0x08, 0x60, 0x08, // 122
  0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x7C, 0x1F, 0x04, 0x10, 0x04, 0x10, // 123
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x3F, // 124
  0x00, 0x00, 0x04, 0x10, 0x04, 0x10, 0x7C, 0x1F, 0x80, 0x00, 0x80, // 125
  0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, // 126
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 127
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 128
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 129
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 130
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 131
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 132
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 133
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 134
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 135
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 136
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 137
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 138
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 139
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 140
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 141
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 142
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 143
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 144
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 145
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 146
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 147
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 148
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 149
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 150
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 151
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 152
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 153
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 154
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 155
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 156
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 157
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 158
  0x00, 0x00, 0xF0, 0x7F, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xF0, 0x7F, // 159
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x3F, // 161
  0x00, 0x00, 0xC0, 0x07, 0x20, 0x08, 0xF8, 0x3F, 0x20, 0x08, 0x40, 0x04, // 162
  0x00, 0x00, 0x80, 0x08, 0xF0, 0x0F, 0x88, 0x08, 0x88, 0x08, 0x08, 0x08, // 163
  0x00, 0x00, 0x20, 0x04, 0xC0, 0x03, 0x40, 0x02, 0x40, 0x02, 0xC0, 0x03, 0x20, 0x04, // 164
  0x08, 0x00, 0x50, 0x01, 0x60, 0x01, 0x80, 0x0F, 0x60, 0x01, 0x50, 0x01, 0x08, // 165
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3C, // 166
  0x00, 0x00, 0xF0, 0x11, 0x28, 0x13, 0x48, 0x12, 0xC8, 0x14, 0x88, 0x0F, // 167
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, // 168
  0xC0, 0x01, 0x20, 0x02, 0xD0, 0x05, 0x50, 0x05, 0x50, 0x05, 0x20, 0x02, 0xC0, 0x01, // 169
  0x00, 0x00, 0xE8, 0x02, 0xA8, 0x02, 0xA8, 0x02, 0xF0, 0x02, // 170
  0x00, 0x00, 0x00, 0x01, 0x80, 0x02, 0x40, 0x04, 0x00, 0x01, 0x80, 0x02, 0x40, 0x04, // 171
  0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x03, // 172
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, // 173
  0xC0, 0x01, 0x20, 0x02, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0x20, 0x02, 0xC0, 0x01, // 174
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, // 175
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x48, 0x00, 0x48, 0x00, 0x30, // 176
  0x80, 0x08, 0x80, 0x08, 0x80, 0x08, 0xE0, 0x0B, 0x80, 0x08, 0x80, 0x08, 0x80, 0x08, // 177
  0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0xE8, 0x00, 0xB8, // 178
  0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0xA8, 0x00, 0xA8, 0x00, 0xD8, // 179
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, // 180
  0x00, 0x00, 0xE0, 0x7F, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0xE0, 0x0F, 0x00, 0x08, // 181
  0x00, 0x00, 0x70, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0xF8, 0x1F, 0x08, 0x00, 0xF8, 0x1F, // 182
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, // 183
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0x30, // 184
  0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0xF8, 0x00, 0x80, // 185
  0x00, 0x00, 0x70, 0x02, 0x88, 0x02, 0x88, 0x02, 0x70, 0x02, // 186
  0x00, 0x00, 0x40, 0x04, 0x80, 0x02, 0x00, 0x01, 0x40, 0x04, 0x80, 0x02, 0x00, 0x01, // 187
  0x44, 0x02, 0x7C, 0x03, 0x40, 0x01, 0x00, 0x0D, 0x80, 0x0B, 0x80, 0x1F, 0x00, 0x08, // 188
  0x44, 0x02, 0x7C, 0x03, 0x40, 0x01, 0x00, 0x11, 0x80, 0x1D, 0x80, 0x17, // 189
  0x00, 0x02, 0x44, 0x03, 0x54, 0x01, 0x54, 0x0D, 0xEC, 0x0B, 0x80, 0x1F, 0x00, 0x08, // 190
  0x00, 0x38, 0x00, 0x4C, 0x00, 0x44, 0x60, 0x43, 0x00, 0x20, // 191
  0x00, 0x00, 0x00, 0x0C, 0xC1, 0x03, 0x3A, 0x02, 0x38, 0x02, 0xC0, 0x03, 0x00, 0x0C, // 192
  0x00, 0x00, 0x00, 0x0C, 0xC0, 0x03, 0x3A, 0x02, 0x39, 0x02, 0xC0, 0x03, 0x00, 0x0C, // 193
  0x00, 0x00, 0x00, 0x0C, 0xC2, 0x03, 0x39, 0x02, 0x39, 0x02, 0xC2, 0x03, 0x00, 0x0C, // 194
  0x00, 0x00, 0x00, 0x0C, 0xC3, 0x03, 0x39, 0x02, 0x3A, 0x02, 0xC3, 0x03, 0x00, 0x0C, // 195
  0x00, 0x00, 0x00, 0x0C, 0xC2, 0x03, 0x38, 0x02, 0x38, 0x02, 0xC2, 0x03, 0x00, 0x0C, // 196
  0x00, 0x00, 0x00, 0x08, 0x00, 0x07, 0xFE, 0x02, 0xE6, 0x02, 0x00, 0x07, 0x00, 0x08, // 197
  0x00, 0x0C, 0x80, 0x03, 0x78, 0x02, 0x08, 0x02, 0xF8, 0x0F, 0x88, 0x08, 0x88, 0x08, // 198
  0x00, 0x00, 0xE0, 0x03, 0x10, 0x04, 0x08, 0x28, 0x08, 0x28, 0x08, 0x38, 0x10, 0x04, // 199
  0x00, 0x00, 0xF8, 0x0F, 0x89, 0x08, 0x8A, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, // 200
  0x00, 0x00, 0xF8, 0x0F, 0x88, 0x08, 0x8A, 0x08, 0x89, 0x08, 0x88, 0x08, 0x88, 0x08, // 201
  0x00, 0x00, 0xF8, 0x0F, 0x8A, 0x08, 0x89, 0x08, 0x89, 0x08, 0x8A, 0x08, 0x88, 0x08, // 202
  0x00, 0x00, 0xF8, 0x0F, 0x8A, 0x08, 0x88, 0x08, 0x8A, 0x08, 0x88, 0x08, 0x88, 0x08, // 203
  0x00, 0x00, 0x08, 0x08, 0x09, 0x08, 0xFA, 0x0F, 0x08, 0x08, 0x08, 0x08, // 204
  0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0xFA, 0x0F, 0x09, 0x08, 0x08, 0x08, // 205
  0x00, 0x00, 0x08, 0x08, 0x0A, 0x08, 0xF9, 0x0F, 0x0A, 0x08, 0x08, 0x08, // 206
  0x00, 0x00, 0x08, 0x08, 0x0A, 0x08, 0xF8, 0x0F, 0x0A, 0x08, 0x08, 0x08, // 207
  0x80, 0x00, 0xF8, 0x0F, 0x88, 0x08, 0x88, 0x08, 0x08, 0x08, 0x10, 0x04, 0xE0, 0x03, // 208
  0x00, 0x00, 0xF8, 0x0F, 0x1B, 0x00, 0xE1, 0x00, 0x82, 0x03, 0x03, 0x0C, 0xF8, 0x0F, // 209
  0x00, 0x00, 0xE0, 0x03, 0x19, 0x0C, 0x0A, 0x08, 0x08, 0x08, 0x18, 0x0C, 0xE0, 0x03, // 210
  0x00, 0x00, 0xE0, 0x03, 0x18, 0x0C, 0x0A, 0x08, 0x09, 0x08, 0x18, 0x0C, 0xE0, 0x03, // 211
  0x00, 0x00, 0xE0, 0x03, 0x1A, 0x0C, 0x09, 0x08, 0x09, 0x08, 0x1A, 0x0C, 0xE0, 0x03, // 212
  0x00, 0x00, 0xE0, 0x03, 0x1B, 0x0C, 0x09, 0x08, 0x0A, 0x08, 0x1B, 0x0C, 0xE0, 0x03, // 213
  0x00, 0x00, 0xE0, 0x03, 0x1A, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x1A, 0x0C, 0xE0, 0x03, // 214
  0x00, 0x00, 0x20, 0x02, 0x40, 0x01, 0x80, 0x00, 0x40, 0x01, 0x20, 0x02, // 215
  0x00, 0x08, 0xE0, 0x07, 0x18, 0x0E, 0x88, 0x09, 0x48, 0x08, 0x38, 0x0C, 0xF8, 0x03, // 216
  0x00, 0x00, 0xF8, 0x07, 0x01, 0x08, 0x02, 0x08, 0x00, 0x08, 0x00, 0x08, 0xF8, 0x07, // 217
  0x00, 0x00, 0xF8, 0x07, 0x00, 0x08, 0x02, 0x08, 0x01, 0x08, 0x00, 0x08, 0xF8, 0x07, // 218
  0x00, 0x00, 0xF8, 0x07, 0x02, 0x08, 0x01, 0x08, 0x01, 0x08, 0x02, 0x08, 0xF8, 0x07, // 219
  0x00, 0x00, 0xF8, 0x07, 0x02, 0x08, 0x00, 0x08, 0x00, 0x08, 0x02, 0x08, 0xF8, 0x07, // 220
  0x08, 0x00, 0x10, 0x00, 0x60, 0x00, 0x82, 0x0F, 0x61, 0x00, 0x10, 0x00, 0x08, // 221
  0x00, 0x00, 0xF8, 0x0F, 0x10, 0x01, 0x10, 0x01, 0x10, 0x01, 0x10, 0x01, 0xE0, // 222
  0x00, 0x00, 0xF8, 0x0F, 0xE4, 0x00, 0xA4, 0x09, 0x18, 0x09, 0x00, 0x06, // 223
  0x00, 0x00, 0x40, 0x06, 0x20, 0x09, 0x24, 0x09, 0x28, 0x09, 0xC0, 0x0F, // 224
  0x00, 0x00, 0x40, 0x06, 0x20, 0x09, 0x28, 0x09, 0x24, 0x09, 0xC0, 0x0F, // 225
  0x00, 0x00, 0x40, 0x06, 0x28, 0x09, 0x24, 0x09, 0x24, 0x09, 0xC8, 0x0F, // 226
  0x00, 0x00, 0x40, 0x06, 0x2C, 0x09, 0x24, 0x09, 0x28, 0x09, 0xCC, 0x0F, // 227
  0x00, 0x00, 0x40, 0x06, 0x28, 0x09, 0x20, 0x09, 0x28, 0x09, 0xC0, 0x0F, // 228
  0x00, 0x00, 0x40, 0x06, 0x26, 0x09, 0x29, 0x09, 0x29, 0x09, 0xC6, 0x0F, // 229
  0x00, 0x00, 0x40, 0x0F, 0x20, 0x09, 0xC0, 0x07, 0x20, 0x09, 0xE0, 0x09, // 230
  0x00, 0x00, 0xC0, 0x07, 0x60, 0x0C, 0x20, 0x28, 0x20, 0x28, 0x40, 0x38, // 231
  0x00, 0x00, 0xC0, 0x07, 0x60, 0x09, 0x24, 0x09, 0x28, 0x09, 0xC0, 0x05, // 232
  0x00, 0x00, 0xC0, 0x07, 0x60, 0x09, 0x28, 0x09, 0x24, 0x09, 0xC0, 0x05, // 233
  0x00, 0x00, 0xC0, 0x07, 0x68, 0x09, 0x24, 0x09, 0x24, 0x09, 0xC8, 0x05, // 234
  0x00, 0x00, 0xC0, 0x07, 0x68, 0x09, 0x20, 0x09, 0x28, 0x09, 0xC0, 0x05, // 235
  0x00, 0x00, 0x20, 0x08, 0x20, 0x08, 0xE4, 0x0F, 0x08, 0x08, 0x00, 0x08, // 236
  0x00, 0x00, 0x20, 0x08, 0x20, 0x08, 0xE8, 0x0F, 0x04, 0x08, 0x00, 0x08, // 237
  0x00, 0x00, 0x28, 0x08, 0x24, 0x08, 0xE4, 0x0F, 0x08, 0x08, 0x00, 0x08, // 238
  0x00, 0x00, 0x20, 0x08, 0x28, 0x08, 0xE0, 0x0F, 0x08, 0x08, 0x00, 0x08, // 239
  0x00, 0x00, 0x80, 0x07, 0x54, 0x08, 0x58, 0x08, 0x68, 0x08, 0xC0, 0x07, // 240
  0x00, 0x00, 0xE0, 0x0F, 0x4C, 0x00, 0x24, 0x00, 0x28, 0x00, 0xCC, 0x0F, // 241
  0x00, 0x00, 0xC0, 0x07, 0x20, 0x08, 0x24, 0x08, 0x28, 0x08, 0xC0, 0x07, // 242
  0x00, 0x00, 0xC0, 0x07, 0x20, 0x08, 0x28, 0x08, 0x24, 0x08, 0xC0, 0x07, // 243
  0x00, 0x00, 0xC0, 0x07, 0x28, 0x08, 0x24, 0x08, 0x28, 0x08, 0xC0, 0x07, // 244
  0x00, 0x00, 0xCC, 0x07, 0x24, 0x08, 0x2C, 0x08, 0x28, 0x08, 0xCC, 0x07, // 245
  0x00, 0x00, 0xC0, 0x07, 0x28, 0x08, 0x20, 0x08, 0x28, 0x08, 0xC0, 0x07, // 246
  0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x40, 0x05, 0x00, 0x01, 0x00, 0x01, // 247
  0x00, 0x00, 0xC0, 0x0F, 0x20, 0x0A, 0x20, 0x09, 0xA0, 0x08, 0xE0, 0x07, // 248
  0x00, 0x00, 0xE0, 0x07, 0x00, 0x08, 0x04, 0x08, 0x08, 0x08, 0xE0, 0x0F, // 249
  0x00, 0x00, 0xE0, 0x07, 0x00, 0x08, 0x08, 0x08, 0x04, 0x08, 0xE0, 0x0F, // 250
  0x00, 0x00, 0xE0, 0x07, 0x08, 0x08, 0x04, 0x08, 0x08, 0x08, 0xE0, 0x0F, // 251
  0x00, 0x00, 0xE0, 0x07, 0x08, 0x08, 0x00, 0x08, 0x08, 0x08, 0xE0, 0x0F, // 252
  0x00, 0x00, 0x60, 0x40, 0x80, 0x67, 0x08, 0x1C, 0x84, 0x03, 0x60, // 253
  0x00, 0x00, 0xFC, 0x7F, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0xC0, 0x07, // 254
  0x00, 0x00, 0x60, 0x40, 0x88, 0x67, 0x00, 0x1C, 0x88, 0x03, 0x60 // 255
};

#endif

