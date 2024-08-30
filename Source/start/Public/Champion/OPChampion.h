// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OPChampion.generated.h"

class AOPDiavolo;

UCLASS()
class START_API AOPChampion : public ACharacter
{
    GENERATED_BODY()

public:
    AOPChampion();

    virtual void BeginPlay() override; // 게임이 시작될 때 실행될 함수
    virtual void Tick(float DeltaTime) override; // 매 프레임마다 실행되는 함수
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // 조작키
    
protected:
    FHitResult MouseCursorHit; // 마우스 커서에서 받아오는 정보를 담을 HitResult

    virtual void Passive();
    virtual void BasicAttack(); // 평타
    virtual void Q();
    virtual void W();
    virtual void E();
    virtual void R();

public:
    void TurnCharacterToLocation(FVector TurnPoint);
    void TurnCharacterToCursor(FHitResult HitResult);
    /********************************************************************** Gravity Field Value Setting**********************************************************************/
       // Custom functions to set gravity direction and scale  
       // Seqence  1.Set Direction 2. Set Scale 
    void SetCustomGravityDirection(FVector NewGravityDirection);
    void SetCustomGravityScale(float NewGravityScale);
    // Overlap end event handler
    UFUNCTION()
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
    // Overlap 이벤트를 처리할 함수 선언(중력)
    UFUNCTION()
    virtual void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
protected:
    /********************************************************************** Player Controller **********************************************************************/
    
    TObjectPtr<class AOPPlayerController> OPPlayerController;

    /************************************************************************ Player Sight ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class USpringArmComponent> SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UCameraComponent> Camera;

    /************************************************************************ Player Inputs ************************************************************************/

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
    TObjectPtr<UInputAction> R_Action;

    void Move(const struct FInputActionValue& Value);
    void Look(const struct FInputActionValue& Value);

    /******************************************************************** Champion AnimInstance ********************************************************************/

    TObjectPtr<class UOPAnimInstance> ChampionAnimInstance;

    /****************************************************************** Diavolo for Physical Test ******************************************************************/
    
    TObjectPtr<AOPDiavolo> TestDiavolo;

    /**************************************************************** Champion Combat Interactions ****************************************************************/

    bool bIsDamaged = false;
    bool bIsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damaged", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DamagedAnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damaged", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DeadAnimMontage;
    
    bool bTrueSightOn = false; // 리신 음파 (Q)에 피격되면 표식에 생겨서 true가 되는 boolean 값
    bool bStumbledByLeeSinE = false; // 리신 음파 (Q)에 피격되면 표식에 생겨서 true가 되는 boolean 값
    bool bFrenziedMaulOn = false; // 볼리베어 W 1타에 피격되어 표식이 생겨서 true가 되는 boolean 값
    int32 StormMarkCount = 0;

    /*************************************************************************** Passive ***************************************************************************/
    
    bool bPassive = true;

    FTimerHandle Passive_Cooldown_TimerHandle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive", meta = (AllowPrivateAccess = "true"))
    float Passive_Cooldown = 10.f;    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Passive_AnimMontage;
    
    /************************************************************************ Basic Attack ************************************************************************/
    
    bool bBasicAttack = true; // 평타를 사용할 수 있는가에 대한 불리언 값. 이 불리언 값이 true라면 좌클릭 시 평타가 나간다.
    
    FTimerHandle BasicAttack_Cooldown_TimerHandle; // 평타 사용 후 쿨타임이 지나면 다음 평타를 사용할 수 있도록 bBasicAttack을 true로 바꿔줄 때 사용하는 타이머핸들

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Cooldown = 1.f; // 평타의 쿨다운

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> BasicAttack_AnimMontage; // 평타 애니메이션
    
    /****************************************************************************** Q ******************************************************************************/
    
    bool bQ = true;
    
    FTimerHandle Q_Cooldown_TimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q", meta = (AllowPrivateAccess = "true"))
    float Q_Cooldown = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Q_AnimMontage;

    /****************************************************************************** W ******************************************************************************/
    
    bool bW = true;

    FTimerHandle W_Cooldown_TimerHandle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
    float W_Cooldown = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> W_AnimMontage;

    /****************************************************************************** E ******************************************************************************/
    
    bool bE = true;

    FTimerHandle E_Cooldown_TimerHandle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
    float E_Cooldown = 10.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> E_AnimMontage;

    /****************************************************************************** R ******************************************************************************/
    
    bool bR = true;

    FTimerHandle R_Cooldown_TimerHandle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R", meta = (AllowPrivateAccess = "true"))
    float R_Cooldown = 10.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> R_AnimMontage;

    /********************************************************************** Champion Movements **********************************************************************/
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Champion Movement", meta = (AllowPrivateAccess = "true"))
    float WalkSpeed = 350.f; // 챔피언의 이동 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Champion Movement", meta = (AllowPrivateAccess = "true"))
    float DefaultWalkSpeed = 350.f; // 챔피언의 기본 이동 속도
    
    void ResetChampionMovement() const; // 챔피언의 이동속도 및 다른 움직임들을 디폴트 값으로 리셋시킴
    void StopChampionMovement() const; // 챔피언의 움직임을 정지시킴 (스킬 쓸때 등)
    FTimerHandle ResetMovementTimerHandle; // 챔피언의 움직임들을 디폴트 값으로 리셋시킬 시간을 조절하기 위한 타이머 핸들

    /****************************************************************************** ETC ******************************************************************************/
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> GravityFieldSphereClass;  // BP_GravityField_Sphere 클래스를 저장할 변수

    // 현재 중력 필드를 저장할 변수
    UPROPERTY()
    AActor* CurrentGravityField;
public:
    /************************************************************************ Getter / Setter ************************************************************************/
    
    FORCEINLINE AOPPlayerController* GetOPPlayerController() const { return OPPlayerController; }
    FORCEINLINE FHitResult GetMouseCursorHit() { return MouseCursorHit; }

    FORCEINLINE bool GetbIsDamaged() const { return bIsDamaged; }
    FORCEINLINE void SetbIsDamagedTrue() { bIsDamaged = true; }
    FORCEINLINE bool GetbIsDead() const { return bIsDead; }
    FORCEINLINE void SetbIsDeadTrue() { bIsDead = true; }
    FORCEINLINE UAnimMontage* GetDamagedAnimMontage() const { return DamagedAnimMontage; }
    FORCEINLINE UAnimMontage* GetDeadAnimMontage() const { return DeadAnimMontage; }

    FORCEINLINE bool GetbBasicAttack() const { return bBasicAttack; }
    FORCEINLINE float GetBasicAttackCooltime() const { return BasicAttack_Cooldown; }
    FORCEINLINE void SetbBasicAttack(bool value) { bBasicAttack = value; }
    FORCEINLINE void SetbBasicAttack_True() { bBasicAttack = true; }
    FORCEINLINE void SetbBasicAttack_False() { bBasicAttack = false; }
    FORCEINLINE FTimerHandle GetBasicAttackTimerHandle() const { return BasicAttack_Cooldown_TimerHandle; }
    FORCEINLINE UAnimMontage* GetBasicAttackAnimMontage() const { return BasicAttack_AnimMontage; }
    
    FORCEINLINE bool GetbPassive() const { return bPassive; }
    FORCEINLINE float GetPassiveCooltime() const { return Passive_Cooldown; }
    FORCEINLINE void SetbPassive(bool value) { bPassive = value; }
    FORCEINLINE void SetbPassive_True() { bPassive = true; }
    FORCEINLINE void SetbPassive_False() { bPassive = false; }
    FORCEINLINE FTimerHandle GetPassiveTimerHandle() const { return Passive_Cooldown_TimerHandle; }
    FORCEINLINE UAnimMontage* GetPassiveAnimMontage() const { return Passive_AnimMontage; }

    FORCEINLINE bool GetbQ() const { return bQ; }
    FORCEINLINE float GetQ_Cooldown() const { return Q_Cooldown; }
    FORCEINLINE void SetbQ(bool value) { bQ = value; }
    FORCEINLINE void SetbQ_True() { bQ = true; }
    FORCEINLINE void SetbQ_False() { bQ = false; }
    FORCEINLINE FTimerHandle GetQ_TimerHandle() const { return Q_Cooldown_TimerHandle; }
    FORCEINLINE UAnimMontage* GetQ_AnimMontage() const { return Q_AnimMontage; }

    FORCEINLINE bool GetbW() const { return bW; }
    FORCEINLINE float GetW_Cooldown() const { return W_Cooldown; }
    FORCEINLINE void SetbW(bool value) { bW = value; }
    FORCEINLINE void SetbW_True() { bW = true; }
    FORCEINLINE void SetbW_False() { bW = false; }
    FORCEINLINE FTimerHandle GetW_TimerHandle() const { return W_Cooldown_TimerHandle; }
    FORCEINLINE UAnimMontage* GetW_AnimMontage() const { return W_AnimMontage; }

    FORCEINLINE bool GetbE() const { return bE; }
    FORCEINLINE float GetE_Cooldown() const { return E_Cooldown; }
    FORCEINLINE void SetbE(bool value) { bE = value; }
    FORCEINLINE void SetbE_True() { bE = true; }
    FORCEINLINE void SetbE_False() { bE = false; }
    FORCEINLINE FTimerHandle GetE_TimerHandle() const { return E_Cooldown_TimerHandle; }
    FORCEINLINE UAnimMontage* GetE_AnimMontage() const { return E_AnimMontage; }

    FORCEINLINE bool GetbR() const { return bR; }
    FORCEINLINE float GetR_Cooldown() const { return R_Cooldown; }
    FORCEINLINE void SetbR(bool value) { bR = value; }
    FORCEINLINE void SetbR_True() { bR = true; }
    FORCEINLINE void SetbR_False() { bR = false; }
    FORCEINLINE FTimerHandle GetR_TimerHandle() const { return R_Cooldown_TimerHandle; }
    FORCEINLINE UAnimMontage* GetR_AnimMontage() const { return R_AnimMontage; }

    FORCEINLINE UOPAnimInstance* GetChampionAnimInstance() const { return ChampionAnimInstance; }

    FORCEINLINE AOPDiavolo* GetTestDiavolo() const { return TestDiavolo; }
    FORCEINLINE void SetTestDiavolo(AOPDiavolo* Diavolo) { TestDiavolo = Diavolo; }

    FORCEINLINE bool GetbTrueSightOn() const { return bTrueSightOn; }
    FORCEINLINE void SetbTrueSightOn_True() { bTrueSightOn = true; }
    FORCEINLINE void SetbTrueSightOn_False() { bTrueSightOn = false; }

    FORCEINLINE bool GetbStumbledByLeeSinE() const { return bStumbledByLeeSinE; }
    FORCEINLINE void SetbStumbledByLeeSinE_True() { bStumbledByLeeSinE = true; }
    FORCEINLINE void SetbStumbledByLeeSinE_False() { bStumbledByLeeSinE = false; }
    
    FORCEINLINE bool GetbFrenziedMaulOn() const { return bFrenziedMaulOn; }
    FORCEINLINE void SetbFrenziedMaulOn_True() { bFrenziedMaulOn = true; }
    FORCEINLINE void SetbFrenziedMaulOn_False() { bFrenziedMaulOn = false; }

    FORCEINLINE int32 GetStormMarkCount() const { return StormMarkCount; }
    FORCEINLINE void IncreaseStormMark() { StormMarkCount++; }
    FORCEINLINE void ResetStormMark() { StormMarkCount = 0; }

    UFUNCTION(BlueprintCallable)
    USkeletalMeshComponent* GetChampionSkeletalMeshComponent() const { return GetMesh(); }



    void PlayDeadAnimMontage() const;
};
