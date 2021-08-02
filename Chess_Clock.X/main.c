#include <pic18f4520.h>
#include "bits.h"
#include "config.h"
#include "io.h"
#include "delay.h"
#include "ssd.h"
#include "timer.h"
#include "keypad.h"
#include "lcd.h"

void pretas(unsigned char coluna){
    char i;
    char black[64] = {
        0x01, 0x03, 0x01, 0x0F, 0x07, 0x03, 0x03, 0x07, //0,0
        0x10, 0x18, 0x10, 0x1E, 0x1C, 0x18, 0x18, 0x1C, //0,1 
        0x00, 0x11, 0x1B, 0x0F, 0x03, 0x03, 0x03, 0x07, //0,2
        0x00, 0x11, 0x1B, 0x1E, 0x18, 0x18, 0x18, 0x1C, //0,3 
        0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0F, 0x1F, //1,0
        0x18, 0x18, 0x18, 0x18, 0x18, 0x1C, 0x1E, 0x1F,  //1,1
        0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0F, 0x1F, //1,2
        0x18, 0x18, 0x18, 0x18, 0x18, 0x1C, 0x1E, 0x1F //1,3
    };
    lcd_cmd(0x40); //Configura para a primeira posição de memória
    //Envia cada uma das linhas em ordem
    for (i = 0; i < 64; i++) {
        lcd_dat(black[i]);
    }
    lcd_cmd(L_L1 + coluna);
    lcd_dat(0);
    lcd_dat(1);
    lcd_dat(2);
    lcd_dat(3);
    lcd_cmd(L_L2 + coluna);
    lcd_dat(4);
    lcd_dat(5);
    lcd_dat(6);
    lcd_dat(7);
}

void brancas(unsigned char coluna){
    char i;
    char white[64] = {
        0x01,0x03,0x01,0x0F,0x04,0x02,0x02,0x06,
        0x10,0x18,0x10,0x1E,0x04,0x08,0x08,0x0C,
        0x00,0x11,0x1A,0x0C,0x02,0x02,0x02,0x04,
        0x00,0x11,0x0B,0x06,0x08,0x08,0x08,0x04,
        0x02,0x02,0x02,0x02,0x02,0x04,0x08,0x1F,
        0x08,0x08,0x08,0x08,0x08,0x04,0x02,0x1F,
        0x02,0x02,0x02,0x02,0x02,0x04,0x08,0x1F,
        0x08,0x08,0x08,0x08,0x08,0x04,0x02,0x1F
    };
    lcd_cmd(0x40); //Configura para a primeira posição de memória
    //Envia cada uma das linhas em ordem
    for (i = 0; i < 64; i++) {
        lcd_dat(white[i]);
    }
    lcd_cmd(L_L1 + coluna);
    lcd_dat(0);
    lcd_dat(1);
    lcd_dat(2);
    lcd_dat(3);
    lcd_cmd(L_L2 + coluna);
    lcd_dat(4);
    lcd_dat(5);
    lcd_dat(6);
    lcd_dat(7);
}

int main(){
    int UnMin = 0, DeMin = 0, UnSeg = 0, DeSeg = 0, tempo;
    int contP1, contP2, flagcontrole = 0;
    unsigned int t1, t2, tecla1, tecla2, tempflag = 0;
    unsigned char i, j;
    
    TRISD = 0x00;
    TRISE = 0x00;
    PORTD = 0x00;
    TRISE = 0x00;
    TRISA = 0xC3;
    TRISB = 0x03;
    TRISC = 0x01;
    
    lcd_init();
    lcd_cmd(0x0C); // desliga o cursor
    
    pretas(3);
    atraso_s(1);
    lcd_cmd(L_CLR);
    
    brancas(8);
    atraso_s(1);
    lcd_cmd(L_CLR);
    
    lcd_cmd(L_L1 + 5);
    lcd_str("CHESS");
    lcd_cmd(L_L2 + 5);
    lcd_str("UNIFEI");
    atraso_s(2);
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1 + 3);
    lcd_str("Escolha do ");
    lcd_cmd(L_L2 + 5);
    lcd_str("Tempo");
    
    ssdInit();
    kpInit();
    timerInit();
    
    for(;;){
        
        ssdDigit((DeMin%10), 0);
        ssdDigit((UnMin%10), 1);
        ssdDigit((DeSeg%10), 2);
        ssdDigit((UnSeg%10), 3);
        for (tempo=0; tempo<100; tempo++);        
        ssdUpdate();
        
        kpDebounce();
        if (kpRead() != tecla1){
            tecla1 = kpRead();
            
            if (bitTst(kpRead(), 3)){ // tecla 1 - incrementa DeMin
                DeMin++;
                if (DeMin > 9){
                    DeMin = 0;
                }
            }
            if (bitTst(kpRead(), 7)){ // tecla 2 - incrementa UnMin
                UnMin++;
                if (UnMin > 9){
                    UnMin = 0;
                }
            }
            if (bitTst(kpRead(), 11)){ // tecla 3 - incrementa DeSeg
                DeSeg++;
                if (DeSeg > 6){
                    DeSeg = 0;
                }
            }
            if (bitTst(kpRead(), 2)){ // tecla 4 - incrementa UnSeg
                UnSeg++;
                if (UnSeg > 9){
                    UnSeg = 0;
                }
            }
            if (bitTst(kpRead(), 0)){
                contP1 = UnSeg+(DeSeg*10)+(UnMin*100)+(DeMin*1000);
                contP2 = UnSeg+(DeSeg*10)+(UnMin*100)+(DeMin*1000);
                timerInit();
                tempflag = 1;
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("Brancas");
                lcd_cmd(L_L2);
                lcd_str("Jogam");
                brancas(9);
                
                //inicia a partida
                for(;;){
                    timerReset(5000);
                    
                    if (tempflag == 1){ // brancas jogam
                        t1++;
                        if (t1 >= 200){
                            contP1--;
                            if (contP1 <= 0){
                                flagcontrole = 1;
                            }
                            if (contP1 / 10 % 10 == 9){
                                contP1 -= 40;
                            }
                            ssdDigit(((contP1 / 1000) % 10), 0);
                            ssdDigit(((contP1 / 100) % 10), 1);
                            ssdDigit(((contP1 / 10) % 10), 2);
                            ssdDigit((contP1 % 10), 3);
                            t1 = 0;
                        }
                    }
                    if (tempflag == 2){ // pretas jogam
                        t2++;
                        if (t2 >= 200){
                            contP2--;
                            if (contP2 <= 0){
                                flagcontrole = 2;
                            }
                            if (contP2 / 10 % 10 == 9){
                                contP2 -= 40;
                            }
                            ssdDigit(((contP2 / 1000) % 10), 0);
                            ssdDigit(((contP2 / 100) % 10), 1);
                            ssdDigit(((contP2 / 10) % 10), 2);
                            ssdDigit((contP2 % 10), 3);
                            t2 = 0;
                        }
                    }
                    
                    kpDebounce();
                    if (kpRead() != tecla2){
                        tecla2 = kpRead();
                        
                        if (bitTst(kpRead(), 3)){ // tecla 1, jogador peças brancas
                            lcd_cmd(L_CLR);
                            lcd_cmd(L_L1);
                            lcd_str("Brancas");
                            lcd_cmd(L_L2);
                            lcd_str("Jogam");
                            brancas(9);
                            
                            tempflag = 1;
                            
                        }
                        if (bitTst(kpRead(), 7)){ // tecla 2, jogador peças pretas
                            
                            lcd_cmd(L_CLR);
                            lcd_cmd(L_L1);
                            lcd_str("Pretas");
                            lcd_cmd(L_L2);
                            lcd_str("Jogam");
                            pretas(9);
                            
                            tempflag = 2;
                        }
                    }
                    
                    if (flagcontrole == 1){
                        flagcontrole = 0;
                        tempflag = 0;
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1 + 1);
                        lcd_str("Vencedor");
                        lcd_cmd(L_L2 + 2);
                        lcd_str("Pretas");
                        pretas(10);
                        
                        for (j = 0; j < 3; j++) {
                            for (i = 1; i > 0; i = i * 2) {
                                bitSet(TRISC, 1);
                                PORTB = i;
                                PORTD = i;
                                atraso_ms(100);
                            }
                            bitClr(TRISC, 1);
                        }
                    }
                    
                    if (flagcontrole == 2){
                        flagcontrole = 0;
                        tempflag = 0;
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1 + 1);
                        lcd_str("Vencedor");
                        lcd_cmd(L_L2 + 2);
                        lcd_str("Brancas");
                        brancas(10);
                        
                        for (j = 0; j < 3; j++) {
                            for (i = 1; i > 0; i = i * 2) {
                                bitSet(TRISC, 1);
                                PORTB = i;
                                PORTD = i;
                                atraso_ms(100);
                            }
                            bitClr(TRISC, 1);
                        }
                    }

                    ssdUpdate();
                    timerWait();
                }
            }
        }   
    }
}
