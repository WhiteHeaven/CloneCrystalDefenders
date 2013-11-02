#include "GameMain.h"
#include "Hero.h"
#include "Waypoint.h"
#include "Enemy.h"


using namespace cocos2d;

CCScene* GameMain::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameMain *layer = GameMain::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

GameMain::GameMain()
{

}

GameMain::~GameMain()
{
	heroBases->release();
	_heroes->release();
	_waypoints->release();
	_enemies->release();
}

// on "init" you need to initialize your instance
bool GameMain::init()
{

	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setTouchEnabled(true);
	CCSize wins = CCDirector::sharedDirector()->getWinSize();
	CCSprite *background = CCSprite::create("Bg1.png");
	this->addChild(background);
	background->setPosition(ccp(wins.width / 2, wins.height / 2));
	background->setScale(0.75);

	this->loadHeroPositions();


	_heroes = CCArray::create();
	_heroes->retain();
	this->addWaypoints();

	wave = 0;
	ui_wave_lbl = CCLabelBMFont::create(CCString::createWithFormat("WAVE: %d", wave)->getCString(), "font_red_14.fnt");
	this->addChild(ui_wave_lbl, 10);
	ui_wave_lbl->setPosition(ccp(400, wins.height - 12));
	ui_wave_lbl->setAnchorPoint(ccp(0, 0.5));

	_enemies = CCArray::create();
	_enemies->retain();
	this->loadWave();

	gameEnded = false;
	playerHp = 5;
	ui_hp_lbl = CCLabelBMFont::create(CCString::createWithFormat("HP: %d",playerHp)->getCString(),"font_red_14.fnt");
	this->addChild(ui_hp_lbl,10);
	ui_hp_lbl->setPosition(ccp(35,wins.height - 12));

	playerGold = 1000;
	ui_gold_lbl = CCLabelBMFont::create(CCString::createWithFormat("GOLD: %d",playerGold)->getCString(),"font_red_14.fnt");
	this->addChild(ui_gold_lbl,10);
	ui_gold_lbl->setPosition(ccp(135, wins.height-12));
	ui_gold_lbl->setAnchorPoint(ccp(0,0.5));

	return true;
}

void GameMain::loadHeroPositions()
{
	CCArray* heroPositions = CCArray::createWithContentsOfFile("HeroesPosition.plist");
	heroBases = CCArray::createWithCapacity(10);
	heroBases->retain();

	CCObject *pObject = NULL;
	CCARRAY_FOREACH(heroPositions, pObject)
	{
		CCDictionary* heroPos = (CCDictionary*)pObject;
		CCSprite* heroBase = CCSprite::create("heroslot.png");
		this->addChild(heroBase);
		heroBase->setPosition(ccp(((CCString*)heroPos->objectForKey("x"))->intValue(),
			((CCString*)heroPos->objectForKey("y"))->intValue()));
		heroBases->addObject(heroBase);
		heroBase->setScale(0.7);
	}
}

bool GameMain::canBuyHero()
{
	if(playerGold - kHERO_COST >= 0)
	{
		return true;
	}
	return false;

}

void GameMain::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter = pTouches->begin();
	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		CCPoint location = pTouch->getLocation();

		CCObject *pObject = NULL;
		CCARRAY_FOREACH(heroBases, pObject)
		{
			/*
			CCSprite *DF =CCSprite::create("DF.png");
			DF->setPosition(ccp(40,40));
			this->addChild(DF);
			*/
			CCSprite *hb = (CCSprite*)pObject;
			if (this->canBuyHero() && hb->boundingBox().containsPoint(location) && !hb->getUserData())
			{

				playerGold -= kHERO_COST;
				ui_gold_lbl->setString(CCString::createWithFormat("GOLD: %d",playerGold)->getCString());

				Hero* hero = Hero::nodeWithTheGame(this, hb->getPosition());
				_heroes->addObject(hero);
				hb->setUserData(hero);
			}           

		}
	}
}

void GameMain::addWaypoints()
{
	_waypoints = CCArray::create();
	_waypoints->retain();

	Waypoint *waypoint1 = Waypoint::nodeWithTheGame(this, ccp(348, 0));
	_waypoints->addObject(waypoint1);

	Waypoint *waypoint2 = Waypoint::nodeWithTheGame(this, ccp(348,234));
	_waypoints->addObject(waypoint2);
	waypoint2->setNextWaypoint(waypoint1);

	Waypoint *waypoint3 = Waypoint::nodeWithTheGame(this, ccp(276, 234));
	_waypoints->addObject(waypoint3);
	waypoint3->setNextWaypoint(waypoint2);

	Waypoint *waypoint4 = Waypoint::nodeWithTheGame(this, ccp(276, 71));
	_waypoints->addObject(waypoint4);
	waypoint4->setNextWaypoint(waypoint3);

	Waypoint *waypoint5 = Waypoint::nodeWithTheGame(this, ccp(150, 71));
	_waypoints->addObject(waypoint5);
	waypoint5->setNextWaypoint(waypoint4);
	
	Waypoint *waypoint6 = Waypoint::nodeWithTheGame(this, ccp(150, 330));
	_waypoints->addObject(waypoint6);
	waypoint6->setNextWaypoint(waypoint5);
}

bool GameMain::collisionWithCircle(CCPoint circlePoint, float radius, CCPoint circlePointTwo, float radiusTwo)
{
	float xdif = circlePoint.x - circlePointTwo.x;
	float ydif = circlePoint.y - circlePointTwo.y;

	float distance = sqrt(xdif * xdif + ydif * ydif);

	if(distance <= radius + radiusTwo)
	{
		return true;
	}

	return false;

}

bool GameMain::loadWave()
{
	CCArray *waveData = CCArray::createWithContentsOfFile("Waves.plist");
    if (wave >= waveData->count())
    {
        return false;
    }
    
    CCArray *currentWaveData = (CCArray*)waveData->objectAtIndex(wave);
    CCObject *pObject = NULL;
    CCARRAY_FOREACH(currentWaveData, pObject)
    {
        CCDictionary* enemyData = (CCDictionary*)pObject;
        Enemy *enemy = Enemy::nodeWithTheGame(this);
        _enemies->addObject(enemy);
        enemy->schedule(schedule_selector(Enemy::doActivate), ((CCString*)enemyData->objectForKey("spawnTime"))->floatValue());
    }

    wave++;
    ui_wave_lbl->setString(CCString::createWithFormat("WAVE: %d", wave)->getCString());
    return true;
}

void GameMain::enemyGotKilled()
{
	if (_enemies->count() <= 0)
    {
        if (!this->loadWave())
        {
            CCLog("You win!");
            CCDirector::sharedDirector()->replaceScene(CCTransitionSplitCols::create(1, GameMain::scene()));
        }       
    } 
}

void GameMain::getHpDamage()
{
	playerHp--;
	  ui_hp_lbl->setString(CCString::createWithFormat("HP: %d", playerHp)->getCString());
	  if(playerHp <=0)
	  {
		  this->doGameOver();
	  }
}

void GameMain::doGameOver()
{
	if (!gameEnded)
	{
		gameEnded = true;
		CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::create(1, GameMain::scene()));
	}
}

void GameMain::awardGold(int gold)
{
	playerGold += gold;
	ui_gold_lbl->setString(CCString::createWithFormat("GOLD: %d", playerGold)->getCString());
}

void GameMain::menuCloseCallback(CCObject* pSender)
{
	this->removeFromParentAndCleanup(true);

}










