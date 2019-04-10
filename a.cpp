#include "stdio.h"

#include "stdlib.h"

int main()

{

    int a[3][3];

    int		*i, j, k, check,n;

	int		sum1, sum2, rsum[3], csum[3]; 



	n = 3;

	

	i = &(a[0][0]);		



	for (i[0] = 1; i[0] <= 9; i[0]++) {

		for (i[1] = 1; i[1] <= 9; i[1]++) {

			if (i[1] == i[0])

				continue;

			for (i[2] = 1; i[2] <= 9; i[2]++) {

				if (i[2]==i[1] || i[2]== i[0])

					continue;

				for (i[3] = 1; i[3] <= 9; i[3]++) {

					if (i[3]==i[2] || i[3]== i[1] || i[3] == i[0])

						continue;

					for (i[4] = 1; i[4] <= 9; i[4]++) {

						if (i[4]==i[3]||i[4]==i[2] ||i[4]== i[1] || i[4] == i[0])

							continue;

						for (i[5] = 1; i[5] <= 9; i[5]++) {

							if (i[5]==i[4]|| i[5] == i[3]|| i[5] == i[2]|| i[5] == i[1]|| i[5] == i[0])

								continue;

							for (i[6] = 1; i[6] <= 9; i[6]++) {

								if (i[6]==i[5]|| i[6] == i[4] || i[6] == i[3] || i[6] == i[2] || i[6] == i[1] || i[6] == i[0])

									continue;

								for (i[7] = 1; i[7] <= 9; i[7]++) {

									if (i[7] == i[6] || i[7] == i[5] || i[7] == i[4] || i[7] == i[3] || i[7] == i[2] || i[7] == i[1] || i[7] == i[0])

										continue;

									for (i[8] = 1; i[8] <= 9; i[8]++) {

										if (i[8] == i[7] || i[8] == i[6] || i[8] == i[5] || i[8] == i[4] || i[8] == i[3] || i[8] == i[2] || i[8] == i[1] || i[8] == i[0])

											continue;



										for (j = 0; j < 3; j++) {

											csum[j] = 0;

											rsum[j] = 0;

										}

										sum1 = 0;

										sum2 = 0;



										for (j = 0; j < 3; j++) {

											for (k = 0; k < 3; k++) {

												csum[k] += a[j][k];

												rsum[j] += a[j][k];

											}

											sum1 += a[j][j];

											sum2 += a[j][2 - j];

										}



									

										if (sum1 != sum2)

											continue;

										check = 0;

										for (j = 0; j < 3; j++) {

											if (csum[j] != sum1 || rsum[j] != sum1) {

												check = 1;

												break;

											}

										}



										if (check == 0) {

											for (j = 0; j < 3; j++) {

												for (k = 0; k < 3; k++) {

													printf("%d ", a[j][k]);

												}

												printf("\n");

											}

											

											printf("\n");

										}

								

									}

								}

							}

						}

					}

				}

			}

		}

	}

system ("pause");

	return 0;

}
