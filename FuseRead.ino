#include <avr/boot.h>

void print_val(char *msg, uint8_t val)
{
    Serial.print(msg);
    Serial.println(val, HEX);
}

void setup(void)
{
    Serial.begin(115200);
    while (!Serial) ;

    if(boot_lock_fuse_bits_get(1) == 0xFF) {
      Serial.println("LOCK BITS FUSE IS ... CORRECT");
    } else {
      Serial.println("LOCK BITS FUSE IS ... WRONG");
    }

    if(boot_lock_fuse_bits_get(2) == 0xFC) {
      Serial.println("EXT FUSE IS       ... CORRECT");
    } else {
      Serial.println("EXT FUSE IS       ... WRONG");
    }

    if(boot_lock_fuse_bits_get(3) == 0xDA) {
      Serial.println("HIGH FUSE IS      ... CORRECT");
    } else {
      Serial.println("HIGH FUSE IS      ... WRONG");
    }

    if(boot_lock_fuse_bits_get(0) == 0xF7) {
      Serial.println("LOW FUSE IS       ... CORRECT");
    } else {
      Serial.println("LOW FUSE IS       ... WRONG");
    }
    Serial.println();
    Serial.println();
            
    print_val("lockb = 0x", boot_lock_fuse_bits_get(1));
    print_val("ext fuse = 0x", boot_lock_fuse_bits_get(2));
    print_val("high fuse = 0x", boot_lock_fuse_bits_get(3));
    print_val("low fuse = 0x", boot_lock_fuse_bits_get(0));
#define SIGRD 5
#if defined(SIGRD) || defined(RSIG)
    Serial.print("Signature : ");
    for (uint8_t i = 0; i < 5; i += 2) {
        Serial.print(" 0x");
        Serial.print(boot_signature_byte_get(i), HEX);
    }
    Serial.println();
#endif
    Serial.print("Serial Number : ");
    for (uint8_t i = 14; i < 24; i += 1) {
        Serial.print(" 0x");
        Serial.print(boot_signature_byte_get(i), HEX);
    }

    Serial.println();
    Serial.println();
    Serial.println("LOW FUSE SUMMARY");
    if(~boot_lock_fuse_bits_get(0) & 0x80) {
      Serial.println("Divide clock by 8 internally");
    }
    if(~boot_lock_fuse_bits_get(0) & 0x40) {
      Serial.println("Clock output on PORTB1");
    }
    uint8_t cksel = 0x0F & boot_lock_fuse_bits_get(0);
    uint8_t sut = 0;
    if((boot_lock_fuse_bits_get(0) & 0x10) != 0) {
      sut |= 0x01;
    }
    if((boot_lock_fuse_bits_get(0) & 0x20) != 0) {
      sut |= 0x02;
    }
    if((boot_lock_fuse_bits_get(0) & 0x01) != 0) {
      sut |= 0x04;
    }
    
    if(cksel >= 0b1000) {
      Serial.print("Low Power Crystal Oscillator: ");
    } else if(cksel >= 0b0110) {
      Serial.print("Full Swing Crystal Oscillator: ");
    } else if(cksel >= 0b0100) {
      Serial.print("Low Frequency Crystal Oscillator: ");
    } else if(cksel == 0b0011) {
      Serial.print("Internal 128kHz RC Oscillator: ");
    } else if(cksel == 0b0010) {
      Serial.print("Calibrated Internal RC Oscillator: ");
    } else if(cksel == 0b0000) {
      Serial.print("External Clock: ");
    } else {
      Serial.print("Undefined: ");
    }

    switch(sut) {
      case 0: Serial.print("14CK + 4.1ms"); break;
      case 1: Serial.print("14CK + 65ms"); break;
      case 2: Serial.print("14CK"); break;
      case 3: Serial.print("14CK + 4.1ms"); break;
      case 4: Serial.print("14CK + 65ms ("); break;
      case 5: Serial.print("14CK"); break;
      case 6: Serial.print("14CK + 4.1ms"); break;
      case 7: Serial.print("14CK + 65ms"); break;
      default: Serial.print("Undefined");
    }
    Serial.println();
    Serial.println();
    
    Serial.println("HIGH FUSE SUMMARY");
    uint8_t bootsize = boot_lock_fuse_bits_get(3) & 0x07;
    if((bootsize & 1) == 0) {
      bootsize >>= 1;
      switch(bootsize & 3) {
        case 0: Serial.println("4096 word bootsector"); break;
        case 1: Serial.println("2048 word bootsector"); break;
        case 2: Serial.println("1024 word bootsector"); break;
        case 3: Serial.println("512 word bootsector"); break;
      }
    } else {
      Serial.println("BOOT RESET VECTOR DISABLED");
    }
    Serial.println();
    Serial.println();
    Serial.println("EXT FUSE SUMMARY");
    uint8_t bodlevel = boot_lock_fuse_bits_get(2) & 0x07;
    switch(bodlevel) {
        case 7: Serial.println("BOD Disabled"); break;
        case 6: Serial.println("BOD 1.8V"); break;
        case 5: Serial.println("BOD 2.7V"); break;
        case 4: Serial.println("BOD 4.3V"); break;     
        default: Serial.println("Undefined BODLEVEL");
    }
    Serial.println();
    Serial.println();
    
    Serial.println("LOW DETAIL");
    if(~boot_lock_fuse_bits_get(0) & 0x80) {
      Serial.println("CKDIV8");
    }
    if(~boot_lock_fuse_bits_get(0) & 0x40) {
      Serial.println("CKOUT");
    }
    if(~boot_lock_fuse_bits_get(0) & 0x20) {
      Serial.println("SUT1");
    }
    if(~boot_lock_fuse_bits_get(0) & 0x10) {
      Serial.println("SUT0");
    }
    if(~boot_lock_fuse_bits_get(0) & 0x08) {
      Serial.println("CKSEL3");
    }
    if(~boot_lock_fuse_bits_get(0) & 0x04) {
      Serial.println("CKSEL2");
    }    
    if(~boot_lock_fuse_bits_get(0) & 0x02) {
      Serial.println("CKSEL1");
    }        
    if(~boot_lock_fuse_bits_get(0) & 0x01) {
      Serial.println("CKSEL0");
    }    

    Serial.println();
    Serial.println("EXT DETAIL");
    if(~boot_lock_fuse_bits_get(2) & 0x80) {
      Serial.println("Bit 7");
    }
    if(~boot_lock_fuse_bits_get(2) & 0x40) {
      Serial.println("Bit 6");
    }
    if(~boot_lock_fuse_bits_get(2) & 0x20) {
      Serial.println("Bit 5");
    }
    if(~boot_lock_fuse_bits_get(2) & 0x10) {
      Serial.println("Bit 4");
    }
    if(~boot_lock_fuse_bits_get(2) & 0x08) {
      Serial.println("Bit 3");
    }
    if(~boot_lock_fuse_bits_get(2) & 0x04) {
      Serial.println("BODLEVEL2");
    }    
    if(~boot_lock_fuse_bits_get(2) & 0x02) {
      Serial.println("BODLEVEL1");
    }        
    if(~boot_lock_fuse_bits_get(2) & 0x01) {
      Serial.println("BODLEVEL0");
    }     

    Serial.println();
    Serial.println("HIGH DETAIL");
    if(~boot_lock_fuse_bits_get(3) & 0x80) {
      Serial.println("OCDEN");
    }
    if(~boot_lock_fuse_bits_get(3) & 0x40) {
      Serial.println("JTAGEN");
    }
    if(~boot_lock_fuse_bits_get(3) & 0x20) {
      Serial.println("SPIEN");
    }
    if(~boot_lock_fuse_bits_get(3) & 0x10) {
      Serial.println("WDTON");
    }
    if(~boot_lock_fuse_bits_get(3) & 0x08) {
      Serial.println("EESAVE");
    }
    if(~boot_lock_fuse_bits_get(3) & 0x04) {
      Serial.println("BOOTSZ1");
    }    
    if(~boot_lock_fuse_bits_get(3) & 0x02) {
      Serial.println("BOOTSZ0");
    }        
    if(~boot_lock_fuse_bits_get(3) & 0x01) {
      Serial.println("BOOTRST");
    }                 
}

void loop(void)
{
}
