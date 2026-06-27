// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"


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

	// Находим камеру владельца один раз и запоминаем.
	if (AActor* OwnerActor = GetOwner())
	{
		CachedCamera = OwnerActor->GetComponentByClass<UCameraComponent>();
	}

	// Если камеры нет — предупреждаем в лог (трейс пойдёт от глаз актора как запасной вариант).
	if (!CachedCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent: камера не найдена у владельца %s"),
			GetOwner() ? *GetOwner()->GetName() : TEXT("nullptr"));
	}

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

	// 2. Узнаём точку и направление, откуда пускаем луч.
	FVector ViewLocation;
	FRotator ViewRotation;

	if (CachedCamera)
	{
		// Основной путь: целимся точно из камеры (центр экрана).
		ViewLocation = CachedCamera->GetComponentLocation();
		ViewRotation = CachedCamera->GetComponentRotation();
	}
	else
	{
		// Запасной путь: камеры нет — старое поведение от "глаз" актора.
		OwnerActor->GetActorEyesViewPoint(ViewLocation, ViewRotation);
	}

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

	// 5. Отладка: рисуем луч (зелёный — попал, красный — мимо). Только если включён дебаг.
	if (bShowDebug)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, -1.0f, 0, 1.0f);
	}

	// 6. Что под прицелом?
	AActor* HitActor = bHit ? HitResult.GetActor() : nullptr;

	if (HitActor && HitActor->Implements<UInteractable>())
	{
		CurrentInteractable = HitActor;

		if (bShowDebug)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Вижу interactable: %s"), *HitActor->GetName()));
		}
	}
	else
	{
		CurrentInteractable = nullptr;
	}
}