#include <stdio.h>

int main() {
    char welcome[] = "Welcome to simple calc!\n";
    char add[] = "1)Addition";
    char minus[] = "2)Subtraction";
    char divide[] = "3)Division";
    char multiply[] = "4)Multiply";
    char quit[] = "0)Quit";

    int choice = -1;
    double num1, num2, result;

    while (choice != 0) {    
        printf("%s\n", welcome);
        printf("%s\n", add); 
        printf("%s\n", minus);
        printf("%s\n", divide);
        printf("%s\n", multiply);
        printf("%s\n", quit);

        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Bye!\n");
        } 
        else if (choice >= 1 && choice <= 4) {
            printf("You selected option %d\n", choice);
            printf("Enter first number: ");
            scanf("%lf", &num1);
            printf("Enter second number: ");
            scanf("%lf", &num2);

            switch(choice) {
                case 1:
                    result = num1 + num2;
                    printf("Result: %g\n\n", result);
                    break;
                case 2:
                    result = num1 - num2;
                    printf("Result: %g\n\n", result);
                    break;
                case 3:
                    if (num2 == 0) {
                        printf("ERROR! Division by zero is not allowed.\n");
                    } else {
                        result = num1 / num2;
                        printf("Result: %g\n\n", result);
                    }
                    break;
                case 4:
                    result = num1 * num2;
                    printf("Result: %g\n\n", result);
                    break;
            }
        } 
        else {
            printf("INVALID CHOICE! Please choose a number between 0 and 4.\n");
        }
    }

    return 0;
}

