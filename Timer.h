#pragma once
#include "stdafx.h"

const ULONG MAX_SAMPLE_COUNT = 50; // 50회의 프레임 처리시간을 누적해서 평균
class CTimer
{
public:
	CTimer();
	~CTimer();

public:
	void init();
	void Tick(); // 시간 갱신
	void Reset();

	void printFPS(HWND	hWnd);

	float GetTimeElapsed(); // 프레임 평균 경과 시간 반환

private:
	__int64				m_nCurrentTime; // 현재 시간
	__int64				m_nLastTime;	// 마지막 프레임의 시간

	float				m_fTimeElapsed; // 마지막 프레임 이후 지나간 시간
	float				m_fTimeScale;	// Scale Counter의 양

	float				m_fFrameTime[MAX_SAMPLE_COUNT];	// 프레임 시간을 누적하기 위한 배열
	unsigned long		m_nCurrentFrameRate; // 현재 프레임 레이트
	float				m_fFPSTimeElapsed; // 프레임 레이트 계산 소요시간
	float				m_nFramesPerSecond; // 초당 프레임 수

	__int64				m_nPerformanceFrequency; // 컴퓨터의 Performance Frequency
	bool				m_bHardwareHasPerformanceCounter; // 컴퓨터의 Performance Counter 소유 여부

	unsigned long		m_nSampleCount; // 누적된 프레임 횟수

};

