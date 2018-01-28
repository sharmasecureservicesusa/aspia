//
// PROJECT:         Aspia
// FILE:            base/scoped_privilege.cc
// LICENSE:         Mozilla Public License Version 2.0
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "base/scoped_privilege.h"
#include "base/logging.h"

namespace aspia {

ScopedProcessPrivilege::ScopedProcessPrivilege(const WCHAR* name)
{
    DCHECK(name);

    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES,
                          process_token_.Recieve()))
    {
        PLOG(LS_ERROR) << "OpenProcessToken failed";
        return;
    }

    memset(&privileges_, 0, sizeof(privileges_));

    if (!LookupPrivilegeValueW(nullptr,
                               name,
                               &privileges_.Privileges[0].Luid))
    {
        PLOG(LS_ERROR) << "LookupPrivilegeValueW failed";
        return;
    }

    privileges_.PrivilegeCount = 1;
    privileges_.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(process_token_,
                               FALSE,
                               &privileges_,
                               0,
                               nullptr,
                               nullptr))
    {
        PLOG(LS_ERROR) << "AdjustTokenPrivileges failed";
        return;
    }

        is_enabled_ = true;
}

ScopedProcessPrivilege::~ScopedProcessPrivilege()
{
    if (is_enabled_)
    {
        privileges_.PrivilegeCount = 1;
        privileges_.Privileges[0].Attributes = 0;

        if (!AdjustTokenPrivileges(process_token_,
                                   FALSE,
                                   &privileges_,
                                   0,
                                   nullptr,
                                   nullptr))
        {
            PLOG(LS_ERROR) << "AdjustTokenPrivileges failed";
        }
    }
}

} // namespace aspia
