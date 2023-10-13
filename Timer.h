#pragma once
#include "stdafx.h"

const ULONG MAX_SAMPLE_COUNT = 50; // 50ȸ�� ������ ó���ð��� �����ؼ� ���
class CTimer
{
public:
	CTimer();
	~CTimer();

public:
	void init();
	void Tick(); // �ð� ����
	void Reset();

	void printFPS(HWND	hWnd);

	float GetTimeElapsed(); // ������ ��� ��� �ð� ��ȯ

private:
	__int64				m_nCurrentTime; // ���� �ð�
	__int64				m_nLastTime;	// ������ �������� �ð�

	float				m_fTimeElapsed; // ������ ������ ���� ������ �ð�
	float				m_fTimeScale;	// Scale Counter�� ��

	float				m_fFrameTime[MAX_SAMPLE_COUNT];	// ������ �ð��� �����ϱ� ���� �迭
	unsigned long		m_nCurrentFrameRate; // ���� ������ ����Ʈ
	float				m_fFPSTimeElapsed; // ������ ����Ʈ ��� �ҿ�ð�
	float				m_nFramesPerSecond; // �ʴ� ������ ��

	__int64				m_nPerformanceFrequency; // ��ǻ���� Performance Frequency
	bool				m_bHardwareHasPerformanceCounter; // ��ǻ���� Performance Counter ���� ����

	unsigned long		m_nSampleCount; // ������ ������ Ƚ��

};

