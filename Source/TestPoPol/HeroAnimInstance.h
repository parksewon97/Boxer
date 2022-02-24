// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTPOPOL_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UHeroAnimInstance();

	void PlayAttackMontage();
	
	



private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;


};
