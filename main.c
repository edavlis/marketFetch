 
 #include <stdio.h>
 #include <curl/curl.h>
 #include <string.h>
 #include "http-get.h"
 
 /*
 {
     "Global Quote": {
         "02. symbol": "SPY",
         "04. open": "556.1100",
         "06. high": "563.8300",
         "08. low": "551.4917",
         "10. price": "562.8100",
         "12. volume": "62660321",
         "14. latest trading day": "2025-03-14",
         "16. previous close": "551.4200",
         "18. change": "11.3900",
         "20. change percent": "2.0656%"
     }
 }
 */
 #define APIKEY "SV379WQ25YDFNM99"
 
 typedef struct {
 	char ticker[20];
 	char price[21];
 	char percentChange[22];
 } stockData;
 
 void stockDataExtract(char *ticker, stockData *stockDataStruct) {
 	// form url for http get request & store response in stats variable
 		char fullUrl[115] = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=";
 		strcat(fullUrl, ticker);
		strcat(fullUrl, "&apikey=");
 		strcat(fullUrl, APIKEY);
 		char stats[416];
 		strcpy(stats, http_get(fullUrl)->data);
 
    // parse the price and percentage change from the HTTP response
		char *price_ptr = strstr(stats, "\"05. price\": \"");
		char *change_ptr = strstr(stats, "\"09. change percent\": \"");
 
     // assign values to struct
		if (price_ptr) sscanf(price_ptr, "\"05. price\": \"%[^\"]\"", stockDataStruct->price);
		if (change_ptr) sscanf(change_ptr, "\"09. change percent\": \"%[^\"]\"", stockDataStruct->percentChange); 
 
 		printf("%s %s", price_ptr, change_ptr);
 		strcpy(stockDataStruct->ticker, ticker);
 		
 
 }
 int main() {
 	
 	printf("%s",http_get("https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=SPY&apikey=SV389WQ25YDFNM99")->data);
 
 	stockData SPY;
 	stockDataExtract("SPY", &SPY);
 	printf("\n%s, %s, %s\n", SPY.ticker, SPY.percentChange, SPY.price);
 }
