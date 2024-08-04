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
    FHitResult MouseCursorHit; // 마우스 커서에서 받아오는 정보를 담을 HitResult
    virtual void BeginPlay() override; // 게임이 시작될 때 실행될 함수

    virtual void Passive();
    virtual void BasicAttack(); // 평타
    virtual void Q();
    virtual void W();
    virtual void E();
    virtual void R();

public:
    virtual void Tick(float DeltaTime) override; // 매 프레임마다 실행되는 함수
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // 조작키

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
    TObjectPtr<UInputAction> BasicAttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> Q_Action;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> W_Action;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> E_Action;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SkillAction4;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> R_Action;

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

    /**************************************** 평타 *******************************************/
    FTimerHandle BasicAttackCooltimeTimerHandle;
    FTimerHandle BasicAttackTimer;
    bool bBasicAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
    float BasicAttackCooltime = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> BasicAttackAnimMontage;

    FTimerHandle BasicAttack_MovementStopTimerHandle;
    
    /**************************************** 패시브 *******************************************/
    FTimerHandle PassiveTimerHandle;
    bool bPassive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive", meta = (AllowPrivateAccess = "true"))
    float Passive_Cooltime = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Passive_AnimMontage;

    /**************************************** 스킬 1 *******************************************/
    FTimerHandle Skill_1_CooltimeTimerHandle;
    bool bSkill_1 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    float Skill_1_Cooltime = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_1_AnimMontage;

    FTimerHandle Skill_1_MovementStopTimerHandle;

    /**************************************** 스킬 2 *******************************************/
    bool bSkill_2 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2", meta = (AllowPrivateAccess = "true"))
    float Skill_2_Cooltime = 10.f;

    FTimerHandle Skill_2_CooltimeTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_2_AnimMontage;

    FTimerHandle Skill_2_MovementStopTimerHandle;

    /**************************************** 스킬 3 *******************************************/
    bool bSkill_3 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3", meta = (AllowPrivateAccess = "true"))
    float Skill_3_Cooltime = 10.f;

    FTimerHandle Skill_3_CooltimeTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_3_AnimMontage;

    FTimerHandle Skill_3_MovementStopTimerHandle;

    /**************************************** 스킬 4 *******************************************/
    bool bSkill_4 = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4", meta = (AllowPrivateAccess = "true"))
    float Skill_4_Cooltime = 10.f;

    FTimerHandle Skill_4_CooltimeTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Skill_4_AnimMontage;

    FTimerHandle Skill_4_MovementStopTimerHandle;

    /**************************************** 궁극기 *******************************************/
    bool bUlt = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
    float Ult_Cooltime = 10.f;

    FTimerHandle Ult_CooltimeTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Ult_AnimMontage;

    FTimerHandle Ult_MovementStopTimerHandle;

    /**************************************** 기타 *******************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Champion Movement", meta = (AllowPrivateAccess = "true"))
    float DefaultWalkSpeed = 350.f; // 챔피언의 기본 이동 속도
    
    void ResetChampionMovement() const;
    void StopChampionMovement() const;
    FTimerHandle ResetMovementTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNiagaraSystem> ShieldEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNiagaraComponent> ShieldEffectComponent;

    FTimerHandle FuzeTimerHandle;

    //LeeSin
    bool bTrueSightOn = false;
    //Volibear
    bool bFrenziedMaulOn = false;

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

    FORCEINLINE bool GetbBasicAttack() const { return bBasicAttack; }
    FORCEINLINE float GetBasicAttackCooltime() const { return BasicAttackCooltime; }
    FORCEINLINE void SetbBasicAttack(bool value) { bBasicAttack = value; }
    FORCEINLINE void SetbBasicAttack_True() { bBasicAttack = true; }
    FORCEINLINE void SetbBasicAttack_False() { bBasicAttack = false; }
    FORCEINLINE FTimerHandle GetBasicAttackTimerHandle() const { return BasicAttackCooltimeTimerHandle; }
    FORCEINLINE UAnimMontage* GetBasicAttackAnimMontage() const { return BasicAttackAnimMontage; }
    
    FORCEINLINE bool GetbPassive() const { return bPassive; }
    FORCEINLINE float GetPassiveCooltime() const { return Passive_Cooltime; }
    FORCEINLINE void SetbPassive(bool value) { bPassive = value; }
    FORCEINLINE void SetbPassive_True() { bPassive = true; }
    FORCEINLINE void SetbPassive_False() { bPassive = false; }
    FORCEINLINE FTimerHandle GetPassiveTimerHandle() const { return PassiveTimerHandle; }
    FORCEINLINE UAnimMontage* GetPassiveAnimMontage() const { return Passive_AnimMontage; }

    FORCEINLINE bool GetbSkill_1() const { return bSkill_1; }
    FORCEINLINE float GetSkill_1_Cooltime() const { return Skill_1_Cooltime; }
    FORCEINLINE void SetbSkill_1(bool value) { bSkill_1 = value; }
    FORCEINLINE void SetbSkill_1_True() { bSkill_1 = true; }
    FORCEINLINE void SetbSkill_1_False() { bSkill_1 = false; }
    FORCEINLINE FTimerHandle GetSkill_1_TimerHandle() const { return Skill_1_CooltimeTimerHandle; }
    FORCEINLINE UAnimMontage* GetSkill_1_AnimMontage() const { return Skill_1_AnimMontage; }

    FORCEINLINE bool GetbSkill_2() const { return bSkill_2; }
    FORCEINLINE float GetSkill_2_Cooltime() const { return Skill_2_Cooltime; }
    FORCEINLINE void SetbSkill_2(bool value) { bSkill_2 = value; }
    FORCEINLINE void SetbSkill_2_True() { bSkill_2 = true; }
    FORCEINLINE void SetbSkill_2_False() { bSkill_2 = false; }
    FORCEINLINE FTimerHandle GetSkill_2_TimerHandle() const { return Skill_2_CooltimeTimerHandle; }
    FORCEINLINE UAnimMontage* GetSkill_2_AnimMontage() const { return Skill_2_AnimMontage; }

    FORCEINLINE bool GetbSkill_3() const { return bSkill_3; }
    FORCEINLINE float GetSkill_3_Cooltime() const { return Skill_3_Cooltime; }
    FORCEINLINE void SetbSkill_3(bool value) { bSkill_3 = value; }
    FORCEINLINE void SetbSkill_3_True() { bSkill_3 = true; }
    FORCEINLINE void SetbSkill_3_False() { bSkill_3 = false; }
    FORCEINLINE FTimerHandle GetSkill_3_TimerHandle() const { return Skill_3_CooltimeTimerHandle; }
    FORCEINLINE UAnimMontage* GetSkill_3_AnimMontage() const { return Skill_3_AnimMontage; }

    FORCEINLINE bool GetbSkill_4() const { return bSkill_4; }
    FORCEINLINE float GetSkill_4_Cooltime() const { return Skill_4_Cooltime; }
    FORCEINLINE void SetbSkill_4(bool value) { bSkill_4 = value; }
    FORCEINLINE void SetbSkill_4_True() { bSkill_4 = true; }
    FORCEINLINE void SetbSkill_4_False() { bSkill_4 = false; }
    FORCEINLINE FTimerHandle GetSkill_4_TimerHandle() const { return Skill_4_CooltimeTimerHandle; }
    FORCEINLINE UAnimMontage* GetSkill_4_AnimMontage() const { return Skill_4_AnimMontage; }

    FORCEINLINE bool GetbUlt() const { return bUlt; }
    FORCEINLINE float GetUlt_Cooltime() const { return Ult_Cooltime; }
    FORCEINLINE void SetbUlt(bool value) { bUlt = value; }
    FORCEINLINE void SetbUlt_True() { bUlt = true; }
    FORCEINLINE void SetbUlt_False() { bUlt = false; }
    FORCEINLINE FTimerHandle GetUlt_TimerHandle() const { return Ult_CooltimeTimerHandle; }
    FORCEINLINE UAnimMontage* GetUlt_AnimMontage() const { return Ult_AnimMontage; }

    FORCEINLINE UOPAnimInstance* GetChampionAnimInstance() const { return ChampionAnimInstance; }

    FORCEINLINE AOPDiavolo* GetTestDiavolo() const { return TestDiavolo; }
    FORCEINLINE void SetTestDiavolo(AOPDiavolo* Diavolo) { TestDiavolo = Diavolo; }

    FORCEINLINE bool GetbTrueSightOn() const { return bTrueSightOn; }
    FORCEINLINE void SetbTrueSightOn_True() { bTrueSightOn = true; }
    FORCEINLINE void SetbTrueSightOn_False() { bTrueSightOn = false; }
    FORCEINLINE bool GetbFrenziedMaulOn() const { return bFrenziedMaulOn; }
    FORCEINLINE void SetbFrenziedMaulOn_True() { bFrenziedMaulOn = true; }
    FORCEINLINE void SetbFrenziedMaulOn_False() { bFrenziedMaulOn = false; }

    UFUNCTION(BlueprintCallable)
    USkeletalMeshComponent* GetChampionSkeletalMeshComponent() const { return GetMesh(); }

    virtual void AddShield(float ShieldAmount);

    void PlayDeadAnimMontage() const;
};
