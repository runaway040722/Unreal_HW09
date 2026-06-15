#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANBPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ANBPlayerState();

private:
    UPROPERTY(Replicated)
    int32 RemainingTries;

    UPROPERTY(Replicated)
    int32 MaxTries;

    UPROPERTY(Replicated)
    bool bHasSubmittedThisTurn;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void ConsumeTry() { if (RemainingTries > 0) RemainingTries--; }
    void ResetTries() { RemainingTries = MaxTries; }

    int32 GetRemainingTries() const { return RemainingTries; }
    int32 GetMaxTries() const { return MaxTries; }
    bool IsOutOfTries() const { return RemainingTries <= 0; }

    void SetHasSubmitted(bool bSubmitted) { bHasSubmittedThisTurn = bSubmitted; }
    bool HasSubmittedThisTurn() const { return bHasSubmittedThisTurn; }
};