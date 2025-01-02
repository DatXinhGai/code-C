#include <stdio.h>
#include <math.h>


void canBacK() {
    printf("Nhập n, k:\n");
    int n, k;
    scanf("%d%d", &n, &k);
    double ans = 1;
    int i = 0;
    double temp = 0;
    do {
        temp = 1.0*(k - 1)/k*ans + n/(k*pow(ans, k - 1)) - ans;
        ans += temp;
        i++;
    } while (i < 1000);
    printf("\n");
    printf("Căn bậc %d của %d là: %lf", k, n, ans);

}