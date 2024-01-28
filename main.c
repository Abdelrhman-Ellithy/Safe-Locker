/*
 * Safe.c
 *
 * Created: 2023-11-10 7:52:13 AM
 * Author : Abdelrahman Ellithy
 */
#include "MCAL/DIO Driver/DIO.h"
#include "HAL/Keypad Driver/Keypad.h"
#include "HAL/LCD Driver/CLCD_interface.h"
#include "MCAL/EEPROM Driver/EEPROM.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#define flag_adsress 0x0070
#define Already_set 0x00
#define base_adsress 0x0051
#define FLAG_FRIST 0xff
#define PASSWORD_LENGTH 6
void Set_password(void);
void PRINT_WORNG_password(void);
void Enter_password(void);
void Correct_password(void);
u8 str_setpass[]="Set Password";
u8 str_pass_seted[]="Password saved";
u8 str_Enter_pass[]="Enter Passowrd " ;
u8 str_Tries_left[]="Tries left" ;
u8 str_Wrong_pass[]="Wrong Passowrd" ;
u8 str_Correct_pass[]="Correct Passowrd" ;
u8 str_safe_is_open[]="Safe is open now" ;
u8 str1_reset_pass[]="Enter * to" ;
u8 str2_reset_pass[]="reset password" ;
u8 str1_reached_tries[]="You reached the" ;
u8 str2_reached_tries[]="Maximum tries" ;
u8 str_safe_close[]="safe will close" ;
u8 str_okay[]="Not * Okay" ;
u8 Pressed_Char	;
u8 tries=1,readed=0x00;
u8 flag=1 ,data,i=0;
int main(void)
{
    CLCD_voidInit();
    Keypad_init();
    readed=0x00;
    while(1)
    {
        EEPROM_read(flag_adsress,&readed);
        if(readed==FLAG_FRIST)
        {
            Set_password();

            CLCD_voidClearScreen();
            CLCD_voidSendString(str1_reset_pass);
            CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
            CLCD_voidSendString(str2_reset_pass);
            Pressed_Char=Get_pressed();
            while ( (Pressed_Char !='*') && (Pressed_Char ==NotPressed) ){
                Pressed_Char=Get_pressed();
            }
            if (Pressed_Char=='*')
            {
                CLCD_voidClearScreen();
                Set_password();
            }
            else{
                CLCD_voidClearScreen();
                CLCD_voidSendString(str_okay);
                CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
                CLCD_voidSendString(str_safe_close);
                _delay_ms(2000);
            }
        }
        else
        {
            flag=1 ,i=0;
            Enter_password();
            if(flag==1)
            {
                CLCD_voidClearScreen();
                CLCD_voidSendString(str_Correct_pass);
                CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
                CLCD_voidSendString(str_safe_is_open);
                _delay_ms(2000);
                CLCD_voidClearScreen();
                CLCD_voidSendString(str1_reset_pass);
                CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
                CLCD_voidSendString(str2_reset_pass);
                _delay_ms(2000);
                Pressed_Char=Get_pressed();
                while ((Pressed_Char !='*') && ( Pressed_Char ==NotPressed) ){
                    Pressed_Char=Get_pressed();
                }
                if (Pressed_Char=='*')
                {
                    CLCD_voidClearScreen();
                    Set_password();
                }
                else{
                    CLCD_voidClearScreen();
                    CLCD_voidSendString(str_okay);
                    CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
                    CLCD_voidSendString(str_safe_close);
                    _delay_ms(2000);
                }

            }
            else
            {
                PRINT_WORNG_password();
                CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
                CLCD_voidSendString(str_Tries_left);
                CLCD_voidSendNumber(3-tries);
                _delay_ms(500);
                _delay_ms(500);
                _delay_ms(500);
				tries++;
                flag=1 ,data,i=0;
                Enter_password();
                while(tries<3 && (flag==0) ){
                    PRINT_WORNG_password();
                    CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
                    CLCD_voidSendString(str_Tries_left);
                    CLCD_voidSendNumber(3-tries);
                    _delay_ms(500);
                    _delay_ms(500);
                    _delay_ms(500);
                    Enter_password();
                    tries++;
                }
                tries=1;
                if(flag==1){
                    Correct_password();
                }
                else{
                    CLCD_voidClearScreen();
                    CLCD_voidSendString(str1_reached_tries);
                    CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
                    CLCD_voidSendString(str2_reached_tries);
                    _delay_ms(3000);
                    CLCD_voidClearScreen();
                    CLCD_voidSendString(str_safe_close);
                    _delay_ms(10000);
                }
            }

        }

    }

}
void Set_password(void)
{
    CLCD_voidSendString(str_setpass);
    CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
    i=0;
    while(i<PASSWORD_LENGTH)
    {
        if  ((Pressed_Char=Get_pressed() )!='*' && Pressed_Char !=NotPressed)
        {
            EEPROM_write(base_adsress+i,Pressed_Char);
            i++;
            CLCD_voidSendData('*');
        }

    }
    i=0;
    CLCD_voidClearScreen();
    CLCD_voidSendString(str_pass_seted);
    CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
    CLCD_voidSendString(str_safe_is_open);
    EEPROM_write(flag_adsress,Already_set);
    _delay_ms(500);
    _delay_ms(500);
    _delay_ms(500);
}
void PRINT_WORNG_password(void)
{
    CLCD_voidClearScreen();
    CLCD_voidSendString(str_Wrong_pass);

}
void Enter_password(void)
{
    CLCD_voidClearScreen();
    CLCD_voidSendString(str_Enter_pass);
    CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
    i=0;
    while(i<PASSWORD_LENGTH)
    {
        if ( (Pressed_Char=Get_pressed())!=NotPressed)
        {
            EEPROM_read(base_adsress+i,&data);
            if(Pressed_Char!=data)
            {
                flag=0;
            }
            CLCD_voidSendData('*');
            i++;
        }
    }
    i=0;

}
void Correct_password(void){
    CLCD_voidClearScreen();
    CLCD_voidSendString(str_Correct_pass);
	CLCD_voidSetPosition(CLCD_ROW_2,CLCD_COL_1);
    CLCD_voidSendString(str_safe_is_open);
    _delay_ms(2000);
    CLCD_voidClearScreen();
    CLCD_voidSendString(str1_reset_pass);
    CLCD_voidSendString(str2_reset_pass);
    if  ((Pressed_Char=Get_pressed())=='*')
    {
        CLCD_voidClearScreen();
        Set_password();
    }
}