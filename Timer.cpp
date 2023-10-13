#include "Timer.h"

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

void CTimer::init()
{
	// ���� ī���Ϳ� ���� ���ļ��� ����Ͽ� �ð� ���� ����
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPerformanceFrequency))
	{
		m_bHardwareHasPerformanceCounter = TRUE;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
		m_fTimeScale = 1.0f / m_nPerformanceFrequency;
	}
	else  // ���� ī���� �ϵ��� ������ ���� �ʴٸ�
	{
		// ���
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
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime); // ���� �ð� ����
	}
	else
	{
		m_nCurrentTime = timeGetTime();
	}

	// ���������� �� �Լ��� ȣ���� ���� ����� �ð� ���
	float fTimeElapse = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;

	// ���� �ð��� m_nLastTime�� ����
	m_nLastTime = m_nCurrentTime;

	// ������ ������ ó�� �ð��� ���� ������ ó�� �ð��� ���� 1���� ������ ���� ������ ó���ð��� m_fFrameTime[0]�� ����
	if (fabsf(fTimeElapse - m_fTimeElapsed) < 1.f)
	{
		memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_fFrameTime[0] = fTimeElapse;
		if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
	}
	
	// �ʴ� ������ �� 1 ����
	m_nFramesPerSecond++;

	// ���� ������ ó�� �ð� �����Ͽ� ����
	m_fFPSTimeElapsed += fTimeElapse;
	if (m_fFPSTimeElapsed > 1.0f)
	{
		m_nCurrentFrameRate = m_nFramesPerSecond;
		m_nFramesPerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	}

	// ������ ������ ó�� �ð��� ����� ���Ͽ� ������ ó�� �ð� ���

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
