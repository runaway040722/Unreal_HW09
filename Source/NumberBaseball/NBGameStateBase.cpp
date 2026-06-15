#include "NBGameStateBase.h"
#include "Net/UnrealNetwork.h"

ANBGameStateBase::ANBGameStateBase()
{
    TimeRemaining = 0;
    CurrentTurnPlayer = nullptr;
}

void ANBGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ANBGameStateBase, TimeRemaining);
    DOREPLIFETIME(ANBGameStateBase, CurrentTurnPlayer);
}