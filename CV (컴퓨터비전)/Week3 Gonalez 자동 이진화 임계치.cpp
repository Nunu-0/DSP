/*
�̸� : �̿���
�а� : �޸������������а�
�й� : 201810793
*/

#pragma warning(disable:4996) // ����� ���� ����
#include <stdio.h>
#include <stdlib.h> // �����Ҵ�
#include <Windows.h> // ��Ʈ�� ��� ���� ����ü(BITMAPFILEHEADER, BITMAPINFOHEADER, RGBQUAD)

// ������׷� ����� (����׷���, ȭ�Ұ� �� �����)
void ObtainHistogram(BYTE *Img, int *Histo, int W, int H) {
	int ImgSize = W * H;

	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
}

// ����ȭ
void Binarization(BYTE *Img, BYTE *Out,int W, int H, BYTE Threshold) {
	int ImgSize = W * H;

	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) { // �Ӱ�ġ���� ������ 0
			Out[i] = 0;
		}
		else { // ũ�� 255
			Out[i] = 255;
		}
	}
}

int GozalezBinThresh(BYTE *Img, int W, int H, int T) {
	int ImgSize = W * H;
	int low = 0, high = 0;
	int lowS = 0, highS = 0;

	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < T) { // T���� ��ο� ��
			lowS += Img[i];
			low++;
		}
		else { // T���� ���� ��
			highS += Img[i];
			high++;
		}
	}
	int Thres = ((lowS / low) + (highS / high)) / 2;


	if (abs(T - Thres) >= 3) {
		return GozalezBinThresh(Img, W, H, Thres);
	}
	else
		return Thres;
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
	// = BYTE Image[ImgSize], �������� ���
	BYTE *Output = (BYTE *)malloc(ImgSize); // ���� ó�� ��� ���
	fread(Image, sizeof(BYTE), ImgSize, fp); // ������ ȭ�� ���� ���
	fclose(fp); //���� �����Ϳ� �������� ���� ���� ����


	//---------------------------------------
	// ���� ó��

	int Histo[256] = { 0 }; // ������׷�
	int min, max;

	ObtainHistogram(Image, Histo, hinfo.biWidth, hinfo.biHeight); // ������׷�
	for (int i = 0; i < 256; i++) {	// ��� �ּҰ�	
		if (Histo[i] > 0) {
			min = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) { // ��� �ִ밪	
		if (Histo[i] > 0) {
			max = i;
			break;
		}
	}

	int T = (min + max) / 2; // ��谪 T �ʱⰪ

	int Thres = GozalezBinThresh(Image, hinfo.biWidth, hinfo.biHeight, T);
	Binarization(Image, Output, hinfo.biWidth, hinfo.biHeight, Thres); // ����ȭ

	//---------------------------------------
	// ó�� ��� ���
	fp = fopen("output.bmp", "wb"); //bmp���� ����, wb = write binary
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); // fwrite(������ ����, ũ��, Ƚ��, ���� ���� ������)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp); // ����� �Ҷ��� 1byte ������ �ؾ���
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp); // ���� ó�� ��� ���
	fclose(fp); // ���������Ϳ� �������� ���� ����

	free(Image); // �����Ҵ� ���� 
	free(Output); // ���������� memory leak ����߻�

	return 0;
}