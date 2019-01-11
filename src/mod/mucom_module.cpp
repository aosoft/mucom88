// MucomModule 
// BouKiCHi 2019

#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "mucom_module.h"

#define MUCOM_DEFAULT_PCMFILE "mucompcm.bin"
#define DEFAULT_OUTFILE "mucom88.mub"

MucomModule::MucomModule() {
  audioRate = 44100;
  pcmfile = MUCOM_DEFAULT_PCMFILE;
  outfile = DEFAULT_OUTFILE;
  voicefile = NULL;
  compileResult = NULL;
}

MucomModule::~MucomModule() {
  Close();
}

void MucomModule::SetRate(int rate) {
  audioRate = rate;
}

void MucomModule::SetPCM(const char *file) {
  pcmfile = file;
}

void MucomModule::SetVoice(const char *file) {
  voicefile = file;
}

bool MucomModule::Open(const char *workingDirectory, const char *songFilename) {
  chdir(workingDirectory);
  mucom = new CMucom();
  int cmpopt = MUCOM_CMPOPT_COMPILE;
  mucom->Init(NULL,cmpopt,audioRate);
  mucom->Reset(cmpopt);
  if (pcmfile) mucom->LoadPCM(pcmfile);
  if (voicefile) mucom->LoadFMVoice(voicefile);
  mucom->CompileFile(songFilename, outfile);

  mucom->PrintInfoBuffer();
  const char *result = mucom->GetMessageBuffer();

  if (compileResult) {
    delete compileResult;
    compileResult = NULL;
  }
  
  compileResult = new char[strlen(result) + 1];
  strcpy(compileResult,result);

  delete mucom;

  // 再生用に再度準備
  mucom = new CMucom();
  cmpopt = MUCOM_CMPOPT_STEP;
  mucom->Init(NULL,cmpopt,audioRate);
  mucom->Reset(0);
  if (mucom->LoadMusic(outfile) < 0) return false;
  return true;
}

bool MucomModule::Play() {
    if (!mucom) return false;
    if (mucom->Play(0) < 0) return false;
    return true;
}

void MucomModule::Close() {
  if (mucom) {
    delete mucom;
    mucom = NULL;
  }
  if (compileResult) {
    delete compileResult;
    compileResult = NULL;
  }
}

const char *MucomModule::GetResult() {
  if (!compileResult) return "";
  return compileResult;
}

void MucomModule::Mix(short *data, int samples) {
	int buf[128];

  int index = 0;
  while(samples > 0) {
      int s = samples < 16 ? samples : 16;
      mucom->RenderAudio(buf, s);

      for(int i=0; i < s*2; i++) {
          int v=buf[i];
          data[index] = v > 32767 ? 32767 : (v < -32768 ? -32768 : v);
          index++;
      }
      samples -= s;
  }
}
