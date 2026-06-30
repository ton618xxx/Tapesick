// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Components/StaticMeshComponent.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Корень — неподвижная опора двери.
	DoorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRoot"));
	SetRootComponent(DoorRoot);

	// Створка — крепится к корню, её будет вращать Timeline в Blueprint.
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorRoot);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	ResolveInitialState();
}

void ADoor::ResolveInitialState()
{
	// СЕЙЧАС: стартовое состояние берём из настройки дизайнера.
	// ПОТОМ: здесь спросим UStorySubsystem про этап истории.
	bIsLocked = bStartLocked;
	bIsOpen = false;
}

void ADoor::OpenDoor()
{
	if (bIsOpen)
	{
		return;
	}

	bIsOpen = true;
	OnDoorOpened();
}

void ADoor::CloseDoor()
{
	if (!bIsOpen)
	{
		return;
	}

	bIsOpen = false;
	OnDoorClosed();
}

void ADoor::SetLocked(bool bNewLocked)
{
	bIsLocked = bNewLocked;
}

void ADoor::Interact_Implementation(AActor* InteractInstigator)
{
	if (bIsLocked)
	{
		OnLockedInteract();
		return;
	}

	if (bIsOpen)
	{
		CloseDoor();
	}
	else
	{
		OpenDoor();
	}
}