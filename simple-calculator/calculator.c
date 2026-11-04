#include <stdio.h>

int main(){
    char welcome[] = "Welcome to simple calc!";
    char add[] = "1)Addition";
    char minus[] = "2)Subtraction";
    char divide[] = "3)Division";
    char multiply[] = "4)Multiply";
    char quit[] = "0)Quit";

int choice = -1;

while (choice !=0){    
printf("%s\n\n", welcome);
printf("%s\n\n", add); 
printf("%s\n\n", minus);
printf("%s\n\n", divide);
printf("%s\n\n", multiply);
printf("%s\n\n", quit);

printf("Enter your choice: ");
scanf("%d", &choice);

if (choice == 0) {
            printf("Bye!\n");
        } 
        else if (choice >= 1 && choice <= 4) {
            printf("You selected option %d\n", choice);
        } 
        else {
            printf("ERROR! Please choose a number between 0 and 4.\n");
        }

}

    return 0;
}
