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
//GetCharacterMovement()->bOrientRotationToMovement 에 접근하기위해 
//UCharacterMovementComponent 클래스의 전체 모양을 알 수 있어야 한다
//그래서 헤더를 포함시켰다
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ATestHEROCharacter::ATestHEROCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	
	//컴포넌트 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MiniMapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapCameraSpringArm"));
	

	

	

	//캐릭터 SpringArm 
	SpringArm->SetupAttachment(GetCapsuleComponent());			//Capsule에 장착
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SocketOffset.Z = 100.0f;
	bUseControllerRotationYaw = false;

	//캐릭터 전용 카메라
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	//캐릭터 SpringArm에 장착



	//미니맵용 SpringArm	
	MiniMapSpringArm->SetupAttachment(GetCapsuleComponent());	//Capsule에 장착
	MiniMapSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-90.0f, 0.0f, 0.0f));
	MiniMapSpringArm->TargetArmLength = 600.0f;
	MiniMapSpringArm->bInheritPitch = false;
	MiniMapSpringArm->bInheritRoll = false;
	MiniMapSpringArm->bInheritYaw = false;


	//캐릭터 메쉬 위치 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, 0.0f, 0.0f));

	//캐릭터 메쉬 장착
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>HEROMESH(TEXT("/Game/Charater/Hero/Rude_Idle.Rude_Idle"));
	if (HEROMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(HEROMESH.Object);
	}
	
	
	
	
	
	

	//애님 블루프린트 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>HERO_ANIM(TEXT("/Game/Charater/Hero/Anim/BPAnim.BPAnim_C"));
	if (HERO_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HERO_ANIM.Class);
	}

	//UI 위젯  장착	(HP위젯 장착용)
	HPBarWidget->SetupAttachment(GetCapsuleComponent());
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 125.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);


	// UIHero 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget>UI_HUD(TEXT("/Game/UI_BP/UIHero.UIHero_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);				//위젯 클래스 설정
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 150.0f));	//위젯 사이즈 설정 언리얼에서 제작한 ui와 크기 맞춰야 함

	}
	

	
	
	//캐릭터 회전 세팅
	//GetCharacterMovement()->ACharater::CharacterMovement 를 은닉화 한것이다.(CharacterMovement는 private)
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

	//캐릭터 이동 구현부

	PlayerInputComponent->BindAxis("MoveOnFowardAndBack", this, &ATestHEROCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveOnRightAndLeft", this, &ATestHEROCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATestHEROCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ATestHEROCharacter::LookUp);
	//캐릭터 공격
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ATestHEROCharacter::JAB);
	//캐릭터 점프
	InputComponent->BindAction("OnJump", EInputEvent::IE_Pressed, this, &ATestHEROCharacter::Jump);
	//캐릭터 빨리 달리기
	InputComponent->BindAction("FastRunKey", EInputEvent::IE_Pressed, this, &ATestHEROCharacter::PressedSpeedRun);
	InputComponent->BindAction("FastRunKey", EInputEvent::IE_Released, this, &ATestHEROCharacter::ReleasedSpeedRun);
	




	//캐릭터 공격자세 변환



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






