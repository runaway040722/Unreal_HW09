#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBGameStateBase.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANBGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    ANBGameStateBase();

private:
    UPROPERTY(Replicated)
    int32 TimeRemaining;

    UPROPERTY(Replicated)
    APlayerController* CurrentTurnPlayer;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void SetTimeRemaining(int32 NewTime) { TimeRemaining = NewTime; }
    int32 GetTimeRemaining() const { return TimeRemaining; }

    void SetCurrentTurnPlayer(APlayerController* NewPlayer) { CurrentTurnPlayer = NewPlayer; }
    APlayerController* GetCurrentTurnPlayer() const { return CurrentTurnPlayer; }
};