#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Bed.generated.h"

class UStaticMeshComponent;

UCLASS()
class TAPESICK_API ABed : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ABed();

	virtual void Interact_Implementation(AActor* InteractInstigator) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Bed")
	void OnBedInteract();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bed")
	TObjectPtr<UStaticMeshComponent> BedMesh;
};