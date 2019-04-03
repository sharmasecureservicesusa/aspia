//
// Aspia Project
// Copyright (C) 2018 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef DESKTOP__SCREEN_CAPTURER_GDI_H
#define DESKTOP__SCREEN_CAPTURER_GDI_H

#include "base/win/scoped_hdc.h"
#include "base/win/scoped_thread_desktop.h"
#include "desktop/screen_capturer.h"
#include "desktop/screen_capture_frame_queue.h"

namespace desktop {

class Differ;
class EffectsDisabler;
class WallpaperDisabler;

class ScreenCapturerGDI : public ScreenCapturer
{
public:
    enum Flags
    {
        DISABLE_EFFECTS = 1,
        DISABLE_WALLPAPER = 2
    };

    ScreenCapturerGDI(uint32_t flags);
    ~ScreenCapturerGDI();

    // ScreenCapturer implementation.
    int screenCount() override;
    bool screenList(ScreenList* screens) override;
    bool selectScreen(ScreenId screen_id) override;
    const Frame* captureFrame() override;

private:
    bool prepareCaptureResources();

    const uint32_t flags_;

    ScreenId current_screen_id_ = kFullDesktopScreenId;
    QString current_device_key_;

    base::ScopedThreadDesktop desktop_;
    Rect desktop_dc_rect_;

    std::unique_ptr<Differ> differ_;
    std::unique_ptr<base::win::ScopedGetDC> desktop_dc_;
    base::win::ScopedCreateDC memory_dc_;

    ScreenCaptureFrameQueue<Frame> queue_;

    std::unique_ptr<EffectsDisabler> effects_disabler_;
    std::unique_ptr<WallpaperDisabler> wallpaper_disabler_;

    DISALLOW_COPY_AND_ASSIGN(ScreenCapturerGDI);
};

} // namespace desktop

#endif // DESKTOP__SCREEN_CAPTURER_GDI_H
