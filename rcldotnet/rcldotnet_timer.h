// Copyright 2023 Queensland University of Technology.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RCLDOTNET_TIMER_H
#define RCLDOTNET_TIMER_H

#include "rcldotnet_macros.h"

RCLDOTNET_EXPORT
int32_t RCLDOTNET_CDECL native_rcl_timer_call(void *timer_handle);

RCLDOTNET_EXPORT
int32_t RCLDOTNET_CDECL native_rcl_timer_cancel(void *timer_handle);

RCLDOTNET_EXPORT
int32_t RCLDOTNET_CDECL native_rcl_timer_reset(void *timer_handle);

RCLDOTNET_EXPORT
int32_t RCLDOTNET_CDECL native_rcl_timer_is_canceled(void *timer_handle, int32_t *is_canceled);

RCLDOTNET_EXPORT
int32_t RCLDOTNET_CDECL native_rcl_timer_is_ready(void *timer_handle, int32_t *is_ready);

#endif // RCLDOTNET_TIMER_H
