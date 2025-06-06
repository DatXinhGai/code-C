//
// Created by HP LAPTOP on 27/01/2025.
//
/*
6
123456789012 M 1L5C
123456789112 M 3C
123456789012 B 2C
123456789012 B 3C
123456788012 M 1L1C
123456788012 M 2L

OUTPUT
Total Buy: 4L9C
Total Sell: 5C
Top Buyer: 123456788012 3L1C
Top Seller: 123456789012 5C
*/

// tạo 1 hàm riêng chuyển để xử lí các định dạng in ra thôi


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct {
	char ID[13];
	int ML;
	int	MC;
	int BC;
	int BL;
} CC;


void inlc(int l, int c) {
	if (l > 0 && c > 0) {
		printf(" %dL%dC", l, c);
	} else if (l > 0) {
		printf(" %dL", l);
	} else {
		printf(" %dC", c);
	}
}


int banVang() {
	// quả lỗi hài vcl tốt nhất đồng bộ fgets


	int n;
	scanf("%d", &n);
	getchar();


	/*
	char tam[100];
	fgets(tam, 99, stdin);
	int n = atoi(tam);
	*/

	// tổng số mua và bán
	int mual = 0;
	int banl = 0;
	int muac = 0;
	int banc = 0;

	CC *cc = (CC*)malloc(n*sizeof(CC));
	int chiSo = 0;

	for (int i = 0; i < n; i++) {
		char *temp = (char*)malloc(1024);
		fgets(temp, 1023, stdin);
		temp[strcspn(temp, "\r\n")] = '\0';

		char id[13];
		int	ml = 0;
		int	mc = 0;
		int bl = 0;
		int bc = 0;

		if (strchr(temp, 'M')) {
			if (strchr(temp, 'L') && strchr(temp, 'C')) {
				sscanf(temp, "%s M %dL%dC", id, &ml, &mc);
			} else if (strchr(temp, 'L')) {
				sscanf(temp, "%s M %dL", id, &ml);
			} else {
				sscanf(temp, "%s M %dC", id, &mc);
			}
		} else {
			if (strchr(temp, 'L') && strchr(temp, 'C')) {
				sscanf(temp, "%s B %dL%dC", id, &bl, &bc);
			} else if (strchr(temp, 'L')) {
				sscanf(temp, "%s B %dL", id, &bl);
			} else {
				sscanf(temp, "%s B %dC", id, &bc);
			}
		}

		mual += ml;
		muac += mc;
		banl += bl;
		banc += bc;

		int them = 1;
		for (int j = 0; j < chiSo; j++) {
			if (strstr(id, cc[j].ID)) {
				cc[j].BC += bc;
				cc[j].BL += bl;
				cc[j].ML += ml;
				cc[j].MC += mc;
				them = 0;
				break;
			}
		}

		if (them) {
			strcpy(cc[chiSo].ID, id);
			cc[chiSo].BC = bc;
			cc[chiSo].BL = bl;
			cc[chiSo].ML = ml;
			cc[chiSo].MC = mc;
			chiSo++;
		}
		free(temp);
		temp = NULL;
	}


	cc = (CC*)realloc(cc, chiSo*sizeof(CC));

	int csban = -1;
	int csmua = -1;
	int banlm;
	int bancm;
	int mualm;
	int muacm;




	for (int i = 0; i < chiSo; i++) {
		if (cc[i].BC > 0 || cc[i].BL > 0) {
			csban = i;
			banlm = cc[i].BL;
			bancm = cc[i].BC;
			break;
		}
	}

	for (int i = 0; i < chiSo; i++) {
		if (cc[i].ML > 0 || cc[i].MC > 0) {
			csmua = i;
			mualm = cc[i].ML;
			muacm = cc[i].MC;
			break;
		}
	}

	if (csmua != - 1) {
		for (int i = 0; i < chiSo; i++) {
			if (cc[i].ML > mualm || (cc[i].ML == mualm && cc[i].MC > muacm)) {
				mualm = cc[i].ML;
				muacm = cc[i].MC;
				csmua = i;
			}
		}
	}


	if (csban != -1) {
		for (int i = 0; i < chiSo; i++) {
			if (cc[i].BL > banlm || (cc[i].BL == banlm && cc[i].BC > bancm)) {
				banlm = cc[i].BL;
				bancm = cc[i].BC;
				csban = i;
			}
		}
	}


	printf("Total Buy:");
	inlc(mual, muac);
	printf("\n");

	printf("Total Sell:");
	inlc(banl, banc);
	printf("\n");

	printf("Top Buyer:");
	if (csmua != -1) {
		printf(" %s", cc[csmua].ID);
		inlc(cc[csmua].ML, cc[csmua].MC);
	}
	printf("\n");

	printf("Top Seller:");
	if (csban != -1) {
		printf(" %s", cc[csban].ID);
		inlc(cc[csban].BL, cc[csban].BC);
	}
	printf("\n");
	free(cc);
	cc = NULL;
	return 0;
}



