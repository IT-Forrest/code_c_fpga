/* ========================================
*
* Copyright YOUR COMPANY, THE YEAR
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* WHICH IS THE PROPERTY OF your company.
*
* Use for serial port Read/Write operation
* ========================================
*/

#include <math.h>
#include <stdint.h>
#include "device.h"
#include <stdlib.h>


void PutStr(char *str) // Output string to terminal
{
    USBUART_1_PutString(str);
    while (!USBUART_1_CDCIsReady());
}

// Read number from console, length is in decimal
uint16  ReadInput(uint8 *length)
{
    //char8 rdin[10] = {'\0'};
    char8 ch = ' ';
    uint16 value = 0;
    *length = 0;
    //fgets(rdin,10-1,stdin);
    //if (*(rdin) != '\n')
    //{
    //    *length = 1;
    //    value = atoi(rdin);
    //}
    //return value;

    while (ch != '\n')
    {
        while (!USBUART_1_DataIsReady());
        ch = USBUART_1_GetChar();

        // the maximum length of uint16 in decimal is 5
        if (ch <= '9' && ch >= '0' && *length <= 5)
        {
            //USBUART_1_PutChar(ch);
            while (!USBUART_1_CDCIsReady());
            value = value * 10 + (ch - '0');
            (*length)++;
        }
        else if (ch == '\b')
        {
            //USBUART_1_PutChar(ch);
            while (!USBUART_1_CDCIsReady());
            value = value / 10;
            if (*length > 0) (*length)--;
        }
        else
        {
            //PutStr("\n");
        }
    }

    return value;

}

void ReadArray(uint8 *dat, uint8 *length) // Read number from console
{
    char8 ch = ' ';
    *length = 0;

    //uint8 index = 0;
    //fgets((char*)dat,12,stdin);
    //while((dat[index] != '\n') && (*length < 12))
    //{
    //    ch = dat[index];
    //   if (ch <= '9' && ch >= '0')
    //    {
    //        dat[*length]= ch - '0';
    //        (*length)++;
    //    }
    //    index++;
    //}
    //return ;

    while (ch != '\n')
    {
        while (!USBUART_1_DataIsReady());
        ch = USBUART_1_GetChar();

        if (ch <= '9' && ch >= '0' && *length < 12)
        {
            //USBUART_1_PutChar(ch);
            while (!USBUART_1_CDCIsReady());
            dat[*length] = ch - '0';
            (*length)++;
        }
        else if (ch == '\b')
        {
            //USBUART_1_PutChar(ch);
            while (!USBUART_1_CDCIsReady());
            dat[*length] = 0;
            if (*length > 0) (*length)--;
        }
        else// if (ch == '\n')
        {
            //PutStr("\r\n");
        }
    }

}

void PutWait() // Wait for any key
{
    PutStr("#WAT\tPress Any Key to continue... ");
    while (getchar() != '\n');// consume all the input chars
    return;
}

//int abs(int val);
void num2str(int16 value, char *str) // uint16 to String
{
    uint8	i = 0;
    uint8	c = 0;
    uint8	len = 0;
    char8	buf[8];
    int16	val = abs(value);

    do
    {
        buf[i] = (val % 10) + '0';
        val = val / 10;
        i = i + 1;
    } while (val != 0);
    len = i;

    if (value < 0)
    {
        str[0] = '-'; c = 1;
    }
    else c = 0;

    for (i = 0; i < len; i++)
    {
        str[i + c] = buf[len - i - 1];
    }
    //str[i+c] = '\t';
    //str[i+c+1] = '\0';
    str[i + c] = '\0';
}

void FmtPrint(int32 val) // Format and printer value
{
    char txt[32] = "";

    num2str(val, txt);
    PutStr(txt);
}


/* [] END OF FILE */
