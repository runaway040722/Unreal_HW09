#include "NBGameModeBase.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "NBGameStateBase.h"

ANBGameModeBase::ANBGameModeBase()
{
    PlayerControllerClass = ANBPlayerController::StaticClass();
    PlayerStateClass = ANBPlayerState::StaticClass();
    GameStateClass = ANBGameStateBase::StaticClass();
    TurnDuration = 10; // 각 턴당 10초 제한
}

void ANBGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    GenerateRandomNumbers();

    // 최초 턴 부여 및 타이머 시작
    FTimerHandle StartDelayHandle;
    GetWorldTimerManager().SetTimer(StartDelayHandle, this, &ANBGameModeBase::SwitchToNextTurn, 2.0f, false);
}

void ANBGameModeBase::GenerateRandomNumbers()
{
    SecretNumber.Empty();
    TArray<int32> Candidates;
    for (int32 i = 1; i <= 9; ++i) Candidates.Add(i);

    for (int32 i = 0; i < Candidates.Num(); ++i)
    {
        int32 Index = FMath::RandRange(i, Candidates.Num() - 1);
        Candidates.Swap(i, Index);
    }
    for (int32 i = 0; i < 3; ++i) SecretNumber.Add(Candidates[i]);

    UE_LOG(LogTemp, Warning, TEXT("============= [SERVER] Secret Number: %d %d %d ============="), SecretNumber[0], SecretNumber[1], SecretNumber[2]);
}

void ANBGameModeBase::UpdateTurnTimer()
{
    ANBGameStateBase* GState = Cast<ANBGameStateBase>(GameState);
    if (!GState) return;

    int32 CurrentTime = GState->GetTimeRemaining();
    if (CurrentTime > 0)
    {
        GState->SetTimeRemaining(CurrentTime - 1);
    }
    else
    {
        // 타임아웃
        GetWorldTimerManager().ClearTimer(TurnTimerHandle);

        APlayerController* CurrentPC = GState->GetCurrentTurnPlayer();
        if (CurrentPC)
        {
            ANBPlayerState* PState = Cast<ANBPlayerState>(CurrentPC->PlayerState);
            if (PState && !PState->HasSubmittedThisTurn())
            {
                PState->ConsumeTry();

                if (ANBPlayerController* NBC = Cast<ANBPlayerController>(CurrentPC))
                {
                    NBC->NetMulticast_DisplayResult(PState->GetPlayerName(), TEXT("TIMEOUT"), TEXT("Time out penalty! Try Consumed."), PState->GetRemainingTries(), PState->GetMaxTries());
                }
            }
        }

        ProcessGameRules(CurrentPC, TEXT("TIMEOUT"));
    }
}

void ANBGameModeBase::SwitchToNextTurn()
{
    ANBGameStateBase* GState = Cast<ANBGameStateBase>(GameState);
    if (!GState) return;

    GetWorldTimerManager().ClearTimer(TurnTimerHandle);

    TArray<APlayerController*> ActivePlayers;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            ActivePlayers.Add(PC);
        }
    }

    if (ActivePlayers.Num() == 0) return;

    APlayerController* NextPlayer = nullptr;
    APlayerController* CurrentPlayer = GState->GetCurrentTurnPlayer();

    if (!CurrentPlayer)
    {
        NextPlayer = ActivePlayers[0];
    }
    else
    {
        int32 CurrentIndex = INDEX_NONE;
        if (ActivePlayers.Find(CurrentPlayer, CurrentIndex))
        {
            int32 NextIndex = (CurrentIndex + 1) % ActivePlayers.Num();
            NextPlayer = ActivePlayers[NextIndex];
        }
        else
        {
            NextPlayer = ActivePlayers[0];
        }
    }

    // 새로운 턴 상태
    GState->SetCurrentTurnPlayer(NextPlayer);
    GState->SetTimeRemaining(TurnDuration);

    if (NextPlayer)
    {
        if (ANBPlayerState* NextPState = Cast<ANBPlayerState>(NextPlayer->PlayerState))
        {
            NextPState->SetHasSubmitted(false); 
        }

        FString TurnNotice = FString::Printf(TEXT("YOUR TURN: %s"), *NextPlayer->PlayerState->GetPlayerName());
        for (auto PC : ActivePlayers)
        {
            if (ANBPlayerController* NBC = Cast<ANBPlayerController>(PC))
            {
                NBC->NetMulticast_NoticeGameOver(TurnNotice);
            }
        }
    }

    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANBGameModeBase::UpdateTurnTimer, 1.0f, true);
}