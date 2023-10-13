#include "Timer.h"

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

void CTimer::init()
{
	// 성능 카운터와 성능 주파수를 사용하여 시간 단위 설정
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPerformanceFrequency))
	{
		m_bHardwareHasPerformanceCounter = TRUE;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
		m_fTimeScale = 1.0f / m_nPerformanceFrequency;
	}
	else  // 성능 카운터 하드웨어를 가지고 있지 않다면
	{
		// 경고문
		m_bHardwareHasPerformanceCounter = FALSE;
		m_nLastTime = timeGetTime();
		m_fTimeScale = 0.001f;
	}
	
	m_nSampleCount = 0;
	m_nCurrentFrameRate = 0;
	m_nFramesPerSecond = 0;
	m_fFPSTimeElapsed = 0.f;
}

void CTimer::Tick()
{
	if (m_bHardwareHasPerformanceCounter)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime); // 현재 시간 갱신
	}
	else
	{
		m_nCurrentTime = timeGetTime();
	}

	// 마지막으로 이 함수를 호출한 이후 경과한 시간 계산
	float fTimeElapse = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;

	// 현재 시간을 m_nLastTime에 저장
	m_nLastTime = m_nCurrentTime;

	// 마지막 프레임 처리 시간과 현재 프레임 처리 시간의 차가 1보다 작으면 현재 프레임 처리시간을 m_fFrameTime[0]에 저장
	if (fabsf(fTimeElapse - m_fTimeElapsed) < 1.f)
	{
		memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_fFrameTime[0] = fTimeElapse;
		if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
	}
	
	// 초당 프레임 수 1 증가
	m_nFramesPerSecond++;

	// 현재 프레임 처리 시간 누적하여 저장
	m_fFPSTimeElapsed += fTimeElapse;
	if (m_fFPSTimeElapsed > 1.0f)
	{
		m_nCurrentFrameRate = m_nFramesPerSecond;
		m_nFramesPerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	}

	// 누적된 프레임 처리 시간의 평균을 구하여 프레임 처리 시간 계산

	m_fTimeElapsed = 0.f;
	for (ULONG i = 0; i < m_nSampleCount; i++) m_fTimeElapsed += m_fFrameTime[i];
	if (m_nSampleCount > 0) m_fTimeElapsed /= m_nSampleCount;

}

void CTimer::Reset()
{
	__int64 nPerformanceCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);

	m_nLastTime = nPerformanceCounter;
	m_nCurrentTime = nPerformanceCounter;

}

void CTimer::printFPS(HWND	hWnd)
{
	_TCHAR m_pszFrameRate[50];
	::_itow_s(m_nCurrentFrameRate, (m_pszFrameRate), 37, 10);
	wcscat_s((m_pszFrameRate), 37, _T(" FPS"));

	SetWindowText(hWnd, m_pszFrameRate);
}

float CTimer::GetTimeElapsed()
{
	return m_fTimeElapsed;
}
