#include "Arduino.h"

#ifndef LED_PIN
#define LED_PIN 13
#endif

#include <hidboot.h>
#include <usbhub.h>

class MouseRptParser : public MouseReportParser {
 protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
};

void MouseRptParser::OnMouseMove(MOUSEINFO *mi) {
Serial.print("dx=");
Serial.print(mi->dX, DEC);
Serial.print(" dy=");
Serial.println(mi->dY, DEC);
}

void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi) {
Serial.println("L Butt Up");
}

void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi) {
Serial.println("L Butt Dn");
}

void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi) {
Serial.println("R Butt Up");
}

void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi) {
Serial.println("R Butt Dn");
}

void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi) {
Serial.println("M Butt Up");
}

void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi) {
Serial.println("M Butt Dn");
}

USB Usb;
USBHub Hub(&Usb);
HIDBoot<HID_PROTOCOL_MOUSE> HidMouse(&Usb);
uint32_t next_time;
MouseRptParser Prs;

void setup() {
    /*
     * Reset of the usb host shield is on pin 7,
     * if we don't do anything there (e.g. output 0)
     * the board won't work.
     */
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);
    Serial.begin(38400);
    Serial.println("Start");
    if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
    delay(200);
    next_time = millis() + 5000;
    HidMouse.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop() {
    Usb.Task();
}
