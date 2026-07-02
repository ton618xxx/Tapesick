#include "Bed.h"
#include "Components/StaticMeshComponent.h"

ABed::ABed()
{
	PrimaryActorTick.bCanEverTick = false;

	BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMesh"));
	SetRootComponent(BedMesh);
}

void ABed::Interact_Implementation(AActor* InteractInstigator)
{
	OnBedInteract();
}