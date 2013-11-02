#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include "GameMain.h"

#define kHERO_COST 300

class Enemy;

class Hero : public cocos2d::CCNode
{
public:
	Hero(void);
	~Hero(void);

	static Hero* nodeWithTheGame(GameMain* game, cocos2d::CCPoint location);
	bool initWithTheGame(GameMain* game, cocos2d::CCPoint location);
	void attackEnemy();
	void chosenEnemyForAttack(Enemy *enemy);
	void shootWeapon(float dt);
	void removeBullet(cocos2d::CCSprite *bullet);
	void damageEnemy();
	void targetKilled();
	void lostSightOfEnemy();

	void update(float dt);
	void draw(void);

	CC_SYNTHESIZE(GameMain*, _theGame, TheGame);
	CC_SYNTHESIZE(cocos2d::CCSpriteBatchNode*, _mySprite, MySprite);

private:
	int attackRange;
	int damage;
	float fireRate;
	bool attacking;
	Enemy *chosenEnemy;
};

#endif  // __TOWER_H__