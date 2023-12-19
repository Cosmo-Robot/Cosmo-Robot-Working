#include "levelManage.h"

////////////////////////////// Managing Levels ///////////////////////////////////////////
void levelManage() {
    if (squareDetected()) {
        checkpoint();
    }

    // Level selection based on pointer value
    switch (pointer) {
        case 0:
            level1(); // LINE FOLLOWING
            break;
        case 1:
            level2(); // WALL FOLLOWING
            break;
        case 2:
            level3(); // RAMP
            break;
        case 3:
            level4(); // PULL the Box
            break;
        case 4:
            level5(); // Color FOLLOWING
            break;
        case 5:
            level6(); // Sound Game
            break;
        case 6:
            level7(); // GUARD AVOIDING
            break;
        default:
            Switch = false; // Safety stop for an invalid task
            break;
    }
}

///////////////////////////// Mode Selection ////////////////////////////////////////////
void modeSelect() {
    delay(1000); // Wait for the sensors to be ready
    int POT_value = 0;

    while (digitalRead(calibration) == LOW) {
        if (digitalRead(12) == HIGH) {
            delay(600);
            POT_value = (POT_value + 1) % 8;
            while (digitalRead(12) == HIGH) {}
        }

        // Display mode on screen based on POT_value
        const char* modeText = "";
        switch (POT_value) {
            case 0:
                modeText = "LINE";
                break;
            case 1:
                modeText = "WALL";
                break;
            case 2:
                modeText = "RAMP";
                break;
            case 3:
                modeText = "PULL";
                break;
            case 4:
                modeText = "COLOR";
                break;
            case 5:
                modeText = "SOUND";
                break;
            case 6:
                modeText = "GUARD";
                break;
            case 7:
                modeText = "HALT";
                break;
            default:
                modeText = "NOT DEF";
                break;
        }

        // Display selected mode on the screen
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, modeText);
        u8g2.sendBuffer();
    }

    pointer = POT_value;

    // Display mode confirmation on the screen
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "ENTERED THE MODE");
    u8g2.sendBuffer();

    delay(1000);
}
