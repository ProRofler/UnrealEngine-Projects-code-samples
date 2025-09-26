#pragma once

#pragma once

class APawn;

class TSUtils
{
  public:
    template <typename T> static T *GetComponent(APawn *PlayerPawn)
    {
        if (!PlayerPawn)
            return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
            return Cast<T>(Component);
    }
};
