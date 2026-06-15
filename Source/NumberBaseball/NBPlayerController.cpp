#include "NBPlayerController.h"
#include "NBGameModeBase.h"
#include "NBPlayerState.h"
#include "NBGameStateBase.h"

bool ANBPlayerController::Server_SubmitGuess_Validate(const FString& InputGuess)
{
    return true;
}

void ANBPlayerController::Server_SubmitGuess_Implementation(const FString& InputGuess)
{
    if (!HasAuthority()) return;

    ANBGameModeBase* GameMode = Cast<ANBGameModeBase>(GetWorld()->GetAuthGameMode());
    ANBGameStateBase* GState = Cast<ANBGameStateBase>(GetWorld()->GetGameState());
    ANBPlayerState* PState = Cast<ANBPlayerState>(PlayerState);

    if (!GameMode || !GState || !PState) return;

    // 턴 및 시간 체크
    if (GState->GetTimeRemaining() <= 0 || GState->GetCurrentTurnPlayer() != this)
    {
        return;
    }

    // 기회 소진 상태 검사
    if (PState->IsOutOfTries())
    {
        NetMulticast_DisplayResult(PState->GetPlayerName(), InputGuess, TEXT("ERROR: Out of Tries!"), PState->GetRemainingTries(), PState->GetMaxTries());
        return;
    }

    if (InputGuess.Len() != 3 || !InputGuess.IsNumeric())
    {
        NetMulticast_DisplayResult(PState->GetPlayerName(), InputGuess, TEXT("ERROR: Invalid Input. Try Again."), PState->GetRemainingTries(), PState->GetMaxTries());
        return; 
    }
    if (InputGuess[0] == InputGuess[1] || InputGuess[1] == InputGuess[2] || InputGuess[0] == InputGuess[2])
    {
        NetMulticast_DisplayResult(PState->GetPlayerName(), InputGuess, TEXT("ERROR: Duplicate Numbers. Try Again."), PState->GetRemainingTries(), PState->GetMaxTries());
        return; 
    }


    PState->SetHasSubmitted(true);
    PState->ConsumeTry();// 기회 차감

    // 숫자 판정
    FString EvaluationResult = GameMode->CheckAnswer(this, InputGuess);
    NetMulticast_DisplayResult(PState->GetPlayerName(), InputGuess, EvaluationResult, PState->GetRemainingTries(), PState->GetMaxTries());

    GameMode->ProcessGameRules(this, EvaluationResult);
}

bool ANBPlayerController::IsMyTurnAndHasTime() const
{
    ANBGameStateBase* GState = Cast<ANBGameStateBase>(GetWorld()->GetGameState());
    if (GState)
    {
        return (GState->GetCurrentTurnPlayer() == this) && (GState->GetTimeRemaining() > 0);
    }
    return false;
}
void ANBPlayerController::NetMulticast_DisplayResult_Implementation(const FString& PlayerName, const FString& Guess, const FString& Result, int32 RemainingTries, int32 MaxTries)
{
    if (IsLocalPlayerController())
    {
        FString ChatMessage = FString::Printf(TEXT("[기회: %d/%d] 입력: %s -> 결과: %s"),
            RemainingTries, MaxTries, *Guess, *Result);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, ChatMessage);
        }
    }
}

void ANBPlayerController::NetMulticast_NoticeGameOver_Implementation(const FString& NoticeMessage)
{
    if (IsLocalPlayerController())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("========================================="));
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("         %s         "), *NoticeMessage));
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("========================================="));
        }
    }
}
