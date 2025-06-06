#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
 	FILE* fptr1;
	char file[] = "C:/TaiLieuC/hs.txt";
	if ((fptr1 = fopen(file, "r+")) == NULL) {
		puts("Cannot open the file");
		exit(EXIT_FAILURE);
	}
	char c;
	// ungetc chỉ trả về kí tự vào đầu luồng chứ không ghi vào file
	while ((c = fgetc(fptr1)) != EOF) {
		c += 'A' - 'a';
		long pos = ftell(fptr1) - 1;	// vị trí trước đó đã lấy vì sau khi đọc fptr1 tăng lên 1 byte
		fseek(fptr1, pos, SEEK_SET);
		fputc(c, fptr1);
		fseek(fptr1, pos + 1, SEEK_SET);
	}


	/*
	char s[10];
	if (fgets(s, 4, fptr1) == NULL) {
		exit(EXIT_FAILURE);
	}
	*/

	fclose(fptr1);

	return 0;
}