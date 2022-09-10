#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "server.h"

ST_accountsDB_t Accounts_DB[255] = { {18000.00, "5170613980119385"},
                                    {1300.00, "5170612673254426"},
                                    {25500.00, "5170612221062164"},
                                    {1750.00, "5170615382231908"},
                                    {11280.00, "5170615647337391"} };

ST_transaction_t Transactions_DB[255] = { {{0}, {0}, {0}}, {0.0, 0.0, {0}}, 0, 0 }; //Initialize all values to zero

uint32_t Account_N, Trans_Cnt = 0;

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
    for (int x = 0; x < 255; x++) {
        if (strcmp(cardData->primaryAccountNumber, Accounts_DB[x].primaryAccountNumber) == 0) {
            Account_N = x;
            return OK_Server;
        }
    }
    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
    if ((Accounts_DB[Account_N].balance) >= (termData->transAmount)) {
        return OK_Server;
    }
    else {
        return LOW_BALANCE;
    }
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
    if (Trans_Cnt > 254) {
        return SAVING_FAILED;
    }

    if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) {
        Transactions_DB[Trans_Cnt].transState = DECLINED_INSUFFECIENT_FUND;
    }
    else if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND) {
        Transactions_DB[Trans_Cnt].transState = DECLINED_STOLEN_CARD;
    }
    else {
        Transactions_DB[Trans_Cnt].transState = APPROVED;
    }

    Transactions_DB[Trans_Cnt].cardHolderData = transData->cardHolderData;
    Transactions_DB[Trans_Cnt].terminalData = transData->terminalData;
    Transactions_DB[Trans_Cnt].transactionSequenceNumber = Trans_Cnt;
    Trans_Cnt++;

    return OK_Server;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
    if (transactionSequenceNumber < Trans_Cnt) {
        transData->cardHolderData = Transactions_DB[transactionSequenceNumber].cardHolderData;
        transData->terminalData = Transactions_DB[transactionSequenceNumber].terminalData;
        transData->transactionSequenceNumber = Transactions_DB[transactionSequenceNumber].transactionSequenceNumber;
        transData->transState = Transactions_DB[transactionSequenceNumber].transState;
        return OK_Server;
    }
    else {
        return TRANSACTION_NOT_FOUND;
    }
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
    if (saveTransaction(transData) == SAVING_FAILED) {
        return INTERNAL_SERVER_ERROR;
    }
    else if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) {
        return DECLINED_INSUFFECIENT_FUND;
    }
    else if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND) {
        return DECLINED_STOLEN_CARD;
    }
    else {
        Accounts_DB[Account_N].balance -= transData->terminalData.transAmount;
        return APPROVED;
    }
}