#include "WelcomeLayer.h"
#include "GameScene.h"
#include "GameOverLayer.h"

WelcomeLayer::WelcomeLayer(void)
{
}

WelcomeLayer::~WelcomeLayer(void)
{
}

bool WelcomeLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		//png加入全局cache中
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot_background.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot.plist");

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		//加入background
		CCSprite* background=CCSprite::createWithSpriteFrameName("background.png");
		background->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(background);

		//加入copyright
		CCSprite* copyright=CCSprite::createWithSpriteFrameName("shoot_copyright.png");
		copyright->setAnchorPoint(ccp(0.5,0));
		copyright->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(copyright);

		//加入loading
		CCSprite* loading=CCSprite::createWithSpriteFrameName("game_loading1.png");
		loading->setPosition(ccp(winSize.width/2,winSize.height/2-40));
		this->addChild(loading);

		CCAnimation* animation=CCAnimation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading1.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading2.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading3.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading4.png"));

		CCAnimate* animate=CCAnimate::create(animation);
		CCRepeat* repeat=CCRepeat::create(animate,2);
		CCCallFuncN* repeatdone=CCCallFuncN::create(this,callfuncN_selector(WelcomeLayer::loadingDone));
		CCSequence* sequence=CCSequence::create(repeat,repeatdone,NULL);
		loading->runAction(sequence);

		getHighestHistorySorce();//得到最高的分数

		this->setKeypadEnabled(true);

		bRet=true;
	} while (0);

	return bRet;
}

void WelcomeLayer::loadingDone(CCNode* pNode)
{
	CCScene* pScene=GameScene::create();
	CCTransitionMoveInB *animateScene=CCTransitionMoveInB::create(0.5f,pScene);
	CCDirector::sharedDirector()->replaceScene(animateScene);
}

void WelcomeLayer::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}

//读取本地的分数，如果没有就初始化一个
bool WelcomeLayer::isHaveSaveFile()
{
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFileXml"))
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFileXml", true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",0);
		CCUserDefault::sharedUserDefault()->flush();
		return false;
	}
	else
	{
		return true;
	}
}


//读取本地的分数
void WelcomeLayer::getHighestHistorySorce()
{
	if (isHaveSaveFile())
	{
		int hscore=CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore",0);
		GameOverLayer::highestHistoryScore=hscore;//打开游戏时取得最高分数，初始化给GameOverlayer中的highestHistoryScore
		CCString* strHighestScore=CCString::createWithFormat("%d",hscore);
	}
}