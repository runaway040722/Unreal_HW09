#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANBGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANBGameModeBase();

protected:
    virtual void BeginPlay() override;

private:
    TArray<int32> SecretNumber;
    FTimerHandle TurnTimerHandle;
    int32 TurnDuration;

    void GenerateRandomNumbers();
    void ResetGame();

    void UpdateTurnTimer();
    void SwitchToNextTurn();

public:
    FString CheckAnswer(APlayerController* PlayerController, const FString& Input);
    void ProcessGameRules(APlayerController* TargetController, const FString& LastResult);
};