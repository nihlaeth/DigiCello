/*
 * Copyright 2015 Nihlaeth
 */
#include "Arduino.h"

#ifndef LED_PIN
#define LED_PIN 13
#endif

// Revision 1.2 (DEV-09628)
#define MAX_RESET 8 // MAX3421E pin 12
#define MAX_GPX   7 // MAX3421E pin 17
// Revision 1.3 (DEV-09947)
#define MAX_RESET 7 // MAX3421E pin 12
#define MAX_GPX   8 // MAX3421E pin 17


#include <hid.h>
#include <hiduniversal.h>
#include <hidescriptorparser.h>

#include <hidboot.h>
#include <usbhub.h>

#include "./pgmstrings.h"

class HIDUniversal2 : public HIDUniversal {
 public:
    HIDUniversal2(USB *usb) : HIDUniversal(usb) {}
 protected:
    uint8_t OnInitSuccessful();
};

uint8_t HIDUniversal2::OnInitSuccessful() {
    uint8_t rcode;
    HexDumper<USBReadParser, uint16_t, uint16_t> Hex;
    ReportDescParser Rpt;
    if ((rcode = GetReportDescr(0, &Hex)))
        goto FailGetReportDescr1;
    if ((rcode = GetReportDescr(0, &Rpt)))
        goto FailGetReportDescr2;
    return 0;
FailGetReportDescr1:
    USBTRACE("GetReportDescr1:");
    goto Fail;
FailGetReportDescr2:
    USBTRACE("GetReportDescr2:");
    goto Fail;
Fail:
    Serial.println(rcode, HEX);
    Release();
    return rcode;
}

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

HIDUniversal2 Hid(&Usb);
UniversalReportParser Uni;

void setup() {
    /*
     * Reset of the usb host shield is on pin 7,
     * if we don't do anything there (e.g. output 0)
     * the board won't work.
     */
    pinMode(MAX_GPX, INPUT);
    pinMode(MAX_RESET, OUTPUT);
    digitalWrite(MAX_RESET, LOW);
    delay(20); // wait 20ms
    digitalWrite(MAX_RESET, HIGH);
    delay(20); // wait 20ms
    Serial.begin(38400);
    Serial.println("Start");
    if (Usb.Init() == -1) {
        Serial.println("OSC did not start.");
    } else {
        Serial.println("OSC started.");
    }
    delay(200);
    next_time = millis() + 5000;
    HidMouse.SetReportParser(0, (HIDReportParser*)&Prs);

    if (!Hid.SetReportParser(0, &Uni))
        ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1 );
}

void loop() {
    Usb.Task();
}
