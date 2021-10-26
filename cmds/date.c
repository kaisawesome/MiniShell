#include<stdio.h>
#include<time.h>

int main(){

    time_t date;
    time(&date);
    printf("\n%s\n\n",ctime(&date));

    return 0;
}
