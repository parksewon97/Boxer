// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"


UHeroAnimInstance::UHeroAnimInstance()
{
	

	

	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("/Game/Charater/Hero/Anim/Jab_Anim__Montage.Jab_Anim__Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;

		
		
		

	}

}

void UHeroAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}

}
