#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "app.h"

void appStart(void) {

	ST_cardData_t Card_N1;

	
	//Testing getCardHolderName function
	if (getCardHolderName(&Card_N1) == OK_Data) {
		printf("Card Holder Name is Valid\n");
	}
	else {
		printf("Wrong Name\n");
		return 0;
	}

	//Testing getCardExpiryDate function
	if (getCardExpiryDate(&Card_N1) == OK_Data) {
		printf("Card Expiry Date is Valid\n");
	}
	else {
		printf("Wrong Date\n");
		return 0;
	}

	//Testing getCardPAN function
	if (getCardPAN(&Card_N1) == OK_Data) {
		printf("Primary Account Number is Valid\n");
	}
	else {
		printf("Wrong PAN\n");
		return 0;
	}
	






	
	//Card Data for testing
	//ST_cardData_t Card_N1 = { "Mohab Osamah Sharafeldin", "5170615382231908", "07/25" };

	ST_terminalData_t Terminal_N1;

	//Testing getTransactionDate function
	getTransactionDate(&Terminal_N1);
	printf("Transaction Date is %s\n", Terminal_N1.transactionDate);

	//Testing isCardExpired function
	if (isCardExpired(Card_N1, Terminal_N1) == OK_Terminal) {
		printf("Valid Card\n");
	}
	else {
		printf("Expired Card\n");
		return 0;
	}

	//Testing isValidCardPAN function
	if (isValidCardPAN(&Card_N1) == OK_Terminal) {
		printf("Valid PAN\n");
	}
	else {
		printf("Invalid PAN\n");
		return 0;
	}

	//Testing getTransactionAmount function
	if (getTransactionAmount(&Terminal_N1) == OK_Terminal) {
		printf("Valid Transaction Amount\n");
	}
	else {
		printf("Invalid Amount\n");
		return 0;
	}

	//Testing setMaxAmount function
	if (setMaxAmount(&Terminal_N1) == OK_Terminal) {
		printf("Valid Max Transaction Amount\n");
	}
	else {
		printf("Invalid Max Amount\n");
		return 0;
	}

	//Testing isBelowMaxAmount function
	if (isBelowMaxAmount(&Terminal_N1) == OK_Terminal) {
		printf("Valid, and Difference Amount is %.2f\n", Terminal_N1.maxTransAmount - Terminal_N1.transAmount);
	}
	else {
		printf("Exceed Max Amount\n");
		return 0;
	}
	





	//Card, Terminal and Transaction Data for testing
	//ST_cardData_t Card_N1 = { "Mohab Osamah Sharafeldin", "5170615382231908", "07/25" };
    //ST_terminalData_t Terminal_N1 = { 500.00, 15000.00, "13/08/2022" };
	ST_transaction_t Trans_N1;
	Trans_N1.cardHolderData = Card_N1;
	Trans_N1.terminalData = Terminal_N1;
	
	/*
	//Testing isValidAccount function
	if (isValidAccount(&Card_N1) == OK_Data) {
		printf("Valid Card\n");
	}
	else {
		printf("Declined Stolen Card\n");
		return 0;
	}

	//Testing isAmountAvailable function
	if (isAmountAvailable(&Terminal_N1) == OK_Terminal) {
		printf("Valid, and Difference Amount is %.2f\n", Accounts_DB[Account_N].balance - Terminal_N1.transAmount);
	}
	else {
		printf("Low Balance\n");
		return 0;
	}

	//Testing saveTransaction function
	if (saveTransaction(&Trans_N1) == OK_Server) {
		printf("Saved Successfully\n");
	}
	else {
		printf("Saving Failed\n");
		return 0;
	}
	*/

	//Testing recieveTransactionData function
	EN_transState_t Trans_State_N1 = recieveTransactionData(&Trans_N1);
	if (Trans_State_N1 == INTERNAL_SERVER_ERROR) {
		printf("Internal Server Error\n");
		return 0;
	}
	else if (Trans_State_N1 == DECLINED_STOLEN_CARD) {
		printf("Declined Stolen Card\n");
		return 0;
	}
	else if (Trans_State_N1 == DECLINED_INSUFFECIENT_FUND) {
		printf("Declined Insuffecient Fund\n");
		return 0;
	}
	else {
		printf("Transaction Approved\n");
		printf("New Available Balance is %.2f\n", Accounts_DB[Account_N].balance);
	}

	//Testing getTransaction function
	ST_transaction_t Trans_Test;
	if (getTransaction(0, &Trans_Test) == OK_Server) {
		printf("Found, %s\n", Trans_Test.cardHolderData.cardHolderName);
		return 0;
	}
	else {
		printf("Transaction Not Found\n");
	}
	
}