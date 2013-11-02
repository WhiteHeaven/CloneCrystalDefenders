#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);





    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSpriteBatchNode *spritebatch = CCSpriteBatchNode::create( "Mag.png" );
	CCSpriteFrameCache* pcache = CCSpriteFrameCache::sharedSpriteFrameCache();
	pcache->addSpriteFramesWithFile("Mag.plist");
	CCSprite *pSprite1 = CCSprite::createWithSpriteFrameName("Mage_A01.png");
	CCAnimationCache *cache =  CCAnimationCache::sharedAnimationCache();
	cache->addAnimationsWithFile("animations.plist");
	CCAnimation *animation = cache->animationByName("stand_B");
	CCAnimate *animate = CCAnimate::create(animation);
	pSprite1->runAction(CCRepeatForever::create(animate));
	spritebatch->addChild(pSprite1);
	addChild(spritebatch);
	spritebatch->setPosition(ccp(winSize.width/2,winSize.height/2));

	

	
   
    return true;
}


