// MucomModule 
// BouKiCHi 2019

#ifndef _MUCOM_MODULE_H_
#define _MUCOM_MODULE_H_

#include "cmucom.h"

class MucomModule  {
public:
    MucomModule();
    ~MucomModule();
    bool Open(const char *workingDirectory, const char *songFilename);
    void SetRate(int rate);
    void SetPCM(const char *file);
    void SetVoice(const char *file);
    void SetOutput(const char *file);
    void Mix(short *buffer, int samples);
    bool Play();
    void Close();
    const char *GetResult();
private:
	CMucom *mucom;
    char *compileResult;
    const char *pcmfile;
    const char *voicefile;
    const char *outfile;
    int audioRate;
};

#endif
