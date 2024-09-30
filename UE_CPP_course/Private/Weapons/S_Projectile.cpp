// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/S_Projectile.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/S_FXWeaponComponent.h"

// Sets default values
AS_Projectile::AS_Projectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision component");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    SetRootComponent(CollisionComponent);

    ProjectileMovementComponent =
        CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movevment component");
    ProjectileMovementComponent->InitialSpeed = 2000.f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.05f;

    FXComponent = CreateDefaultSubobject<US_FXWeaponComponent>("Weapon FX");
    
}

void AS_Projectile::BeginPlay()
{   
    Super::BeginPlay();

    check(FXComponent);
    //Owner = Cast<ACharacter>(GetOwner());

    ProjectileMovementComponent->Velocity = ProjectileDirection * ProjectileMovementComponent->InitialSpeed;

    CollisionComponent->OnComponentHit.AddDynamic(this, &AS_Projectile::OnProjectileHit);
}

void AS_Projectile::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                                    UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    if (!GetWorld())
        return;

    ProjectileMovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage( // s
        GetWorld(),                      //
        DamageAmount,                    //
        GetActorLocation(),              //
        DamageRadius,                    //
        UDamageType::StaticClass(),      //
        {},                              // GetOwner() if don't want to damage yourself
        this,                            //
        GetController(),                 // for future purpose to check who's dealt the damage
        false);                          //

    //DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Yellow, false, 2.0f);
    FXComponent->SpawnEmitter(Hit);


    Destroy();
}

AController *AS_Projectile::GetController()
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
