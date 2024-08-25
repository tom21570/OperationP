// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPYasuo.generated.h"

class AOPYasuoWhirlWind;
class AOPYasuoWindWall;

/**
 * 
 */
UCLASS()
class START_API AOPYasuo : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPYasuo();

	virtual void BeginPlay() override;
	
protected:
	/*************************************************************************** Passive ***************************************************************************/
	
	virtual void Passive() override;

	/************************************************************************ Basic Attack ************************************************************************/
	
	virtual void BasicAttack() override;
	bool BasicAttackTrace(); // 평타 발동 시 트레이스하는 함수

	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	void Q_PlayOrdinaryAnimMontage();
	bool Q_Trace(); // 강철폭풍 발동 시 트레이스하는 함수
	void Q_WhirlWind(); // 강철폭풍 3타에서 회오리 날리기

	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	void W_WindWall();

	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;

	UFUNCTION()
	void E_OnDrawingSword(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	
private:
	/********************************************************************* Projectile Movement *********************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent; // 야스오 E 구현을 위해 야스오에 발사체 움직임을 추가해줌

	/************************************************************************ Basic Attack ************************************************************************/
	
	int32 BasicAttackComboCount = 0; // 평타 연속 사용 시 4번의 연결된 동작을 위한 콤보카운트

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 450.f; // 평타 사거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width = 80.f; // 평타 너비
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength = 0.f; // 평타 충격량

	FTimerHandle BasicAttack_Cast_TimerHandle; // 평타 시전을 위한 타이머

	FTimerHandle BasicAttack_ComboCount_TimerHandle; // 평타 연결된 동작 타이머

	FORCEINLINE void ResetBasicAttackComboCount() { BasicAttackComboCount = 0; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> BasicAttack_Hit_SFX;

	/****************************************************************************** Q ******************************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_Strength = 0.f; // 스킬 1 충격량

	int32 Q_Stack = 0; // 강철폭풍 스택
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_Range = 450.f; // 평타 사거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_Width = 80.f; // 평타 너비
	
	FTimerHandle Q_Cast_TimerHandle; // 강철폭풍 트레이스 시전시간을 위한 타이머
	FTimerHandle Q_Stack_TimerHandle; // 강철폭풍 스택 유지를 위한 타이머

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	bool bQ_CanSwift = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Q_Swift_AnimMontage;

	FTimerHandle Q_SwiftStart_TimerHandle;
	FTimerHandle Q_SwiftEnd_TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Q_WhirlWindSpawnPoint;

	TObjectPtr<AOPYasuoWhirlWind> Q_WhirlWindStorage; // 강철폭풍 3타 회오리를 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPYasuoWhirlWind> Q_WhirlWindClass; // 위 변수에 담아서 실제로 날릴 회오리

	FTimerHandle Q_WhirlWindSpawn_TimerHandle; // 회오리를 소환하는데 걸리는 시간을 설정하기 위한 타이머

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> Q_Hit_SFX; // 디아볼로가 강철폭풍 에 피격시 재생할 사운드 이펙트 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> Q_Charged_SFX;
	
	/****************************************************************************** W ******************************************************************************/

	TObjectPtr<AOPYasuoWindWall> W_WindWallStorage; // 바랑장막을 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPYasuoWindWall> W_WindWallClass; // 바람장막

	/****************************************************************************** E ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Strength = 0.f; // 발도술 중 적과 충돌했을 시의 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_DistanceBetweenEnemy = 0.f; // 발도술 시 적과 야스오 사이의 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed = 0.f; // 발도술 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_CastTime = 0.f; // 발도술을 시전하는 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Angle = 0.f; // 발도술을 시전하는 시간

	FTimerHandle E_End_TimerHandle;

	/****************************************************************************** R ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Strength = 0.f; // 궁극기 시전 후 디아볼로가 땅에 박히는 강도
	
	FTimerHandle R_End_TimerHandle;
};
