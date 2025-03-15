#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "http-get.h"

/*
{
    "Global Quote": {
        "01. symbol": "SPY",
        "02. open": "556.1100",
        "03. high": "563.8300",
        "04. low": "551.4917",
        "05. price": "562.8100",
        "06. volume": "62660321",
        "07. latest trading day": "2025-03-14",
        "08. previous close": "551.4200",
        "09. change": "11.3900",
        "10. change percent": "2.0656%"
    }
}
*/
#define APIKEY "SV368WQ25YDFNM99"

typedef struct {
	char ticker[8];
	float price;
	float percentChange;
} stockData;

stockData stockDataExtract(char *ticker) {
	// form url for http get request & store response in stats variable
		char fullUrl[100] = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=";
		strcat(fullUrl, ticker);
		strcat(fullUrl, APIKEY);
		char stats[400];
		strcpy(stats, http_get(fullUrl)->data);
	
		const char *ptr = strstr(stats, "\"price\":");


}
int main() {
	
	printf("%s",http_get("https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=SPY&apikey=SV368WQ25YDFNM99")->data);
}
