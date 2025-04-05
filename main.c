#include <stdio.h>
#include <time.h>
#include <stdbool.h>
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

extern const char redGraph[13][87];
extern const char greenGraph[13][87];

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
}

void stockDataExtract(char *ticker, stockData *stockDataStruct) {
	// form url for http get request & store response in stats variable
		char fullUrl[115] = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=";
		strcat(fullUrl, ticker);
		strcat(fullUrl, "&apikey=");
		strcat(fullUrl, APIKEY);
		char stats[416];
	//	strcpy(stats, http_get(fullUrl)->data);
		strcpy(stats, fake_http_get());
		
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

	// time things
	time_t t;
	struct tm *tm_info;
	char buffer[64];

	// Get current time
	time(&t);
	tm_info = localtime(&t);
	strftime(buffer, sizeof(buffer), "(%A) %d/%m/%Y %H:%M", tm_info);

	// create list of stocks
	stockData stockList[6] = { {"SPY", "", ""}, {"NVDA", "", ""}, {"AAPL", "", ""}, {"LMT", "", ""}, {"NKE", "", ""}, {"AMZN", "", ""}};
	int length = sizeof(stockList) / sizeof(stockList[0]);
	for (int i = 0; i < 6; i++) {
		stockDataExtract(stockList[i].ticker, &stockList[i]);
	}

	// choose whethere to use red or grreen graph
	float SPYpercentChange = atoi(stockList[0].percentChange); // SPY average market change
	char currentGraph[13][87];
	if (SPYpercentChange >= 0) {
		for (int i = 0; i < 13; i++) {
		    strcpy(currentGraph[i], greenGraph[i]);
		}
	} else {
		for (int i = 0; i < 13; i++) {
		    strcpy(currentGraph[i], redGraph[i]);
		}
	}

	// print out graph and stocks
	int m = 0;
 	printf("\033[2J\033[H\n"); // clear screen
	printf("\t\t\033[1;37m%s\033[0m\n\n", buffer); // print time
	for (int i = 0; i < 13; i++) { // per line of graph
			if (SPYpercentChange > 0) {
				printf("\033[32m%s\033[0m", currentGraph[i]);
			} else if (SPYpercentChange < 0) {
				printf("\033[31m%s\033[0m", currentGraph[i]);
			} else {
				printf("%s", currentGraph[i]);
			}


			if (m < length) {
				if (atoi(stockList[m].percentChange) >= 0) {
					printf("\t%s \033[1;32m%s %0.2f%%\033[0m",stockList[m].ticker, stockList[m].price, (float) atoi(stockList[m].percentChange));
					printf("\033[0m\n");

				} else {
					printf("\t%s \033[1;31m%s %0.2f%%\033[0m",stockList[m].ticker, stockList[m].price, (float) atoi(stockList[m].percentChange));
					printf("\033[0m\n");
				}
			} else {
				printf("\n");
			}
						printf("\033[0m");

		m++;
	}



}
