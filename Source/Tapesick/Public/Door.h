// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Door.generated.h"

class UStaticMeshComponent;

UCLASS()
class TAPESICK_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void BeginPlay() override;

	// --- Компоненты ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<USceneComponent> DoorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	// --- Состояние (рантайм) ---

	UPROPERTY(BlueprintReadOnly, Category = "Door|State")
	bool bIsOpen = false;

	UPROPERTY(BlueprintReadOnly, Category = "Door|State")
	bool bIsLocked = false;

	// --- Настройки старта ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Setup")
	bool bStartLocked = false;

public:
	// --- Публичные команды ---

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	void SetLocked(bool bNewLocked);

	// --- Интерфейс взаимодействия ---

	virtual void Interact_Implementation(AActor* InteractInstigator) override;

	// --- Мосты в Blueprint ---

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void OnDoorOpened();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void OnDoorClosed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void OnLockedInteract();

protected:
	void ResolveInitialState();
};