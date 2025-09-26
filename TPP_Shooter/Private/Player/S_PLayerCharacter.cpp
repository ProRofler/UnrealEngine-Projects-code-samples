// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/S_PLayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/S_CharacterMovementComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "S_Health_Component.h"
#include "Weapons/S_PlayerWeaponComponent.h"
#include "Weapons/S_BaseWeapon.h"

// Sets default values
AS_PLayerCharacter::AS_PLayerCharacter(const FObjectInitializer &ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<US_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    PlayerSpringarm = CreateDefaultSubobject<USpringArmComponent>("Spring arm");
    PlayerSpringarm->SetupAttachment(GetRootComponent());
    PlayerSpringarm->bUsePawnControlRotation = true;

    PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player camera");
    PlayerCamera->SetupAttachment(PlayerSpringarm);

    PlayerHealthComponent = CreateDefaultSubobject<US_Health_Component>("Player health");
    PlayerWeaponComponent = CreateDefaultSubobject<US_PlayerWeaponComponent>("Player weapon");

    HealthBarComponent = CreateDefaultSubobject<UTextRenderComponent>("Health bar");
    HealthBarComponent->SetupAttachment(GetRootComponent());
    HealthBarComponent->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AS_PLayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    // check components for nulls
    check(PlayerHealthComponent);
    check(HealthBarComponent);
    check(GetMesh());

    OnHealthChanged(PlayerHealthComponent->GetHealth(), 1.0f);

    LandedDelegate.AddDynamic(this, &AS_PLayerCharacter::OnGroundLanded);
    PlayerHealthComponent->OnDeath.AddUObject(this, &AS_PLayerCharacter::OnDeath);
    PlayerHealthComponent->OnHealthChange.AddUObject(this, &AS_PLayerCharacter::OnHealthChanged);
}

// Called every frame
void AS_PLayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // GetPlayerMovementAngle();

    // PlayerHealthComponent->AutoHeal(0.05f, 1.0f);
    // TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}

// Called to bind functionality to input
void AS_PLayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    ControllerSetup();

    if (UEnhancedInputComponent *Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(MovingAction, ETriggerEvent::Triggered, this, &AS_PLayerCharacter::MoveAction);
        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AS_PLayerCharacter::LookingAction);
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AS_PLayerCharacter::Jump);
        Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AS_PLayerCharacter::WantsToRun);
        Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AS_PLayerCharacter::WantsToRun);
        Input->BindAction(FireAction, ETriggerEvent::Started, PlayerWeaponComponent, &US_PlayerWeaponComponent::Fire);
        Input->BindAction(FireAction, ETriggerEvent::Completed, PlayerWeaponComponent, &US_PlayerWeaponComponent::StopFire);
        Input->BindAction(WeaponSwitchAction, ETriggerEvent::Triggered, PlayerWeaponComponent, &US_PlayerWeaponComponent::SwitchWeapon);
        Input->BindAction(ReloadAction, ETriggerEvent::Triggered, PlayerWeaponComponent, &US_PlayerWeaponComponent::Reload);
    }
}

void AS_PLayerCharacter::ControllerSetup()
{
    if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Got controller!"));
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }
}

void AS_PLayerCharacter::MoveAction(const FInputActionValue &Value)
{
    const FVector2D MovingAxis = Value.Get<FVector2D>();
    // UE_LOG(LogTemp, Warning, TEXT("IA_input triggered"));

    AddMovementInput(GetActorForwardVector(), MovingAxis.X);
    AddMovementInput(GetActorRightVector(), MovingAxis.Y);

    /*
    FVector NewLocation = GetActorLocation();
    NewLocation.X += MovingAxis.X * MovementSpeed;
    NewLocation.Y += MovingAxis.Y * MovementSpeed;
    SetActorLocation(NewLocation);*/
}

void AS_PLayerCharacter::WantsToRun(const FInputActionValue &Value)
{
    if (!SprintAction)
        return;
    bWantsToRun = Value.Get<bool>();
    // UE_LOG(LogTemp, Display, TEXT("Wants to RUN is %s"),
    //        bWantsToRun ? TEXT("true") : TEXT("false"));
}

void AS_PLayerCharacter::LookingAction(const FInputActionValue &Value)
{
    const FVector2D LookingAxis = Value.Get<FVector2D>();

    AddControllerYawInput(LookingAxis.X);
    if (IsInverted)
    {
        AddControllerPitchInput(LookingAxis.Y);
    }
    else
    {
        AddControllerPitchInput(LookingAxis.Y * -1);
    }
}

void AS_PLayerCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto LandVelocity = -GetVelocity().Z;
    //UE_LOG(LogCharacterDamage, Display, TEXT("Land velocity: %f"), LandVelocity);

    if (LandVelocity < LandingSpeedRange.X)
        return;
    const auto LandDamage = FMath::GetMappedRangeValueClamped(LandingSpeedRange, LandingDamageRange, LandVelocity);
    TakeDamage(LandDamage, FDamageEvent{}, nullptr, nullptr);
    UE_LOG(LogCharacterDamage, Display, TEXT("Taken %f of land damage"), LandDamage);
}

float AS_PLayerCharacter::GetPlayerMovementAngle() const
{
    //GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Velocity: %s"), *GetVelocity().ToString()), true);
    if (GetVelocity().IsZero())
        return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto VectorsAngle = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degree = FMath::RadiansToDegrees(VectorsAngle);
    //GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Cross product: %f Degree %f"), CrossProduct.Z, Degree), true);

    return CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

void AS_PLayerCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->Deactivate();
    //PlayAnimMontage(DeathAnimation); //instant death no animation
    //HealthBarComponent->DestroyComponent();
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    PlayerWeaponComponent->DetachMainWeapon();
    SetLifeSpan(3.0f);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void AS_PLayerCharacter::OnHealthChanged(float health, float HealthDelta)
{
    // const auto health = PlayerHealthComponent->GetHealth();
    HealthBarComponent->SetText(FText::FromString(FString::Printf(TEXT("Health : % .0f"), health)));
}

