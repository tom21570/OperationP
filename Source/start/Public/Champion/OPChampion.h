// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "OPChampion.generated.h"

class AOPDiavolo;

UCLASS()
class START_API AOPChampion : public ACharacter
{
    GENERATED_BODY()

public:
    AOPChampion();

protected:
    FHitResult MouseCursorHit;
    virtual void BeginPlay() override;

    virtual void Passive();
    virtual void MeleeAttack();
    virtual void LongDistanceAttack();
    virtual void Skill_1();
    virtual void Skill_2();
    virtual void Skill_3();
    virtual void Skill_4();
    virtual void Ult();

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void TurnCharacterToLocation(FVector TurnPoint);
    void TurnCharacterToCursor(FHitResult HitResult);

protected:
    TObjectPtr<class AOPPlayerController> OPPlayerController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class USpringArmComponent> SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UCameraComponent> Camera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UInputMappingContext> ChampionMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UInputAction> JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MeleeAttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SkillAction1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SkillAction2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SkillAction3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SkillAction4;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> UltAction;

    void Move(const struct FInputActionValue& Value);
    void Look(const struct FInputActionValue& Value);

    TObjectPtr<class UOPAnimInstance> ChampionAnimInstance;

    TObjectPtr<AOPDiavolo> TestDiavolo;

    bool bIsDamaged = false;
    bool bIsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damaged", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DamagedAnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damaged", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DeadAnimMontage;

    FTimerHandle MeleeAttackCooltimeTimer;
    bool bMeleeAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeAttack", meta = (AllowPrivateAccess = "true"))
    float MeleeAttackCooltime = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> MeleeAttackAnimMontage;

    FTimerHandle LongDistanceAttackCooltimeTimer;
    bool bLongDistanceAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LongDistanceAttack", meta = (AllowPrivateAccess = "true"))
    float LongDistanceAttackCooltime = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Long DistanceAttack", meta = (AllowPrivateAccess = "true"))
    class UAnimMontage* LongDistanceAttackAnimMontage;


    FTimerHandle PassiveTimer;
    bool bPassive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive", meta = (AllowPrivateAccess = "true"))
    float Passive_Cooltime = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Passive_AnimMontage;

    FTimerHandle Skill_1_CooltimeTimer;
    bool bSkill_1 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    float Skill_1_Cooltime = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_1_AnimMontage;

    bool bSkill_2 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2", meta = (AllowPrivateAccess = "true"))
    float Skill_2_Cooltime = 10.f;

    FTimerHandle Skill_2_CooltimeTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_2_AnimMontage;

    bool bSkill_3 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3", meta = (AllowPrivateAccess = "true"))
    float Skill_3_Cooltime = 10.f;

    FTimerHandle Skill_3_CooltimeTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_3_AnimMontage;

    bool bSkill_4 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4", meta = (AllowPrivateAccess = "true"))
    float Skill_4_Cooltime = 10.f;

    FTimerHandle Skill_4_CooltimeTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_4_AnimMontage;

    bool bUlt = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
    float Ult_Cooltime = 10.f;

    FTimerHandle Ult_CooltimeTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Ult_AnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNiagaraSystem> ShieldEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNiagaraComponent> ShieldEffectComponent;

    FTimerHandle FuzeTimerHandle;

public:
    void DeactivateShieldEffect();

    FORCEINLINE AOPPlayerController* GetOPPlayerController() const { return OPPlayerController; }
    FORCEINLINE FHitResult GetMouseCursorHit() { return MouseCursorHit; }

    FORCEINLINE bool GetbIsDamaged() const { return bIsDamaged; }
    FORCEINLINE void SetbIsDamagedTrue() { bIsDamaged = true; }
    FORCEINLINE bool GetbIsDead() const { return bIsDead; }
    FORCEINLINE void SetbIsDeadTrue() { bIsDead = true; }
    FORCEINLINE UAnimMontage* GetDamagedAnimMontage() const { return DamagedAnimMontage; }
    FORCEINLINE UAnimMontage* GetDeadAnimMontage() const { return DeadAnimMontage; }

    FORCEINLINE bool GetbMeleeAttack() const { return bMeleeAttack; }
    FORCEINLINE float GetMeleeAttackCooltime() const { return MeleeAttackCooltime; }
    FORCEINLINE void SetbMeleeAttack(bool value) { bMeleeAttack = value; }
    FORCEINLINE void SetbMeleeAttack_True() { bMeleeAttack = true; }
    FORCEINLINE void SetbMeleeAttack_False() { bMeleeAttack = false; }
    FORCEINLINE FTimerHandle GetMeleeAttackTimerHandle() { return MeleeAttackCooltimeTimer; }
    FORCEINLINE UAnimMontage* GetMeleeAttackAnimMontage() const { return MeleeAttackAnimMontage; }

    FORCEINLINE bool GetbLongDistanceAttack() const { return bLongDistanceAttack; }
    FORCEINLINE float GetLongDistanceAttackCooltime() const { return LongDistanceAttackCooltime; }
    FORCEINLINE void SetbLongDistanceAttack(bool value) { bLongDistanceAttack = value; }
    FORCEINLINE void SetbLongDistanceAttack_True() { bLongDistanceAttack = true; }
    FORCEINLINE void SetbLongDistanceAttack_False() { bLongDistanceAttack = false; }
    FORCEINLINE FTimerHandle GetLongDistanceAttackTimerHandle() { return LongDistanceAttackCooltimeTimer; }
    FORCEINLINE UAnimMontage* GetLongDistanceAttackAnimMontage() const { return LongDistanceAttackAnimMontage; }


    FORCEINLINE bool GetbPassive() const { return bPassive; }
    FORCEINLINE float GetPassiveCooltime() const { return Passive_Cooltime; }
    FORCEINLINE void SetbPassive(bool value) { bPassive = value; }
    FORCEINLINE void SetbPassive_True() { bPassive = true; }
    FORCEINLINE void SetbPassive_False() { bPassive = false; }
    FORCEINLINE FTimerHandle GetPassiveTimerHandle() const { return PassiveTimer; }
    FORCEINLINE UAnimMontage* GetPassiveAnimMontage() const { return Passive_AnimMontage; }

    FORCEINLINE bool GetbSkill_1() const { return bSkill_1; }
    FORCEINLINE float GetSkill_1_Cooltime() const { return Skill_1_Cooltime; }
    FORCEINLINE void SetbSkill_1(bool value) { bSkill_1 = value; }
    FORCEINLINE void SetbSkill_1_True() { bSkill_1 = true; }
    FORCEINLINE void SetbSkill_1_False() { bSkill_1 = false; }
    FORCEINLINE FTimerHandle GetSkill_1_TimerHandle() const { return Skill_1_CooltimeTimer; }
    FORCEINLINE UAnimMontage* GetSkill_1_AnimMontage() const { return Skill_1_AnimMontage; }

    FORCEINLINE bool GetbSkill_2() const { return bSkill_2; }
    FORCEINLINE float GetSkill_2_Cooltime() const { return Skill_2_Cooltime; }
    FORCEINLINE void SetbSkill_2(bool value) { bSkill_2 = value; }
    FORCEINLINE void SetbSkill_2_True() { bSkill_2 = true; }
    FORCEINLINE void SetbSkill_2_False() { bSkill_2 = false; }
    FORCEINLINE FTimerHandle GetSkill_2_TimerHandle() const { return Skill_2_CooltimeTimer; }
    FORCEINLINE UAnimMontage* GetSkill_2_AnimMontage() const { return Skill_2_AnimMontage; }

    FORCEINLINE bool GetbSkill_3() const { return bSkill_3; }
    FORCEINLINE float GetSkill_3_Cooltime() const { return Skill_3_Cooltime; }
    FORCEINLINE void SetbSkill_3(bool value) { bSkill_3 = value; }
    FORCEINLINE void SetbSkill_3_True() { bSkill_3 = true; }
    FORCEINLINE void SetbSkill_3_False() { bSkill_3 = false; }
    FORCEINLINE FTimerHandle GetSkill_3_TimerHandle() const { return Skill_3_CooltimeTimer; }
    FORCEINLINE UAnimMontage* GetSkill_3_AnimMontage() const { return Skill_3_AnimMontage; }

    FORCEINLINE bool GetbSkill_4() const { return bSkill_4; }
    FORCEINLINE float GetSkill_4_Cooltime() const { return Skill_4_Cooltime; }
    FORCEINLINE void SetbSkill_4(bool value) { bSkill_4 = value; }
    FORCEINLINE void SetbSkill_4_True() { bSkill_4 = true; }
    FORCEINLINE void SetbSkill_4_False() { bSkill_4 = false; }
    FORCEINLINE FTimerHandle GetSkill_4_TimerHandle() const { return Skill_4_CooltimeTimer; }
    FORCEINLINE UAnimMontage* GetSkill_4_AnimMontage() const { return Skill_4_AnimMontage; }

    FORCEINLINE bool GetbUlt() const { return bUlt; }
    FORCEINLINE float GetUlt_Cooltime() const { return Ult_Cooltime; }
    FORCEINLINE void SetbUlt(bool value) { bUlt = value; }
    FORCEINLINE void SetbUlt_True() { bUlt = true; }
    FORCEINLINE void SetbUlt_False() { bUlt = false; }
    FORCEINLINE FTimerHandle GetUlt_TimerHandle() const { return Ult_CooltimeTimer; }
    FORCEINLINE UAnimMontage* GetUlt_AnimMontage() const { return Ult_AnimMontage; }

    FORCEINLINE UOPAnimInstance* GetChampionAnimInstance() const { return ChampionAnimInstance; }

    UFUNCTION(BlueprintCallable)
    USkeletalMeshComponent* GetChampionSkeletalMeshComponent() const { return GetMesh(); }

    virtual void AddShield(float ShieldAmount);
};
