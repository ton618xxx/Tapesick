// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 1. Берём владельца компонента (актор, на котором висит компонент — игрок)
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	// 2. Узнаём точку и направление "глаз" владельца
	FVector ViewLocation;
	FRotator ViewRotation;
	OwnerActor->GetActorEyesViewPoint(ViewLocation, ViewRotation);

	// 3. Считаем конец луча: от глаз вперёд на InteractionDistance
	const FVector TraceStart = ViewLocation;
	const FVector TraceEnd = TraceStart + (ViewRotation.Vector() * InteractionDistance);

	// 4. Пускаем луч, игнорируя самого владельца
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	// 5. Отладка: рисуем луч (зелёный — попал, красный — мимо)
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, -1.0f, 0, 1.0f);

	// 6. Что под прицелом?
	AActor* HitActor = bHit ? HitResult.GetActor() : nullptr;

	// 7. Проверяем, реализует ли объект интерфейс Interactable
	if (HitActor && HitActor->Implements<UInteractable>())
	{
		CurrentInteractable = HitActor;
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Вижу interactable: %s"), *HitActor->GetName()));
	}
	else
	{
		CurrentInteractable = nullptr;
	}
}

