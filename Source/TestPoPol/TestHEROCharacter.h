// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestHEROCharacter.generated.h"

UCLASS()
class TESTPOPOL_API ATestHEROCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestHEROCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float AxisValue);

	UFUNCTION()
		void MoveRight(float AxisValue);

	UFUNCTION()
		void LookUp(float AxisValue);

	UFUNCTION()
		void Turn(float AxisValue);

	UFUNCTION()
		void PressedSpeedRun();

	UFUNCTION()
		void ReleasedSpeedRun();

	UFUNCTION()
		void JAB();




	


	UPROPERTY(VisibleAnywhere,Category=Camera)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* MiniMapSpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere,Category=UI)
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float JapDamage;
	


	

	

	

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		bool IsAttacking;


};


