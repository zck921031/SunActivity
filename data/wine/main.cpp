#include<bits/stdc++.h>
int main(){
    FILE *fin, *ftrain, *ftest, *fout;
    fin = fopen("wine.data", "r");
    ftrain = fopen("wine_train.data", "w");
    ftest = fopen("wine_test.data", "w");
    int cnt = 0;
    char str[10240];
    while( fgets(str, 10240, fin) ){
        ++cnt;
        if ( cnt%5 != 0 ) fout = ftrain;
        else fout = ftest;
        fputs(str, fout);
    }
    fclose(fin);
    fclose(ftrain);
    fclose(ftest);
    return 0;
}
