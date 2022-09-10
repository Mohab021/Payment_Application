#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    time_t t = time(NULL); //The time(), localtime() are functions and time_t data type is present inside the time.h header file
    struct tm L_T_struct = *localtime(&t); // struct tm is a structure defined in the time.h header file

    sprintf(termData->transactionDate, "%02d/%02d/%04d", L_T_struct.tm_mday, L_T_struct.tm_mon + 1, L_T_struct.tm_year + 1900); //int to string
    return OK_Terminal; //Always guaranteed in this case
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    uint8_t MM_C[3], YY_C[3]; // cardExpirationDate form is MM/YY
    uint8_t MM_T[3], YY_T[3]; // transactionDate form is DD/MM/YYYY
    strncpy(MM_C, &cardData.cardExpirationDate[0], 2); //printf("%d\n", atoi(MM_C)); //for testing, delete later
    strncpy(YY_C, &cardData.cardExpirationDate[3], 2); //printf("%d\n", atoi(YY_C)); //for testing, delete later
    strncpy(MM_T, &termData.transactionDate[3], 2); //printf("%d\n", atoi(MM_T)); //for testing, delete later
    strncpy(YY_T, &termData.transactionDate[8], 2); //printf("%d\n", atoi(YY_T)); //for testing, delete later

    if (atoi(YY_T) < atoi(YY_C)) { // 22<24
        return OK_Terminal;
    }
    else if ((atoi(YY_T) == atoi(YY_C)) && (atoi(MM_T) <= atoi(MM_C))) { // 22==22 && 08<=09
        return OK_Terminal;
    }
    else {
        return EXPIRED_CARD;
    }
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    uint8_t nDigits = strlen(cardData->primaryAccountNumber);
    uint16_t nSum = 0, d, r;

    for (sint8_t i = nDigits - 1, j = 0; i >= 0; i--, j++) {
        d = cardData->primaryAccountNumber[i] - '0';

        if (j % 2 > 0) { //odd index
            d *= 2;
        }

        r = d % 10;
        nSum += r;
        d /= 10;
        nSum += d;
    }

    if (nSum % 10 == 0) {
        return OK_Terminal;
    }
    else {
        return INVALID_CARD;
    }
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    float T_Amount;
    printf("Please enter the Transaction Amount\n");
    scanf("%f", &T_Amount);

    if (T_Amount > 0) {
        termData->transAmount = T_Amount;
        return OK_Terminal;
    }
    else {
        return INVALID_AMOUNT;
    }
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    float MXT_Amount;
    printf("Please enter the Max Transaction Amount\n");
    scanf("%f", &MXT_Amount);

    if (MXT_Amount > 0) {
        termData->maxTransAmount = MXT_Amount;
        return OK_Terminal;
    }
    else { return INVALID_MAX_AMOUNT; }
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    if (termData->transAmount <= termData->maxTransAmount) {
        return OK_Terminal;
    }
    else {
        return EXCEED_MAX_AMOUNT;
    }
}
