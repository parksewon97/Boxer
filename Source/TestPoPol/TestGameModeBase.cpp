// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"
#include"TestHEROCharacter.h"

ATestGameModeBase::ATestGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn>DefaultChareaterTest(TEXT("/Game/BPTypeChar/CPP_BP_HERO.CPP_BP_HERO_C"));						//cpp�� ���� ���ΰ�:	/Game/BPTypeChar/CPP_BP_HERO.CPP_BP_HERO_C		,	�������Ʈ�� ����  ���ΰ�: /Game/BPTypeChar/BPHero.BPHero_C
	if (DefaultChareaterTest.Succeeded())
	{
		DefaultPawnClass = DefaultChareaterTest.Class;				// DefaultChareaterTest.Class;																					//C++������ Hero�� 	�������Ʈ�� �Ļ���Ų�� DefaultPawnClass����	( �ӽ÷� �������Ʈ Hero��� )						//ATestHEROCharacter::StaticClass();		//c++�θ� �ۼ��ߴٸ� �ش� �ڵ� ���
	}
	
	
		
}


