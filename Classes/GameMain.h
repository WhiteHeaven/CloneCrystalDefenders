#ifndef __GAME_MAIN_H__
#define __GAME_MAIN_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class GameMain : public cocos2d::CCLayer
{
public:
	GameMain();
	~GameMain();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(GameMain);

	void loadHeroPositions();
	bool canBuyHero();
	void addWaypoints();
	bool collisionWithCircle(cocos2d::CCPoint circlePoint, float radius, cocos2d::CCPoint circlePointTwo, float radiusTwo);
	bool loadWave();
	void enemyGotKilled();
	void getHpDamage();
	void doGameOver();
	void awardGold(int gold);

	void menuCloseCallback(CCObject* pSender);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _heroes, Heroes);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _waypoints, Waypoints);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _enemies, Enemies);
	
private:
	cocos2d::CCArray* heroBases;
	int wave;
	cocos2d::CCLabelBMFont* ui_wave_lbl;
	int playerHp;
	cocos2d::CCLabelBMFont *ui_hp_lbl;
	bool gameEnded;
	int playerGold;
	cocos2d::CCLabelBMFont *ui_gold_lbl;
};

#endif  // __HELLOWORLD_SCENE_H__