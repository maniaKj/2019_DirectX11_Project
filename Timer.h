#pragma once
#include <chrono>

struct TimeInfo {
public:
	float time;
	float deltaTime;
};

class Timer {
public:
	Timer();
	double GetMilisecondsElapsed();
	void Restart();
	bool Stop();
	bool Start();
	void timeinfo_Update();

	TimeInfo Time;
private:
	bool isrunning = false;
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
	std::chrono::time_point<std::chrono::steady_clock> prevTimePoint;

#else //������ ȯ���� �ƴ� ��, ex ������
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> stop;
	std::chrono::time_point<std::chrono::system_clock> prevTimePoint;
#endif
};