//
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "http-get.h"

#define APIKEY "SV379WQ25YDFNM99"
typedef struct {
	char ticker[20];
	char price[21];
	char percentChange[22];
} stockData;

const char *fake_http_get() { // so htat i dont have to use api requests
    return "{ \"Global Quote\": {"
           "\"01. symbol\": \"SPY\","
           "\"02. open\": \"556.1100\","
           "\"03. high\": \"563.8300\","
           "\"04. low\": \"551.4917\","
           "\"05. price\": \"562.8100\","
           "\"06. volume\": \"62660321\","
           "\"07. latest trading day\": \"2025-03-14\","
           "\"08. previous close\": \"551.4200\","
           "\"09. change\": \"11.3900\","
           "\"10. change percent\": \"2.0656%\""
           "} }";
};

void stockDataExtract(char *ticker, stockData *stockDataStruct) {
	// form url for http get request & store response in stats variable
		char fullUrl[115] = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=";
		strcat(fullUrl, ticker);
		strcat(fullUrl, "&apikey=");
		strcat(fullUrl, APIKEY);
		char stats[416];
		strcpy(stats, http_get(fullUrl)->data);
	//	strcpy(stats, fake_http_get());
		
	// parse the price and percentage change from the HTTP response
		char *price_ptr = strstr(stats, "\"05. price\": \"");
		char *change_ptr = strstr(stats, "\"10. change percent\": \"");

	// assign values to struct
		if (price_ptr) sscanf(price_ptr, "\"05. price\": \"%[^\"]\"", stockDataStruct->price);
		if (change_ptr) sscanf(change_ptr, "\"10. change percent\": \"%[^%%]%%\"", stockDataStruct->percentChange);
	//	printf("Price: %s\n Percentage Change: %s", price_ptr, change_ptr); // this was for testing
		strcpy(stockDataStruct->ticker, ticker);
		

}
int main() {

	// create list of stocks
	stockData stockList[6] = { {"SPY", "", ""}, {"NVDA", "", ""}, {"AAPL", "", ""}, {"LMT", "", ""}, {"NKE", "", ""}, {"AMZN", "", ""} };

	for (int i = 0; i < 6; i++) {
		stockDataExtract(stockList[i].ticker, &stockList[i]);
	}

//	FILE *fptr; // file for opening the ascii files. 
//	fptr = fopen("upgraph.txt", "r");
//	char imageStorage[2048]; // i have no clue how big the actuall ascii file is 
//	while (fgets(imageStorage, 2048, fptr)) {
//		printf("%s", imageStorage);
//	}
	
	// check if it's a a red or green day
	if (atoi(stockList[0].percentChange) <= 0) { // Checks if '-' is at the first position
		system("bash downgraph.txt");
	} else {
		system("bash upgraph.txt");
	    }

	// print out stocks
	for (int i = 0; i < 6; i++) {
		printf("\n   %-6s:    $%-10s    Change: %-9s%%", stockList[i].ticker, stockList[i].price, stockList[i].percentChange);
	} 
	printf("\n");

}





