/*
이름 : 이연우
학과 : 휴먼지능정보공학과
학번 : 201810793
*/

#pragma warning(disable:4996) // 입출력 에러 막기
#include <stdio.h>
#include <stdlib.h> // 동적할당
#include <Windows.h> // 비트맵 헤더 관련 구조체(BITMAPFILEHEADER, BITMAPINFOHEADER, RGBQUAD)
int main() {
	// -----------------------------------
	// 영상 입력
	BITMAPFILEHEADER hf; // 14 bytes
	BITMAPINFOHEADER hinfo; // 40 bytes
	RGBQUAD hRGB[256]; // 4 * 256 = 1024 bytes

	FILE *fp; // 파일 오픈을 위한 파일 포인터 변수
	fp = fopen("lenna.bmp", "rb"); // 파일 오픈, 읽기 위해 rb(read binary) , 이미지 파일은 전부 binary
	if (fp == NULL) { // 파일 포인터가 파일을 가르키지 않고있다면
		printf("File not found!/n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); // fread로 파일 읽기
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp); // fread(저장할 변수, 크기, 횟수, 현재 파일 포인터)
	fread(hRGB, sizeof(RGBQUAD), 256, fp); // 배열이름 자체가 주소라 &사용 x
	int ImgSize = hinfo.biWidth * hinfo.biHeight; // 영상의 화소(인포헤더에서 가로, 세로 정보 가져오기)
	BYTE *Image = (BYTE *)malloc(ImgSize); // 동적할당, BYTE(0 ~ 255)
	// = BYTE Image[ImgSize], 원본영상 담기
	BYTE *Output2 = (BYTE *)malloc(ImgSize); // 밝기값 50 증가한 영상, 영상 처리 결과 담기
	BYTE *Output3 = (BYTE *)malloc(ImgSize); // 원본 영상의 반전
	fread(Image, sizeof(BYTE), ImgSize, fp); // 영상의 화수 정보 담기
	fclose(fp); //파일 포인터와 영성파일 간의 연결 끊기

	//---------------------------------------
	// 영상 처리
	//output1
	// Image = Output1

	//output2
	for (int i = 0; i < ImgSize; i++) { //원본 영상의 밝기값 50 증가
		Output2[i] = Image[i] + 50; // (255보다 큰 값이 나오면 overflow(검하게 나옴))
	}

	//output3
	for (int i = 0; i < ImgSize; i++) { //원본 영상의 반전 결과 영상
		Output3[i] = 255 - Image[i];
	}

	//---------------------------------------
	// 처리 결과 출력
	// output1.bmp
	fp = fopen("output1.bmp", "wb"); //bmp파일 생성, wb = write binary
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); // fwrite(저장할 변수, 크기, 횟수, 현재 파일 포인터)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp); // 기록을 할때는 1byte 단위로 해야함
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Image, sizeof(BYTE), ImgSize, fp); // 영상 처리 결과 출력
	fclose(fp); // 파일포인터와 영상파일 관계 끊기

	// output2.bmp
	fp = fopen("output2.bmp", "wb"); //bmp파일 생성, wb = write binary
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); // fwrite(저장할 변수, 크기, 횟수, 현재 파일 포인터)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp); // 기록을 할때는 1byte 단위로 해야함
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output2, sizeof(BYTE), ImgSize, fp); // 영상 처리 결과 출력
	fclose(fp); // 파일포인터와 영상파일 관계 끊기

	// output3.bmp
	fp = fopen("output3.bmp", "wb"); //bmp파일 생성, wb = write binary
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); // fwrite(저장할 변수, 크기, 횟수, 현재 파일 포인터)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp); // 기록을 할때는 1byte 단위로 해야함
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output3, sizeof(BYTE), ImgSize, fp); // 영상 처리 결과 출력
	fclose(fp); // 파일포인터와 영상파일 관계 끊기

	free(Image); // 동적할당 해제
	free(Output2); // 하지않으면 memory leak 현상발생
	free(Output3);

	return 0;
}
