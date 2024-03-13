/*write a c program to get input of IP address from user and ask whether its classful or classless, if its classful do classful addresseing on IP address and give out of IP addresss with  its mask value, print first address, last address and no of hosts in IP address.
   if its classless do classless addresseing on IP address by getting mask value from user and give out of IP addresss with  its mask value, print first address, last address and no of hosts in IP address */
#include <stdio.h>
#include <string.h>

void classfulAddressing(char *ip_address) {

    int i, num;
    int octets[4];
    for (i = 0; i < 4; i++) {
        sscanf(strtok((i == 0) ? ip_address : NULL, "."), "%d", &num);
        octets[i] = num;
    }

    int first_octet = octets[0];
    printf("%d\n",octets[0]);
    int mask = 0, num_hosts = 0;
    if (first_octet >= 0 && first_octet <= 127) {
        printf("Class A\n");
        mask = 8;  // 255.0.0.0
        num_hosts=16,777,214;
    } else if (first_octet >= 128 && first_octet <= 191) {
        printf("Class B\n");
        mask = 16;  // 255.255.0.0
        num_hosts=65,536;
    } else if (first_octet >= 192 && first_octet <= 223) {
        printf("Class C\n");
        mask = 24;   // 255.255.255.0
        num_hosts=254;
    } else {
        printf("Invalid IP address class\n");
        return;
    }

    printf("IP address: %d.%d.%d.0\%d\n", octets[0], octets[1], octets[2],octets[3],mask);
    printf("First address: %d.%d.%d.0\n", octets[0], octets[1], octets[2]);
    printf("Last address: %d.%d.%d.255\n", octets[0], octets[1], octets[2]);
    printf("Number of hosts: %d\n", num_hosts);
}

void classlessAddressing(char *ip_address, int mask) {
    // ... (Implementation for classless addressing)
}

int main() {
    char ip_address[16];
    int octets[4];
    int choice, mask;

    printf("Enter IP address: ");
    fgets(ip_address, 16, stdin);
    ip_address[strcspn(ip_address, "\n")] = '\0';  // Remove trailing newline

    printf("Enter 1 for classful addressing, 2 for classless addressing: ");
    scanf("%d", &choice);

    if (choice == 1) {
        classfulAddressing(ip_address);
    } else if (choice == 2) {
        printf("Enter mask value: ");
        scanf("%d", &mask);
        classlessAddressing(ip_address, mask);
    } else {
        printf("Invalid choice\n");
    }

    return 0;
}
