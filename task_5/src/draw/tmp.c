#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* tmp file exp fix in draw */

void formatExponential(double number) {
    char buffer[50];
    sprintf(buffer, "%.5e", number);

    char* ePosition = strchr(buffer, 'e');
    if (ePosition != NULL) {
        *ePosition = '\0';
        printf("%s*10^(%s)\n", buffer, ePosition + 1);
    } else {
        printf("%s\n", buffer);
    }
}

int main() {
    double number = -3.28502e+40;
    formatExponential(number);
    return 0;
}
