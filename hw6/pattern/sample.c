int MAIN(){
    int n = 10, m = 10;
    int i, j;
    int value[100], w[100], dp[100][100];
    for(i = 0; i < n; i = i + 1){
        for(j = 0; j < m; j = j + 1){
            if((j>=w[i])&&(dp[i][j]<dp[i-1][j-w[i]]+value[i])){
                dp[i][j]=dp[i-1][j-w[i]]+value[i];
            }
        }
    }

}


