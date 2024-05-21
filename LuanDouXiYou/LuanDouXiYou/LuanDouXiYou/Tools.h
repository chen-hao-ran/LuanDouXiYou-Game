#pragma once

#include <windows.h>
#include <stdio.h>

//��ͼ����
void MyTransparentBlt(HDC hdcDest,      // Ŀ��DC
	int nXOriginDest,   // Ŀ��Xƫ��
	int nYOriginDest,   // Ŀ��Yƫ��
	int nWidthDest,     // Ŀ����
	int nHeightDest,    // Ŀ��߶�
	HDC hdcSrc,         // ԴDC
	int nXOriginSrc,    // ԴX���
	int nYOriginSrc,    // ԴY���
	int nWidthSrc,      // Դ���
	int nHeightSrc,     // Դ�߶�
	UINT crTransparent  // ͸��ɫ,COLORREF����
);

//����λͼ����
void PrintBitmap(HDC hdc, int x, int y, const char* bmpFilePath);

//------------------------------------------------------------------------

void MyTransparentBlt(HDC hdcDest,      // Ŀ��DC
	int nXOriginDest,   // Ŀ��Xƫ��
	int nYOriginDest,   // Ŀ��Yƫ��
	int nWidthDest,     // Ŀ����
	int nHeightDest,    // Ŀ��߶�
	HDC hdcSrc,         // ԴDC
	int nXOriginSrc,    // ԴX���
	int nYOriginSrc,    // ԴY���
	int nWidthSrc,      // Դ���
	int nHeightSrc,     // Դ�߶�
	UINT crTransparent  // ͸��ɫ,COLORREF����
)
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// ��������λͼ
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// ������ɫ����λͼ
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

	// ��ԴDC�е�λͼ��������ʱDC��
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,
			hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// ����͸��ɫ
	SetBkColor(hImageDC, crTransparent);

	// ����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// ����͸������Ϊ��ɫ���������򱣳ֲ����λͼ
	SetBkColor(hImageDC, RGB(0, 0, 0));
	SetTextColor(hImageDC, RGB(255, 255, 255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// ͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
	SetBkColor(hdcDest, RGB(0xff, 0xff, 0xff));
	SetTextColor(hdcDest, RGB(0, 0, 0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// "��"����,��������Ч��
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

void PrintBitmap(HDC hdc, int x, int y, const char* bmpFilePath) {
	// Load the bitmap from file
	HBITMAP hBitmap = (HBITMAP)LoadImage(
		NULL,
		bmpFilePath,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);

	if (hBitmap) {
		// Create a memory device context compatible with the specified device context
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		// Get the bitmap's dimensions
		BITMAP bitmap;
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		// Copy the bitmap from the memory device context to the specified device context
		BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);

		// Restore the old bitmap and delete the memory device context
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);

		// Delete the loaded bitmap
		DeleteObject(hBitmap);
	}
	else {
		// Handle the error if the bitmap could not be loaded
		MessageBox(NULL, "Failed to load bitmap", "Error", MB_OK | MB_ICONERROR);
	}
}