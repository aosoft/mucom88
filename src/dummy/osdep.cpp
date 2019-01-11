// OsDependent 
// BouKiCHi 2019

#include <stdio.h>
#include "osdep.h"

// コンストラクタ
OsDependent::OsDependent() {
	SoundInstance = NULL;
	RealChipInstance = NULL;
	MuteAudio = false;
	UserTimerCallback = new TimerCallback;
	UserAudioCallback = new AudioCallback;
	SoundSending = 0;
}
OsDependent::~OsDependent() {}


bool OsDependent::CoInitialize() {
	return true;
}

// オーディオ
bool OsDependent::InitAudio(void *hwnd, int Rate, int BufferSize) {
	return true;
}

void OsDependent::FreeAudio() {
	SoundInstance = NULL;
}

void OsDependent::WaitSendingAudio() {
}

bool OsDependent::SendAudio() {

	return true;
}

// 実チップ
bool OsDependent::InitRealChip() {
	return false;
}

void OsDependent::FreeRealChip() {
}


int OsDependent::CheckRealChip() {
	return 0;
}

void OsDependent::ResetRealChip() {
}


void OsDependent::OutputRealChip(unsigned int Register, unsigned int Data) {
}


// タイマー
bool OsDependent::InitTimer() {
	return false;
}

void OsDependent::FreeTimer() {
}

void OsDependent::UpdateTimer() {
}

// 時間
int OsDependent::GetMilliseconds() {
	return 0;
}

void OsDependent::Delay(int ms) {
}
