#include "Timer.h"

Timer::Timer() {
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMilisecondsElapsed() {
	if (isrunning) {
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else {
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}

void Timer::Restart() {
	isrunning = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop() {
	if (!isrunning) return false;
	else {
		stop = std::chrono::high_resolution_clock::now();
		isrunning = false;
		return true;
	}
}

bool Timer::Start() {
	if (isrunning) return false;
	else {
		start = std::chrono::high_resolution_clock::now();
		prevTimePoint = std::chrono::high_resolution_clock::now();
		isrunning = true;
		return true;
	}
}

void Timer::timeinfo_Update()
{
	auto now = std::chrono::high_resolution_clock::now();
	Time.time = std::chrono::duration<double>(now - start).count();
	Time.deltaTime = std::chrono::duration<double>(now - prevTimePoint).count();
	prevTimePoint = now;
}

