#include "Hero.h"
#include "Enemy.h"
using namespace cocos2d;

Hero::Hero(void)
{
}

Hero::~Hero(void)
{
}

Hero* Hero::nodeWithTheGame(GameMain* game, CCPoint location)
{
	Hero *pRet = new Hero();
	if (pRet && pRet->initWithTheGame(game, location))
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Hero::initWithTheGame(GameMain* game, CCPoint location)
{
	bool bRet = false;
	do 
	{
		attackRange = 70;
		damage = 10;
		fireRate = 1;
		chosenEnemy = NULL;

		_mySprite = CCSpriteBatchNode::create( "Mag.png" );
		CCSpriteFrameCache* pcache = CCSpriteFrameCache::sharedSpriteFrameCache();
		pcache->addSpriteFramesWithFile("Mag.plist");
		CCSprite *pSprite1 = CCSprite::createWithSpriteFrameName("Mage_A01.png");
		CCAnimationCache *cache =  CCAnimationCache::sharedAnimationCache();
		cache->addAnimationsWithFile("animations.plist");
		CCAnimation *animation = cache->animationByName("stand_B");
		CCAnimate *animate = CCAnimate::create(animation);
		pSprite1->runAction(CCRepeatForever::create(animate));
		_mySprite->addChild(pSprite1);
		this->addChild(_mySprite);
		_mySprite->setPosition(location);
		_theGame = game;
		_theGame->addChild(this);

		this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

void Hero::update(float dt)
{
	if(chosenEnemy)
	{
		CCPoint normalized = ccpNormalize(ccp(chosenEnemy->getMySprite()->getPosition().x - _mySprite->getPosition().x,
            chosenEnemy->getMySprite()->getPosition().y - _mySprite->getPosition().y));


		if(!_theGame->collisionWithCircle(_mySprite->getPosition(),attackRange,chosenEnemy->getMySprite()->getPosition(),1))
		{
			this->lostSightOfEnemy();
		}

	}
	else
	{
		CCObject *pObject = NULL;
		CCARRAY_FOREACH(_theGame->getEnemies(),pObject)
		{
			Enemy *enemy = (Enemy*)pObject;
			if(_theGame->collisionWithCircle(_mySprite->getPosition(),attackRange,enemy->getMySprite()->getPosition(),1))
			{
				this->chosenEnemyForAttack(enemy);
				break;
			}
		}
	}
}

void Hero::draw(void)
{
#ifdef COCOS2D_DEBUG
	ccDrawColor4F(255, 255, 255, 255);
	ccDrawCircle(_mySprite->getPosition(), attackRange, 360, 30, false);
#endif
	CCNode::draw();
}

void Hero::attackEnemy()
{
	this->schedule(schedule_selector(Hero::shootWeapon), fireRate);
}

void Hero::chosenEnemyForAttack(Enemy *enemy)
{
	
	chosenEnemy = NULL;
	chosenEnemy = enemy;
	this->attackEnemy();
	enemy->getAttacked(this);
	
}

void Hero::shootWeapon(float dt)
{
	CCSprite *bullet = CCSprite::create("bullet.png");
	_theGame->addChild(bullet);
	bullet->setPosition(_mySprite->getPosition());
	bullet->runAction(CCSequence::create(
		CCMoveTo::create(0.1,chosenEnemy->getMySprite()->getPosition()),
		CCCallFunc::create(this,callfunc_selector(Hero::damageEnemy)),
		CCCallFuncN::create(this,callfuncN_selector(Hero::removeBullet)),
		NULL));
}

void Hero::removeBullet(CCSprite *bullet)
{
	bullet->getParent()->removeChild(bullet, true);
}

void Hero::damageEnemy()
{
	
	if (chosenEnemy)
	{
		chosenEnemy->getDamaged(damage);
	}
	
}

void Hero::targetKilled()
{
	
	if (chosenEnemy)
	{
		chosenEnemy = NULL;
	}

	this->unschedule(schedule_selector(Hero::shootWeapon));
	
}

void Hero::lostSightOfEnemy()
{
	
	chosenEnemy->gotLostSight(this);
	if (chosenEnemy)
	{
		chosenEnemy = NULL;
	}

	this->unschedule(schedule_selector(Hero::shootWeapon));
	
}
