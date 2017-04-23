// 
// 
// 

#include "SButton.h"

/**
 * ����������� ������ ������
 * ������ ��� �������� ��� ���������� � ������� � ���������� �� �����
 * ������� ������������ ���������� �� ������� ������ (������������ 1)
 * � ���������� ������ (������������ ����� ������� ������, ����)
 * tm1 - ������� �������� ���������. �� ��������� 50��
 * tm2 - ����� �������� ������� �������. �� ��������� 2000��
 * tm3 - ����� �������� ������ � ��������� ����� �������. �� ��������� ���������
 * tm4 - ����� ����� ������� � �����. �� ��������� 500 ��. ���� tm3 = 0 �� �� ��������
 */
SButton::SButton(uint8_t pin,uint16_t tm1, uint16_t tm2,uint16_t tm3, uint16_t tm4){
   Pin               = pin;
   State             = false;
   Long_press_state  = false;
   Auto_press_state  = false;
   Ms1               = 0;
   Ms2               = 0;
   Ms_auto_click     = 0;
   TM_bounce         = tm1;
   TM_long_click     = tm2;
   TM_auto_click     = tm3;
   Period_auto_click = tm4;
}

/**
 * ������������� ������
 */
void SButton::begin() {
   pinMode(Pin, INPUT_PULLUP);
#ifdef DEBUG_SERIAL      
   Serial.print("Init button pin ");
   Serial.println(Pin);
#endif      
}

/**
 * �������� ������������ � ����� ��� �� �������
 * ���������� SB_NONE ���� ������ �� ������ � ������� ������� ��� ������� ������� ������
*/
SBUTTON_CLICK SButton::Loop() {
   uint32_t ms = millis();
   bool pin_state = digitalRead(Pin);
// ��������� ������� ������ 
   if( pin_state == LOW && State == false && (ms-Ms1)>TM_bounce ){
       uint16_t dt = ms - Ms1;
       Long_press_state = false;
       Auto_press_state = false;
#ifdef DEBUG_SERIAL      
       Serial.print(">>>Event button, pin=");
       Serial.print(Pin);
       Serial.print(",press ON, tm=");
       Serial.print(dt);
       Serial.println(" ms");
#endif      
       State = true;
       Ms2    = ms;
       if( TM_long_click == 0 && TM_auto_click == 0 )return SB_CLICK;
   }

// ��������� ������� ������� ������   
   if( pin_state == LOW && !Long_press_state && TM_long_click>0 && ( ms - Ms2 )>TM_long_click ){
      uint16_t dt      = ms - Ms2;
      Long_press_state = true;
#ifdef DEBUG_SERIAL      
      Serial.print(">>>Event button, pin=");
      Serial.print(Pin);
      Serial.print(",long press, tm=");
      Serial.print(dt);
      Serial.println(" ms");
#endif 
      return SB_LONG_CLICK;
   }

// ��������� ���� ������� ������   
   if( pin_state == LOW && TM_auto_click > 0 
       && ( ms - Ms2 ) > TM_auto_click 
       && ( ms - Ms_auto_click ) > Period_auto_click ){
      uint16_t dt      = ms - Ms2;
      Auto_press_state = true;
      Ms_auto_click    = ms;
#ifdef DEBUG_SERIAL      
      Serial.print(">>>Event button, pin=");
      Serial.print(Pin);
      Serial.print(",auto press, tm=");
      Serial.print(dt);
      Serial.println(" ms");
#endif 
      return SB_AUTO_CLICK;
   }

   
// ��������� ���������� ������ 
   if( pin_state == HIGH && State == true  && (ms-Ms2)>TM_bounce ){
       uint16_t dt      = ms - Ms2;
// ���������� ��� �����       
       State            = false;
#ifdef DEBUG_SERIAL      
       Serial.print(">>>Event button, pin=");
       Serial.print(Pin);
       Serial.print(",press OFF, tm=");
       Serial.print(dt);
       Serial.println(" ms");
#endif 
      Ms1    = ms;
// ���������� �������� ����      
      if( (TM_long_click != 0 || TM_auto_click != 0) && !Long_press_state && !Auto_press_state )return SB_CLICK;
       
   }


   return SB_NONE;
}     

