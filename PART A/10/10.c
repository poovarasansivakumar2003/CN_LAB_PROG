#include <stdio.h> 
#include <netdb.h> 
#include <arpa/inet.h> 
int main() { 
    char host[100]; 
    printf("Enter domain: "); 
    scanf("%s", host); 
    struct hostent *h = gethostbyname(host); 
    if (h)  
        printf("IP: %s\n", inet_ntoa(*(struct in_addr*)h->h_addr_list[0])); 
    else  
        printf("Lookup failed\n"); 
    return 0; 
}