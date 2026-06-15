#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"

ANBPlayerState::ANBPlayerState()
{
    MaxTries = 3;
    RemainingTries = MaxTries; 
    bHasSubmittedThisTurn = false;
}

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ANBPlayerState, RemainingTries);
    DOREPLIFETIME(ANBPlayerState, MaxTries);
    DOREPLIFETIME(ANBPlayerState, bHasSubmittedThisTurn);
}