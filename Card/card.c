#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "card.h"

EN_cardError_t getCardHolderName (ST_cardData_t * cardData)
{
    uint8_t CH_Name[30];
    printf("Please enter the Card Holder Name\n");
    gets(CH_Name);

    if(strlen(CH_Name)<20 || strlen(CH_Name)>24){
        return WRONG_NAME;
    }
    else{
        strcpy(cardData->cardHolderName, CH_Name);
        return OK_Data;
    }
}

EN_cardError_t getCardExpiryDate (ST_cardData_t * cardData)
{
    uint8_t CEXP_Date[30], MM[3], YY[3];
    printf("Please enter the Card Expiry Date\n");
    gets(CEXP_Date);

    if(strlen(CEXP_Date)!=5){
        return WRONG_EXP_DATE;
    }
    else if(CEXP_Date[2]!='/'){
        return WRONG_EXP_DATE;
    }
    else{
        MM[0]=CEXP_Date[0];
        MM[1]=CEXP_Date[1];
        YY[0]=CEXP_Date[3];
        YY[1]=CEXP_Date[4];

        if(atoi(MM)>=1 && atoi(MM)<=12 && atoi(YY)>=22){
            strcpy(cardData->cardExpirationDate, CEXP_Date);
            return OK_Data;
        }
        else{
            return WRONG_EXP_DATE;
        }
    }
}

EN_cardError_t getCardPAN (ST_cardData_t * cardData)
{
    uint8_t CPA_Num[30];
    printf("Please enter the Primary Account Number\n");
    gets(CPA_Num);

    if(strlen(CPA_Num)<16 || strlen(CPA_Num)>19){
        return WRONG_PAN;
    }
    else{
        strcpy(cardData->primaryAccountNumber, CPA_Num);
        return OK_Data;
    }
}
