// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������ ��������� ������
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	// ����������� ������ � ������� ���������
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());

	// ��������� ������ �� ������ ���� (���� ��������, ������������ ������ �������� �����)
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

	// ��������� ������ �������������� ������ �� �������� ������ (����)
	FirstPersonCamera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �������� PlayerController, ����������� ���� ����������
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// �������� ���������� Enhanced Input
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// ��������� ��� ����� �������� (���� �� �������� � Blueprint)
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	// Устанавливаем стартовую скорость ходьбы из нашей переменной
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���������� ������� InputComponent � Enhanced-������
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ��������: ���� ������� ������ (Triggered) � �������� Move
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		// �����: ���� ��������� ���� � �������� Look
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}

		// ������: ����� (Started) � StartSprint, �������� (Completed) � StopSprint
		if (SprintAction)
		{
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
		}

		// ������: ����� � StartCrouch, �������� � StopCrouch
		if (CrouchAction)
		{
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouch);
		}
	}
}

// === �������� (WASD) ===
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// ������ ������ ����������� �� ����� (X = ����, Y = �����/�����)
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// ������/����� (W/S): ��� Y (IMC swizzle ��������� W/S �� Y)
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);

		// ������/����� (A/D): ��� X
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

// === ����� ����� ===
void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// ������� �����/������
		AddControllerYawInput(LookVector.X);

		// ������ �����/���� (��������� Negate �� Y � IMC_Player, ��� � �������� UE)
		AddControllerPitchInput(LookVector.Y);
	}
}

// === ������ ===
void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// === ���������� ===
void APlayerCharacter::StartCrouch()
{
	Crouch();
}

void APlayerCharacter::StopCrouch()
{
	UnCrouch();
}