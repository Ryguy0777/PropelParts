#pragma once

#include <nw4r/types_nw4r.h>
#include <revolution/OS/OSContext.h>

namespace nw4r {
namespace db {

void Exception_SetDisplayInfo(u16);

void Exception_Printf_(const char* format, ...);
void ShowMainInfo_(u16 error, const OSContext* context, ulong dsisr, ulong dar);

// I hate this
void* sException();

} // namespace db
} // namespace nw4r