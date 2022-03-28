// Fill out your copyright notice in the Description page of Project Settings.


#include "TestHEROCharacter.h"
#include"HeroAnimInstance.h"
#include<Engine/Classes/Camera/CameraComponent.h>
#include<Engine/Classes/Components/CapsuleComponent.h>
#include<Engine/Classes/GameFramework/SpringArmComponent.h>
#include<Kismet/KismetMathLibrary.h>
#include<Components/WidgetComponent.h>
#include<SceneCaptureComponent2D.generated.h>
#include"EnemyAIController.h"



//#include "Components/InputComponent.h"
//GetCharacterMovement()->bOrientRotationToMovement �� �����ϱ����� 
//UCharacterMovementComponent Ŭ������ ��ü ����� �� �� �־�� �Ѵ�
//�׷��� ����� ���Խ��״�
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ATestHEROCharacter::ATestHEROCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	
	//������Ʈ ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MiniMapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapCameraSpringArm"));
	

	

	

	//ĳ���� SpringArm 
	SpringArm->SetupAttachment(GetCapsuleComponent());			//Capsule�� ����
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SocketOffset.Z = 100.0f;
	bUseControllerRotationYaw = false;

	//ĳ���� ���� ī�޶�
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	//ĳ���� SpringArm�� ����



	//�̴ϸʿ� SpringArm	
	MiniMapSpringArm->SetupAttachment(GetCapsuleComponent());	//Capsule�� ����
	MiniMapSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-90.0f, 0.0f, 0.0f));
	MiniMapSpringArm->TargetArmLength = 600.0f;
	MiniMapSpringArm->bInheritPitch = false;
	MiniMapSpringArm->bInheritRoll = false;
	MiniMapSpringArm->bInheritYaw = false;


	//ĳ���� �޽� ��ġ ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, 0.0f, 0.0f));

	//ĳ���� �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>HEROMESH(TEXT("/Game/Charater/Hero/Rude_Idle.Rude_Idle"));
	if (HEROMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(HEROMESH.Object);
	}
	
	
	
	
	
	

	//�ִ� �������Ʈ ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>HERO_ANIM(TEXT("/Game/Charater/Hero/Anim/BPAnim.BPAnim_C"));
	if (HERO_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HERO_ANIM.Class);
	}

	//UI ����  ����	(HP���� ������)
	HPBarWidget->SetupAttachment(GetCapsuleComponent());
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 125.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);


	// UIHero �������Ʈ
	static ConstructorHelpers::FClassFinder<UUserWidget>UI_HUD(TEXT("/Game/UI_BP/UIHero.UIHero_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);				//���� Ŭ���� ����
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 150.0f));	//���� ������ ���� �𸮾󿡼� ������ ui�� ũ�� ����� ��

	}
	

	
	
	//ĳ���� ȸ�� ����
	//GetCharacterMovement()->ACharater::CharacterMovement �� ����ȭ �Ѱ��̴�.(CharacterMovement�� private)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	

	IsAttacking = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	


	JapDamage = 10;
	

}

// Called when the game starts or when spawned
void ATestHEROCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestHEROCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestHEROCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//ĳ���� �̵� ������

	PlayerInputComponent->BindAxis("MoveOnFowardAndBack", this, &ATestHEROCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveOnRightAndLeft", this, &ATestHEROCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATestHEROCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ATestHEROCharacter::LookUp);
	//ĳ���� ����
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ATestHEROCharacter::JAB);
	//ĳ���� ����
	InputComponent->BindAction("OnJump", EInputEvent::IE_Pressed, this, &ATestHEROCharacter::Jump);
	//ĳ���� ���� �޸���
	InputComponent->BindAction("FastRunKey", EInputEvent::IE_Pressed, this, &ATestHEROCharacter::PressedSpeedRun);
	InputComponent->BindAction("FastRunKey", EInputEvent::IE_Released, this, &ATestHEROCharacter::ReleasedSpeedRun);
	




	//ĳ���� �����ڼ� ��ȯ



}

void ATestHEROCharacter::MoveForward(float AxisValue)
{
		FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
		AddMovementInput(Direction, AxisValue);
}

void ATestHEROCharacter::MoveRight(float AxisValue)
{
	//AddMovementInput(GetActorRightVector(), AxisValue);
	FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	AddMovementInput(Direction, AxisValue);

}

void ATestHEROCharacter::LookUp(float AxisValue){AddControllerPitchInput(AxisValue);}
void ATestHEROCharacter::Turn(float AxisValue){AddControllerYawInput(AxisValue);}

void ATestHEROCharacter::PressedSpeedRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void ATestHEROCharacter::ReleasedSpeedRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ATestHEROCharacter::JAB()
{
	UE_LOG(LogClass,Warning,TEXT("Attack"));
	
	
	
	
	
	auto AnimInstance = Cast<UHeroAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance)return;
	AnimInstance->PlayAttackMontage();
	
}






