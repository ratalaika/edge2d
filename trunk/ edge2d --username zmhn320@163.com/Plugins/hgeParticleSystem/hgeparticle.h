/*
** Haaf's Game Engine 1.5
** Copyright (C) 2003-2004, Relish Games
** hge.relishgames.com
**
** hgeParticleSystem helper class header
*/


#ifndef HGEPARTICLE_H
#define HGEPARTICLE_H


#include <memory.h>
#include "hgevector.h"
#include "hgecolor.h"
#include "hgerect.h"

namespace Edge
{
	class Image;
}

namespace	HGE
{

#define MAX_PARTICLES	500
#define MAX_PSYSTEMS	100

#ifndef M_PI
#define M_PI	3.14159265358979323846f
#define M_PI_2	1.57079632679489661923f
#define M_PI_4	0.785398163397448309616f
#define M_1_PI	0.318309886183790671538f
#define M_2_PI	0.636619772367581343076f
#endif


struct hgeParticle
{
	hgeVector	vecLocation;
	hgeVector	vecVelocity;

	float		fGravity;
	float		fRadialAccel;
	float		fTangentialAccel;

	float		fSpin;
	float		fSpinDelta;

	float		fSize;
	float		fSizeDelta;

	hgeColor	colColor;		// + alpha
	hgeColor	colColorDelta;

	float		fAge;
	float		fTerminalAge;
};

struct hgeParticleSystemInfo
{
	//hgeSprite*	sprite;    // texture + blend mode
	Edge::Image		*sprite;

	int			nEmission; // particles per sec
	float		fLifetime;

	float		fParticleLifeMin;
	float		fParticleLifeMax;

	float		fDirection;
	float		fSpread;
	bool		bRelative;

	float		fSpeedMin;
	float		fSpeedMax;

	float		fGravityMin;
	float		fGravityMax;

	float		fRadialAccelMin;
	float		fRadialAccelMax;

	float		fTangentialAccelMin;
	float		fTangentialAccelMax;

	float		fSizeStart;
	float		fSizeEnd;
	float		fSizeVar;

	float		fSpinStart;
	float		fSpinEnd;
	float		fSpinVar;

	hgeColor	colColorStart; // + alpha
	hgeColor	colColorEnd;
	float		fColorVar;
	float		fAlphaVar;
};

class hgeParticleSystem
{
public:
	hgeParticleSystemInfo info;
	
	hgeParticleSystem(const char *filename, Edge::Image *sprite, float fps=0.0f);
	hgeParticleSystem(hgeParticleSystemInfo *psi, float fps=0.0f);
	hgeParticleSystem(const hgeParticleSystem &ps);
	~hgeParticleSystem() { /*hge->Release(); */}

	hgeParticleSystem&	operator= (const hgeParticleSystem &ps);


	void				Render();
	void				FireAt(float x, float y);
	void				Fire();
	void				Stop(bool bKillParticles=false);
	void				Update(float fDeltaTime);
	void				MoveTo(float x, float y, bool bMoveParticles=false);
	void				Transpose(float x, float y) { fTx=x; fTy=y; }
	void				TrackBoundingBox(bool bTrack) { bUpdateBoundingBox=bTrack; }

	int					GetParticlesAlive() const { return nParticlesAlive; }
	float				GetAge() const { return fAge; }
	void				GetPosition(float *x, float *y) const { *x=vecLocation.x; *y=vecLocation.y; }
	void				GetTransposition(float *x, float *y) const { *x=fTx; *y=fTy; }
	hgeRect*			GetBoundingBox(hgeRect *rect) const { memcpy(rect, &rectBoundingBox, sizeof(hgeRect)); return rect; }

private:
	hgeParticleSystem();
	void				_update(float fDeltaTime);


	float				fUpdSpeed;
	float				fResidue;

	float				fAge;
	float				fEmissionResidue;

	hgeVector			vecPrevLocation;
	hgeVector			vecLocation;
	float				fTx, fTy;

	int					nParticlesAlive;
	hgeRect				rectBoundingBox;
	bool				bUpdateBoundingBox;

	hgeParticle			particles[MAX_PARTICLES];

private:
	static	bool	m_bInitRandom;

	void	InitRandom();

};

class hgeParticleManager
{
public:
	hgeParticleManager(float fps=0.0f);
	~hgeParticleManager();

	void				Update(float dt);
	void				Render();

	hgeParticleSystem*	SpawnPS(hgeParticleSystemInfo *psi, float x, float y);
	bool				IsPSAlive(hgeParticleSystem *ps) const;
	void				Transpose(float x, float y);
	void				GetTransposition(float *dx, float *dy) const {*dx=tX; *dy=tY;}
	void				KillPS(hgeParticleSystem *ps);
	void				KillAll();

private:
	hgeParticleManager(const hgeParticleManager &);
	hgeParticleManager&	operator= (const hgeParticleManager &);

	float				fFPS;
	int					nPS;
	float				tX;
	float				tY;
	hgeParticleSystem*	psList[MAX_PSYSTEMS];
};

}

#endif
