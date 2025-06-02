#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
namespace LogUtils {

void LogToScreen(FString &&Message, UEngine *Engine) {
  const float TIME_TO_DISPLAY = 5.0F;
  Engine->AddOnScreenDebugMessage(-1, TIME_TO_DISPLAY, FColor::White, Message);
}

} // namespace LogUtils
