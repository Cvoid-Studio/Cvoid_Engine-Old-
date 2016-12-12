#include "CVGraph2D.h"

void SpriteClass2D::CV_PlaySound(char* FilePath)
{
	M_DSBuffer8->SetVolume(Sound_Volume);
}

void SpriteClass2D::CV_PlayMusic(char* FilePath, bool isLoop)
{
	char* temp_ptr;
	DWORD size = 0;
	fread(buf, 1, size, fopen(FilePath, "r"));

	if (SUCCEEDED(M_DSBuffer8->Lock(0, 0, (void**)&temp_ptr, &size, 0, 0, DSBLOCK_ENTIREBUFFER)))
	{
		ZeroMemory(&temp_ptr, sizeof(temp_ptr));
	}

	M_DSBuffer8->Unlock(temp_ptr, size, NULL, 0);

	M_DSBuffer8->SetCurrentPosition(0);
	M_DSBuffer8->SetVolume(Music_Volume);
	M_DSBuffer8->Play(0, 0, DSBPLAY_LOOPING);
}