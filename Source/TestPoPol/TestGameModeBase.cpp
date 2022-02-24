// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"
#include"TestHEROCharacter.h"

ATestGameModeBase::ATestGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn>DefaultChareaterTest(TEXT("/Game/BPTypeChar/CPP_BP_HERO.CPP_BP_HERO_C"));						//cpp로 만든 주인공:	/Game/BPTypeChar/CPP_BP_HERO.CPP_BP_HERO_C		,	블루프린트로 만든  주인공: /Game/BPTypeChar/BPHero.BPHero_C
	if (DefaultChareaterTest.Succeeded())
	{
		DefaultPawnClass = DefaultChareaterTest.Class;				// DefaultChareaterTest.Class;																					//C++제작한 Hero를 	블루프린트로 파생시킨뒤 DefaultPawnClass설정	( 임시로 블루프린트 Hero사용 )						//ATestHEROCharacter::StaticClass();		//c++로만 작성했다면 해당 코드 사용
	}
	
	
		
}


