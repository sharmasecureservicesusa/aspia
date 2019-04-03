//
// Aspia Project
// Copyright (C) 2019 Dmitry Chapyshev <dmitry@aspia.ru>
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

#ifndef DESKTOP__WIN__DXGI_ADAPTER_DUPLICATOR_H
#define DESKTOP__WIN__DXGI_ADAPTER_DUPLICATOR_H

#include "desktop/shared_desktop_frame.h"
#include "desktop/win/d3d_device.h"
#include "desktop/win/dxgi_context.h"
#include "desktop/win/dxgi_output_duplicator.h"

#include <vector>

#include <wrl/client.h>

namespace desktop {

// A container of DxgiOutputDuplicators to duplicate monitors attached to a single video card.
class DxgiAdapterDuplicator
{
public:
    using Context = DxgiAdapterContext;

    // Creates an instance of DxgiAdapterDuplicator from a D3dDevice. Only
    // DxgiDuplicatorController can create an instance.
    explicit DxgiAdapterDuplicator(const D3dDevice& device);

    // Move constructor, to make it possible to store instances of
    // DxgiAdapterDuplicator in std::vector<>.
    DxgiAdapterDuplicator(DxgiAdapterDuplicator&& other);

    ~DxgiAdapterDuplicator();

    // Initializes the DxgiAdapterDuplicator from a D3dDevice.
    bool initialize();

    // Sequentially calls Duplicate function of all the DxgiOutputDuplicator instances owned by
    // this instance, and writes into |target|.
    bool duplicate(Context* context, SharedFrame* target);

    // Captures one monitor and writes into |target|. |monitor_id| should be between [0, screenCount()).
    bool duplicateMonitor(Context* context, int monitor_id, SharedFrame* target);

    // Returns desktop rect covered by this DxgiAdapterDuplicator.
    Rect desktopRect() const { return desktop_rect_; }

    // Returns the size of one screen owned by this DxgiAdapterDuplicator. |id|
    // should be between [0, screenCount()).
    Rect screenRect(int id) const;

    // Returns the device name of one screen owned by this DxgiAdapterDuplicator
    // in utf8 encoding. |id| should be between [0, screenCount()).
    const std::string& deviceName(int id) const;

    // Returns the count of screens owned by this DxgiAdapterDuplicator. These screens can be
    // retrieved by an interger in the range of [0, screenCount()).
    int screenCount() const;

    void setup(Context* context);

    void unregister(const Context* const context);

    // The minimum num_frames_captured() returned by |duplicators_|.
    int64_t numFramesCaptured() const;

    // Moves |desktop_rect_| and all underlying |duplicators_|. See
    // DxgiDuplicatorController::translateRect().
    void translateRect(const Point& position);

private:
    bool doInitialize();

    const D3dDevice device_;
    std::vector<DxgiOutputDuplicator> duplicators_;
    Rect desktop_rect_;
};

} // namespace desktop

#endif // DESKTOP__WIN__DXGI_ADAPTER_DUPLICATOR_H
