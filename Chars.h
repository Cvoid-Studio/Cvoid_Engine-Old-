#pragma once
#ifndef CHARS_H_
#define CHARS_H_


#define LIMITBulletR	3280
#define LIMITBulletS	1024
#define LIMITShot1		16
#define LIMITShot2		32
#define LIMITItem		512

#define Reimu_SPD		1.8f
#define Reimu_LowSPD	0.8f
#define Reimu_ShootSPD	30.f	//Per Milisecond

#define Marisa_SPD		2.2f
#define Marisa_LowSPD	1.2f
#define Marisa_ShootSPD	27.f

#define Mima_SPD		2.5f
#define Mima_LowSPD		1.3f
#define Mima_ShootSPD	25.f

#define	Youka_SPD		1.6f
#define	Youka_LowSPD	0.8f
#define Youka_ShootSPD	30.f

struct BulletMapR
{
	float	X;
	float	Y;
	bool	exist = false;
	float	distance;
	float	CriDist;
};

struct BulletMapS
{
	float	X;
	float	Y;
	bool	exist = false;
	float	distance;
	float	CriDistX;
	float	CriDistY;
};



struct G_MainChar
{
	G_MainChar() {
			memset(Bullet1LBuffer, 0, sizeof(BulletMapS) * LIMITShot1);
			memset(Bullet1RBuffer, 0, sizeof(BulletMapS) * LIMITShot1);
			memset(Bullet2LBuffer, 0, sizeof(BulletMapS) * LIMITShot2);
			memset(Bullet2RBuffer, 0, sizeof(BulletMapS) * LIMITShot2);
	};
	~G_MainChar() {};

	float posX = 894.f;
	float posY = 150.f;
	int rotateX = 0;
	int rotateY = 0;

	bool  Moving;
	bool  LowSpd;
	bool  Alive;
	bool  Shooting;

	int Weight[4]; // 0-UP 1-RIGHT 2-DOWN 3-LEFT

	unsigned int Energy = 0;
	unsigned int BulletNum = 0;
	
	BulletMapS Bullet1LBuffer[LIMITShot1];
	BulletMapS Bullet1RBuffer[LIMITShot1];
	BulletMapS Bullet2LBuffer[LIMITShot2];
	BulletMapS Bullet2RBuffer[LIMITShot2];

	int movStatus = 0;

	void InitChar() { 
		memset(Weight, 0, 4 * sizeof(int));
		rotateX = rotateY = 0;
		Moving = false;
		LowSpd = false;
		Shooting=false; };
};



#endif