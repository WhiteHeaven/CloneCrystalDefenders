#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "GameMain.h"
#include "Waypoint.h"
#include "Hero.h"

class Enemy : public cocos2d::CCNode
{
public:
	Enemy(void);
	~Enemy(void);

	static Enemy* nodeWithTheGame(GameMain* game);
	bool initWithTheGame(GameMain* game);
	void doActivate(float dt);
	void getRemoved();
	void getAttacked(Hero* attacker);
	void gotLostSight(Hero* attackere);
	void getDamaged(int damage);
	void update(float dt);
	void draw(void);

	CC_SYNTHESIZE(GameMain*, _theGame,TheGame);
	CC_SYNTHESIZE(cocos2d::CCSprite*,_mySprite,MySprite);

private:
	cocos2d::CCPoint myPosition;
	int maxHp;
	int currentHp;
	float walkingSpeed;
	Waypoint *destinationWaypoint;
	bool active;
	cocos2d::CCArray *attackedBy;
};

#endif