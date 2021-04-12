/*
�̸� : �̿���
�а� : �޸������������а�
�й� : 201810793
*/

#pragma warning(disable:4996) // ����� ���� ����
#include <stdio.h>
#include <stdlib.h> // �����Ҵ�
#include <Windows.h> // ��Ʈ�� ��� ���� ����ü(BITMAPFILEHEADER, BITMAPINFOHEADER, RGBQUAD)

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hinfo, RGBQUAD *hRGB, BYTE *Output, int W, int H, const char *FileName) {
	FILE *fp = fopen(FileName, "wb"); //bmp���� ����, wb = write binary
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); // fwrite(������ ����, ũ��, Ƚ��, ���� ���� ������)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp); // ����� �Ҷ��� 1byte ������ �ؾ���
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W * H, fp); // ���� ó�� ��� ���
	fclose(fp); // ���������Ϳ� �������� ���� ����

}

int main() {
	// -----------------------------------
	// ���� �Է�
	BITMAPFILEHEADER hf; // 14 bytes
	BITMAPINFOHEADER hinfo; // 40 bytes
	RGBQUAD hRGB[256]; // 4 * 256 = 1024 bytes

	FILE *fp; // ���� ������ ���� ���� ������ ����
	fp = fopen("lenna.bmp", "rb"); // ���� ����, �б� ���� rb(read binary) , �̹��� ������ ���� binary
	if (fp == NULL) { // ���� �����Ͱ� ������ ����Ű�� �ʰ��ִٸ�
		printf("File not found!/n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); // fread�� ���� �б�
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp); // fread(������ ����, ũ��, Ƚ��, ���� ���� ������)
	fread(hRGB, sizeof(RGBQUAD), 256, fp); // �迭�̸� ��ü�� �ּҶ� &��� x
	int ImgSize = hinfo.biWidth * hinfo.biHeight; // ������ ȭ��(����������� ����, ���� ���� ��������)
	BYTE *Image = (BYTE *)malloc(ImgSize); // �����Ҵ�, BYTE(0 ~ 255)
	BYTE *Output = (BYTE *)malloc(ImgSize); // ��Ⱚ 50 ������ ����, ���� ó�� ��� ���
	fread(Image, sizeof(BYTE), ImgSize, fp); // ������ ȭ�� ���� ���
	fclose(fp); //���� �����Ϳ� �������� ���� ���� ����

	//---------------------------------------
	// ���� ó�� ��� ���
	SaveBMPFile(hf, hinfo, hRGB, Image, hinfo.biWidth, hinfo.biHeight, "output1.bmp");

	for (int i = 0; i < ImgSize; i++) { //���� ������ ��Ⱚ 50 ����
		Output[i] = Image[i] + 50; // (255���� ū ���� ������ overflow(���ϰ� ����))
	}
	SaveBMPFile(hf, hinfo, hRGB, Output, hinfo.biWidth, hinfo.biHeight, "output2.bmp");
	
	//output3
	for (int i = 0; i < ImgSize; i++) { //���� ������ ���� ��� ����
		Output[i] = 255 - Image[i];
	}
	SaveBMPFile(hf, hinfo, hRGB, Output, hinfo.biWidth, hinfo.biHeight, "output3.bmp");
	//---------------------------------------
	// ������

	free(Image); // �����Ҵ� ���� 
	free(Output); // ���������� memory leak ����߻�

	return 0;
}