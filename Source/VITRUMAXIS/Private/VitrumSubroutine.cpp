#include "VitrumSubroutine.h"
#include "VitrumMarble.h"

AVitrumSubroutine::AVitrumSubroutine()
{
    // Base constructor
}

void AVitrumSubroutine::Use(AVitrumMarble* User)
{
    // Base use, to be overridden in subclasses
    Destroy();
}
