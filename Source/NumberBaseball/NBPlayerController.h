#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANBPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "NumberBaseball")
    void Server_SubmitGuess(const FString& InputGuess);

    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_DisplayResult(const FString& PlayerName, const FString& Guess, const FString& Result, int32 RemainingTries, int32 MaxTries);

    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_NoticeGameOver(const FString& NoticeMessage);

    UFUNCTION(BlueprintCallable, Category = "NumberBaseball")
    bool IsMyTurnAndHasTime() const;
};