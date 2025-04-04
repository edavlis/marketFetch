marketfetch: main.c
	gcc main.c http-get.c greenGraph.c redGraph.c -lcurl -o marketFetch 


