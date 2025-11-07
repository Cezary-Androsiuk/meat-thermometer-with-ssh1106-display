#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// https://github.com/olikraus/u8g2
// doc: https://github.com/olikraus/u8g2/wiki/u8g2reference

// handle displaying on controller SSH1106 
// https://pl.aliexpress.com/item/1005001920647851.html
class Display{

    static constexpr int DISPLAY_WIDTH = 128;
    static constexpr int DISPLAY_HEIGHT = 64;

    // fonts: https://github.com/olikraus/u8g2/wiki/fntlistall
    // u8g2_font_ncenB10_tr // 12px
    static constexpr uint8_t *FONT = u8g2_font_samim_16_t_all ; // 12px height font
    static constexpr int FONT_SIZE = 12;

public:
    static void initialize()
    {
        u8g2.begin();  
        u8g2.setFont(FONT);
    }

    static void printTemp(int adcValue, double tempValue)
    {
        String adcString = "ADC: " + String(adcValue);
        String tempString = String(tempValue) + " *C";

        u8g2.firstPage();
        do
        {
            u8g2.drawLine(2, 2, DISPLAY_WIDTH-2, 2); // top line
            u8g2.drawLine(2, DISPLAY_HEIGHT-2, DISPLAY_WIDTH-2, DISPLAY_HEIGHT-2); // bottom line
            u8g2.drawLine(2, 2, 2, DISPLAY_HEIGHT-2); // left line
            u8g2.drawLine(DISPLAY_WIDTH-2, 2, DISPLAY_WIDTH-2, DISPLAY_HEIGHT-2); // right line

            // u8g2.setFont(u8g2_font_t0_16_mn);
            u8g2.drawStr(12, 12+FONT_SIZE, adcString.c_str());
            u8g2.drawStr(12, DISPLAY_HEIGHT-12, tempString.c_str());
        } 
        while ( u8g2.nextPage() );
    }
};