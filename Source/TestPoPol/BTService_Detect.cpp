// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include"EnemyAIController.h"
#include"TestHEROCharacter.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"DrawDebugHelpers.h"
#include"TestGameModeBase.h"




UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 20.0f;				//호출 주기 ,Service 노드 내부에 설정된 Interval속성값으로 지정할 수 있음



}


void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;


	if (nullptr == World)return;
	TArray<FOverlapResult>OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam

	);


	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			

			ATestHEROCharacter* ABCharater = Cast<ATestHEROCharacter>(OverlapResult.GetActor());
			if (ABCharater && ABCharater->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, ABCharater);						

				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//
				DrawDebugPoint(World, ABCharater->GetActorLocation(), 10.0f, FColor::Blue, false,0.2f);
				//
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharater->GetActorLocation(), FColor::Blue, false, 0.2f);

				return;




			}
		}
	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);


}
