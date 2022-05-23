//
// Created by priskn on 07/05/22.
//

#include <iostream>
#include <cstring>
#include "UDPRobot.h"





void UDPRobot::gestion(char* chaine){
    cout<<"<<Chaine>> : "<<chaine<<endl;
    if(chaine[0]=='P'){
        char* str = (char*) malloc(255);
        for(int i=2;i<strlen(chaine);i++){
            str[i-2]=chaine[i];
        }
        cout<<"Points ajoutés : "<<str<<endl;
    }
    else if(chaine[0]=='S'){
        char* str = (char*) malloc(255);
        for(int i=2;i<strlen(chaine);i++){
            str[i-2]=chaine[i];
        }
        cout<<"Stratégie : "<<str<<endl;
    }
    else if(chaine[0]=='D'){
        cout<<"signal de depart"<<endl;
        //appel de la fonction de depart
    }
    else{
        printf("chaine non valide");
    }
}
