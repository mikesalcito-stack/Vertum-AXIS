#include "VitrumSystemKey.h"
#include "VitrumMarble.h"
// #include "VitrumEncryptedLock.h" // Assume a lock class

AVitrumSystemKey::AVitrumSystemKey()
{
    // Constructor
}

void AVitrumSystemKey::Use(AVitrumMarble* User)
{
    if (User)
    {
        // Find nearby lock with matching ID
        // TArray<AActor*> Locks;
        // UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVitrumEncryptedLock::StaticClass(), Locks);

        // for (AActor* LockActor : Locks)
        // {
        //     if (AVitrumEncryptedLock* Lock = Cast<AVitrumEncryptedLock>(LockActor))
        //     {
        //         if (Lock->LockID == KeyID && FVector::Dist(User->GetActorLocation(), Lock->GetActorLocation()) < 200.0f)
        //         {
        //             Lock->Unlock();
        //             Destroy();
        //             return;
        //         }
        //     }
        // }
    }
}
