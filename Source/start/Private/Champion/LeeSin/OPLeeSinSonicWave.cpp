// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#include "Champion/LeeSin/OPLeeSinSonicWave.h"
#include "Champion/OPChampion.h"
#include "Animation/AnimInstance.h" // �ִϸ��̼� �ν��Ͻ� ��� ����
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Champion/LeeSin/OPLeeSin.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPLeeSinSonicWave::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
    {
        AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor);
        AOPLeeSin* LeeSinOwner = Cast<AOPLeeSin>(GetOwner());
        if (Diavolo)
        {
            LeeSinOwner->SetResonateTarget(Diavolo);
            LeeSinOwner->SetTestDiavolo(Diavolo);

            LeeSinOwner->SetbCanResonate_True();
            Diavolo->SetbTrueSightOn_True();

            Diavolo->GetChampionAnimInstance()->Montage_Play(Diavolo->GetDiavolo_DamagedByLeeSinSonicWave_AnimMontage());

            if (!Diavolo->GetbCanBeTestedMultipleTimes())
            {
                Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
            }
            if (LeeSinOwner)
            {
                LeeSinOwner->GetWorldTimerManager().SetTimer(LeeSinOwner->Skill_1_StackTimer, [LeeSinOwner, Diavolo]()
                {
                    LeeSinOwner->SetbSkill_1_True();
                    LeeSinOwner->RemoveMarkerOnTarget(Diavolo);
                }, LeeSinOwner->Skill_1_StackTime, false); // ���� ����

                LeeSinOwner->CreateMarkerOnTarget(Diavolo);
            }
        }
    }
    Destroy();
}
